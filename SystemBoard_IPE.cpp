/* 
 * File:   SystemBoard_IPE.cpp
 * Author: usamanwar
 * 
 * Created on June 17, 2014, 2:32 PM
 */

#include "SystemBoard_IPE.h"
#include "GPAtoHVATranslator_IPE.h"
#include "GVAtoHVATranslation_IPE.h"
#include "SignalHandler.h"
#include <pthread.h>

/** Constructor of SystemBoard
  * Creates a new virtual board, create required processor instances, initializes memory syystem and IO devices
  * @param numcores: Number of cores required
  * @param uboot_path: Path of bootloader binary
  * @param kernel_path: Path of kernel binary
  * 
  * @retval none
  */
SystemBoard_IPE::SystemBoard_IPE(Specifications* spec)
{
    int i, numcores;
    this->specs = spec;
    numcores = this->specs->getNumCores();
    this->initializeSystemMemory();
    this->loadFirmware();
    initializeMemorySegments();
    this->Processors = new Processor_IPE[numcores]();
    this->CIUnit = new CIU(this->Processors, numcores);
    this->UARTunit = new UART(this->CIUnit);
    signalHandler::registerSignals();
    signalHandler::registerUART(this->UARTunit); //Registering UART in signal handler to deliver Ctrl+C signal to guest
    
    pthread_t thread[numcores];
    
    pthread_t CIU_Thread;
    if (pthread_create(&CIU_Thread, NULL, &SystemBoard_IPE::startCIU, (void*) (this->CIUnit))) 
    {
        printf("Error creating CIU\n");
        exit(1);
    }
    pthread_t UART_Thread;
    if (pthread_create(&UART_Thread, NULL, &SystemBoard_IPE::startUART, (void*) (this->UARTunit))) 
    {
        printf("Error creating UART\n");
        exit(1);
    }
    
    this->Disk = new VirtioBlk(this->CIUnit);
    signalHandler::registerDisk(this->Disk);  //Registering Net card in signal handler to properly close vhost and TAP files
    pthread_t Disk_Thread;
    if (pthread_create(&Disk_Thread, NULL, &SystemBoard_IPE::startDisk, (void*) (this->Disk))) 
    {
        printf("Error creating VBLK\n");
        exit(1);
    }
    
#ifdef ENABLE_VNET
    this->NetCard = new VirtioNet(this->CIUnit);
    signalHandler::registerVnet(this->NetCard);  //Registering Net card in signal handler to properly close vhost and TAP files
    pthread_t Net_Thread;
    if (pthread_create(&Net_Thread, NULL, &SystemBoard_IPE::startNetCard, (void*) (this->NetCard))) 
    {
        printf("Error creating VNET\n");
        exit(1);
    }
#endif
    
    for (i = 0; i < numcores; i++)
    {
        uint64_t Ebase = this->Processors[i].getCP0()->read(H2_CP0_EBASE_1,1);
        Ebase = Ebase & ~EBASE_CORENUM_MASK;
        Ebase = Ebase | i;
        this->Processors[i].getCP0()->write(Ebase,H2_CP0_EBASE_1,1);
        
        if(pthread_create(&thread[i], NULL, &SystemBoard_IPE::startCore,(void*)(&this->Processors[i])))
        {
            printf("Error creating core number %d\n",i);
            exit(1);
        }
    }

    pthread_join(CIU_Thread,NULL);
    pthread_join(UART_Thread,NULL);
    pthread_join(Disk_Thread,NULL);
        
#ifdef ENABLE_VNET
    pthread_join(Net_Thread,NULL);
#endif
    
    for (i = 0; i < numcores; i++)
    {
        pthread_join(thread[i],NULL);
    }
}

/** Destructor of SystemBoard
  * Destroys all instances of processors along with IO devices
  * @param none
  * 
  * @retval none
  */
SystemBoard_IPE::~SystemBoard_IPE() {
    delete this->CIUnit;
    delete this->UARTunit;
    delete this->specs;
    delete this->Disk;
#ifdef ENABLE_VNET
    delete this->NetCard;
#endif
    delete  [] this->Processors;
}

/** Create virtual memory for guest using memory mapping
  * @param none
  * 
  * @retval none
  */
void SystemBoard_IPE::initializeSystemMemory()
{
    GPAtoHVATranslator_IPE::initialize_GPA_subsections(Allocated_memory);
}

/** Some memory regions are initialized with value other than zero. This function assign these values to such memory regions
  * @param none
  * 
  * @retval none
  */
void SystemBoard_IPE::initializeMemorySegments()
{
    uint64_t NPEI_DBG_Addr;
    if(GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(NPEI_DBG_ADDRESS, &NPEI_DBG_Addr) == false)
    {
        printf("Unable to translate NPEI_DBG address.\n");
        exit(1);
    }
    *((uint64_t*)NPEI_DBG_Addr) = NPEI_DBG_DATA;
}

/** Loads the bootloader and kernel in memory of guest
  * @param none
  * 
  * @retval none
  */
void SystemBoard_IPE::loadFirmware()
{
    const char * uboot = this->specs->getUbootPath();
    const char * kernel_image = this->specs->getKernelPath();
    uint64_t binaryLoadAddr,kernelLoadAddr;
    if(GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(UBOOT_LOAD_ADDRESS, &binaryLoadAddr) == false)
    {
        printf("Unable to load binary.\n");
        exit(1);
    }
    if(GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(KERNEL_LOAD_ADDRESS, &kernelLoadAddr) == false)
    {
        printf("Unable to load binary.\n");
        exit(1);
    }
    this->loadfile(uboot,binaryLoadAddr);
    this->loadfile(kernel_image,kernelLoadAddr);
}

/** Copies the specified file on required memory region
  * @param filename: Name of file to be copied
  * @param loadAddr: Address at which file should be copied
  * 
  * @retval none
  */
void SystemBoard_IPE::loadfile(const char* filename,uint64_t loadAddr) {
    uint64_t f_size = 0;

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error:Unable to open file.\n");
        exit(1);
    }

    f_size = this->getSize(filename);

    fread((void*) loadAddr, f_size, 1, fp);
    fclose(fp);

    if (DBG)printf("\n\tFile Loading address...: 0x%016llx\n", (uint64_t) loadAddr);
    if (DBG)printf("\n\n##################################################################\n");
}

/** Calculates the total size of a file
  * @param file_name: Name of file
  * 
  * @retval size of the given file
  */
uint64_t SystemBoard_IPE::getSize (const char* file_name) {
    struct stat sb;
    if (stat (file_name, & sb) != 0) {
        fprintf (stderr, "'stat' failed for '%s': %s.\n",file_name, strerror (errno));
        exit (1);
    }
    return sb.st_size;
}

/** Registers processor with signal handler and starts execution
  * This function is called when thread of processor is created
  * @param core: Instance of virtual processor
  * 
  * @retval none
  */
void *SystemBoard_IPE::startCore(void* core)
{
    Processor_IPE *processor = (Processor_IPE*) core;
    
    signalHandler::registerProcessor(processor); //Registers a processor pointer in map and used in case of exceptions.
    
    processor->execute();
    printf("Terminated core %d.\n",processor->getCP0()->corenum());
}

/** Starts execution of CIU
  * This function is called when CIU thread is created
  * @param ciu: Instance of virtual Central Interrupt Unit
  * 
  * @retval none
  */
void *SystemBoard_IPE::startCIU(void* ciu)
{
   CIU *CIUnit = (CIU *) ciu;
   
   CIUnit->start();
    
    printf("CIU terminated\n");
}

/** Starts execution of UART
  * This function is called when UART thread is created
  * @param uart: Instance of virtual UART
  * 
  * @retval none
  */
void *SystemBoard_IPE::startUART(void* uart)
{
    UART *uartunit = (UART*) uart;
    
    uartunit->start();
    
    printf("UART terminated\n");
}

/** Starts execution of Disk
  * This function is called when Disk thread is created
  * @param uart: Instance of virtual Disk
  * 
  * @retval none
  */
void *SystemBoard_IPE::startDisk(void* disk)
{
    VirtioBlk *diskunit = (VirtioBlk*) disk;
    
    diskunit->start();
    
    printf("UART terminated\n");
}

#ifdef ENABLE_VNET
/** Starts execution of Network card
  * This function is called when network thread is created
  * @param netcard: Instance of virtual network card
  * 
  * @retval none
  */
void *SystemBoard_IPE::startNetCard(void* netcard)
{
    VirtioNet *card = (VirtioNet*) netcard;
    
    card->start();
    
    printf("Vnet terminated\n");
}
#endif