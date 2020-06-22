/* 
 * File:   VirtioBlk.cpp
 * Author: usamanwar
 * 
 * Created on 28 September 2015, 17:06
 */

#include "VirtioBlk.h"

static pthread_t reqPollingAgent;

/** Constructor for VirtioBlk
  * VirtioBlk class is container class of Virtio Block device. IOManager interacts with this class for using device.
  * This class is inherrited from VirtioDevice class. Virtio Block functionality is implemented on base Virtio device.
  * This class contains virtual disk and also Virtio Block related registers. It initializes device alongwith vhost client.
  * In our case, we are using raw file as disk.
  * @param CIUunit: Pointer to CIU to generate device interrupt
  * 
  * @retval none
  */
VirtioBlk::VirtioBlk(CIU* CIUunit)
{
    IOManager* manager = IOManager::getInstance();
    printf("Initializing VBLK........\n");
    this->CIUunit = CIUunit;
    this->BaseAddr = manager->getDeviceHVA(VBLK_ID);
    this->MessageBox = manager->getMessageBox(VBLK_ID);
    this->config = (virtio_blk_config *)(this->BaseAddr + VIRTIO_MMIO_CONFIG);
    this->NumQueues = 1;
    this->Initialize(BLOCK_DEVICE_ID);
    this->Backendfd = this->OpenDisk();
    this->InitConfig();
    this->InitHostClient();
}

/** Initializes configuration space of Virtio Block device
  * Disk capacity, block size etc for disk are set in this function.
  * @param CIUunit: Pointer to CIU to generate device interrupt
  * 
  * @retval none
  */
void VirtioBlk::InitConfig()
{
    if(VIRTIOBLK_DBG) printf("%s\n",__func__);
    this->config->capacity = DISK_SECTORS;
    this->config->blk_size = 512;
    this->config->seg_max = 2;
    this->config->size_max = 4096;
}

/** Destructor for VirtioBlk
  * Closes virtual disk file.
  * @param none
  * 
  * @retval none
  */
VirtioBlk::~VirtioBlk() {
    if(VIRTIOBLK_DBG) printf("%s\n",__func__);
    close(this->Backendfd);
}

/** Starts working of Virtio Block device.
  * It starts base functionality of virtio device and initializes polling agents for queues. 
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::VblkStart()
{
    if(VIRTIOBLK_DBG) printf("%s\n",__func__);
    this->StartDevice();
    this->InitPollingAgent();
}

/** Starts working of Virtio Block device.
  * Thread for Net device calls this function to start its working
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::start()
{
    printf("Initialized VBlk\n");
        
    while(true)
    {
        pthread_cond_wait(&(this->MessageBox->cond_device), &(this->MessageBox->lock_device)); //Waiting for notification from core
        if(this->MessageBox->RW == WRITE) //Writing to UART register
            this->vnet_io_write();
        /*else    //Reading from UART register
            this->vnet_io_read();*/
        this->MessageBox->isServiced = true;
    }
}

/** This function is invoked when core writes to any of device registers
  * If general device related register is altered, then it handles it using function inherited from Virtio Device
  * If Virtio BLock related register is altered then this class handles it by itself
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::vnet_io_write()
{
    uint32_t addr = this->MessageBox->addr & VNET_OFFSET_MASK;
    uint32_t data = (uint32_t)this->MessageBox->data;
    
    (addr >= VIRTIO_MMIO_CONFIG ) ? this->updateConfig() : this->PerformOperation(addr, data);
}

/** Make required operations when device config register is changed
  * This function is invoked when core writes to Virtio Block device specific register
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::updateConfig()
{
    printf("%s\n",__func__);
}

/** Opens virtual disk file. If file doesn't exist then it creates file
  * @param none
  * 
  * @retval -1 on failure, file descriptor on success
  */
int VirtioBlk::OpenDisk()
{
    int fd = -1;	
    if(VIRTIOBLK_DBG) printf("%s\n",__func__);
    if (0 != access(DISK_PATH, F_OK))
    {
        char ddcmd[128];
        sprintf(ddcmd,"dd if=/dev/zero of=%s bs=512 count=%d &> /dev/null",DISK_PATH,DISK_SECTORS);
        printf("Ramdisk file not found\n");
        printf("Be patient. Creating ramdisk file\n");
        system(ddcmd);
        printf("Created Ramdisk file");
    }
        
    if ((fd = open(DISK_PATH, O_RDWR)) == -1)
        err(1, "open");	
        
    return fd;
}

/** Initializes vhost client and gets its features
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::InitHostClient()
{
    if(this->HostClient.Initialize(VHOST_BLK_PATH) < 0)
    {
        printf("Need to load Vhost-blk module.\nLoading module.....\n");
        system("insmod /lib/modules/2.6.32.27-Cavium-Octeon/drivers/vhost/vhost_blk.ko");
        if(this->HostClient.Initialize(VHOST_BLK_PATH) < 0)
        {
            printf("Error: Loading Vhost-blk module.\nExiting hypervisor\n");
            exit(1);
        }
    }
    this->GetBackendHostFeatures();
}

/** Checks which register is altered and performs operation according to it
  * @param addr: Address of register which is manipulated
  * @param data: Value which is written to register
  * 
  * @retval none
  */
void VirtioBlk::PerformOperation(uint32_t Addr, uint32_t data)
{
    switch(Addr)
    {
        default:
        case VIRTIO_MMIO_HOST_FEATURES_SEL:
            this->SelHostFeatures(data);
            break;
            
        case VIRTIO_MMIO_GUEST_FEATURES:
            this->SetGuestFeatures(data);
            break;
            
        case VIRTIO_MMIO_GUEST_FEATURES_SEL:
            
            break;
            
        case VIRTIO_MMIO_QUEUE_SEL:
            this->UpdateQueue(data);
            break;
            
        case VIRTIO_MMIO_QUEUE_NUM:
            
            break;
            
        case VIRTIO_MMIO_QUEUE_ALIGN:
            
            break;
            
        case VIRTIO_MMIO_QUEUE_PFN:
            this->UpdateVirtQueue(data);
            break;
            
        case VIRTIO_MMIO_QUEUE_NOTIFY:
            if(!this->DeviceStarted)
                this->VblkStart();
            this->NotifyDevice(data);
            break;
            
        case VIRTIO_MMIO_INTERRUPT_ACK:
            this->AckInterrupt(data);
            break;
            
        case VIRTIO_MMIO_STATUS:
//            this->UpdateStatus(data);
            break;
    }
}

/** Gets backend features of Vhost and set some block related feature flags
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::GetBackendHostFeatures()
{
//    uint64_t features = this->HostClient.getFeatures();
    uint64_t features = VIRTIO_RING_F_INDIRECT_DESC | VIRTIO_RING_F_EVENT_IDX;
    features |= 0x1 << VIRTIO_BLK_F_FLUSH;
    features |= 0x1 << VIRTIO_BLK_F_SEG_MAX;
    features |= 0x1 << VIRTIO_BLK_F_SIZE_MAX;
    features |= 0x1 << VIRTIO_BLK_F_BLK_SIZE;
    this->BackendHostFeatures = features;
}

/** Reset block related features flags and returns remaining features
  * @param none
  * 
  * @retval return features negotiated with Vhost
  */
uint64_t VirtioBlk::GetBackendGuestFeatures()
{
    uint64_t features = this->AckGuestFeatures;
    features &= ~(0x1 << VIRTIO_BLK_F_BARRIER);
    features &= ~(0x1 << VIRTIO_BLK_F_SIZE_MAX);
    features &= ~(0x1 << VIRTIO_BLK_F_SEG_MAX);
    features &= ~(0x1 << VIRTIO_BLK_F_GEOMETRY);
    features &= ~(0x1 << VIRTIO_BLK_F_RO);
    features &= ~(0x1 << VIRTIO_BLK_F_BLK_SIZE);
    features &= ~(0x1 << VIRTIO_BLK_F_SCSI);
    features &= ~(0x1 << VIRTIO_BLK_F_FLUSH);
    return features;
}

/** Updates interrupt status of device
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::UpdateInterrupt()
{
    uint32_t intStatus = *this->InterruptStatus;
    if(intStatus > 0)
    {
        this->CIUunit->generateInterrupt(CIU_SUM_GPIO0);
    }
    else
        this->CIUunit->clearInterrupt(CIU_SUM_GPIO0);
    if(VIRTIOBLK_DBG) printf("%s\n",__func__);
}

/** Invoked when VIRTIO_MMIO_INTERRUPT_ACK register is manipulated
  * It updates the interrupt status after acknowledgement
  * @param data: Acknowledgement bits
  * 
  * @retval none
  */
void VirtioBlk::AckInterrupt(int data)
{
    *this->InterruptStatus &= ~data;
    this->UpdateInterrupt();
}

/** Updates interrupt status of device
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::UpdateStatus(int stat)
{
    uint32_t status = swap_endianess32(stat);
    if(VIRTIOBLK_DBG) printf("%s\n",__func__);
    if(status & VIRTIO_CONFIG_S_DRIVER_OK && !this->DeviceStarted)
        this->VblkStart();
}

/** Generates interrupt for this device
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::GenerateInterrupt()
{
    uint32_t intStatus = VIRTIO_MMIO_INT_VRING;
    *this->InterruptStatus = swap_endianess32(intStatus);
    if(VIRTIOBLK_DBG) printf("%s\n",__func__);
    this->UpdateInterrupt();
}

/** Initializes polling agents for eventfd file
  * @param none
  * 
  * @retval none
  */
void VirtioBlk::InitPollingAgent()
{
    if (pthread_create(&reqPollingAgent, NULL, &VirtioBlk::StartReqPolling, (void*) this)) 
    {
        printf("Error creating tx polling agent for eventfd.\n");
        pthread_exit(NULL);
    }
}

/** Starts polling for eventfd file of Request queue
  * @param client: pointer to this device
  * 
  * @retval none
  */
void *VirtioBlk::StartReqPolling(void* client)
{
    VirtioBlk *device = (VirtioBlk*) client;
    VirtQueue *queue = device->getQueue(REQUEST_QUEUE);
    queue->EnableGuestPolling();
    
    while(true)
    {
        if(queue->PollGuest() > 0)
            device->GenerateInterrupt();
    }
}
