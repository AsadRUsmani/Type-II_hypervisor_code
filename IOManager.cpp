#include "IOManager.h"
#include "GVAtoHVATranslation_IPE.h"

IOManager* IOManager::iomanager = NULL;

/** Initializes instance of IOManager. Also initializes mutex for each device message box and gets translation
  * from Guest Virtual to Host Virtual address for each device
  * @param none
  * 
  * @retval none
  */
IOManager::IOManager()
{
    for(int i = 0; i < AVAILABLE_DEVICES; i++)
    {
        pthread_mutex_init(&(this->MessageBox[i].lock_device),NULL);
        pthread_cond_init(&(this->MessageBox[i].cond_device),NULL);
        this->MessageBox[i].isServiced = true;
    }
    if(GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(UART1_BASE_ADDR,&(this->DeviceHVA[UART_ID])) == false)
    {
        printf("Unable to get UART addr\n");
        exit(1);
    }
    if(GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(CIU_BASE_ADDR,&(this->DeviceHVA[CIU_ID])) == false)
    {
        printf("Unable to get CIU addr\n");
        exit(1);
    }
    if(GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(VBLK_BASE_ADDR,&(this->DeviceHVA[VBLK_ID])) == false)
    {
        printf("Unable to get VBLK addr\n");
        exit(1);
    }
#ifdef ENABLE_VNET
    if(GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(VNET_BASE_ADDR,&(this->DeviceHVA[VNET_ID])) == false)
    {
        printf("Unable to get VNET addr\n");
        exit(1);
    }
#endif
}

/** Destroys IOManager instance along with mutexes for each device
  * @param none
  * 
  * @retval none
  */
IOManager::~IOManager()
{
    for(int i = 0; i < AVAILABLE_DEVICES; i++)
    {
        pthread_mutex_destroy(&(this->MessageBox[i].lock_device));
        pthread_cond_destroy(&(this->MessageBox[i].cond_device));
    }
}

/** IOManager is singleton object and can have only one instance. If that instance exist it returns that instance 
  * otherwise create an instance and and return pointer to it
  * @param none
  * 
  * @retval pointer to instance of IOManager
  */
IOManager* IOManager::getInstance()
{
    if(IOManager::iomanager == NULL)
        IOManager::iomanager = new IOManager();
    return IOManager::iomanager;
}

/** Gives the pointer to message box of device with given ID
  * @param id: ID of device
  * 
  * @retval pointer to message box of device
  */
DeviceMessageBox* IOManager::getMessageBox(int id)
{
    return &(this->MessageBox[id]);
}

/** Gives the base address of device with specified ID
  * @param id: ID of device
  * 
  * @retval the base HVA of the given device
  */
uint64_t IOManager::getDeviceHVA(int id)
{
    return this->DeviceHVA[id];
}

/** Translates the Guest Virtual Address to Host Virtual Address for IO devices.
  * Also updates message box of the device for which translation is requested.
  * @param GVA: Address to be translated
  * @param HVA: pointer to location where translated address should be placed
  * @param context: Context of the current translation
  * 
  * @retval none
  */
void IOManager::GVAtoHVA(uint64_t GVA, uint64_t* HVA, Context_IPE* context)
{
    if(GVA >= CIU_BASE_ADDR && GVA < CIU_END_ADDR)
    {
        if(context->getLDST() == WRITE)
            this->updateMessageBox(CIU_ID, GVA, context);
        else //In case of read, wait for previous device operation to complete so core can get updated register vlaues
            while(!this->MessageBox[CIU_ID].isServiced);
        *HVA = this->DeviceHVA[CIU_ID] | (GVA & CIU_OFFSET_MASK);
    }
    else if(GVA >= UART0_BASE_ADDR && GVA < UART1_END_ADDR)
    {
        this->updateMessageBox(UART_ID, GVA, context);
        *HVA = this->DeviceHVA[UART_ID] | (GVA & UART_OFFSET_MASK);
    }
#ifdef ENABLE_VNET
    else if(GVA >= VNET_BASE_ADDR && GVA < VNET_END_ADDR)
    {
//        printf("Vnet: Accessing 0x%016lx at PC 0x%016lx\tRW = %d\n",GVA,context->getPc(),context->getLDST());
        if(context->getLDST() == WRITE)
            this->updateMessageBox(VNET_ID, GVA, context);
        else
            while(!this->MessageBox[VNET_ID].isServiced);
        *HVA = this->DeviceHVA[VNET_ID] | (GVA & VNET_OFFSET_MASK);
    }
#endif
    else if(GVA >= VBLK_BASE_ADDR && GVA <= VBLK_END_ADDR)
    {
//        printf("VBLK: Accessing 0x%016lx at PC 0x%016lx\tRW = %d\t",GVA,context->getPc(),context->getLDST());
        if(context->getLDST() == WRITE)
            this->updateMessageBox(VBLK_ID, GVA, context);
        else
            while(!this->MessageBox[VBLK_ID].isServiced);
        *HVA = this->DeviceHVA[VBLK_ID] | (GVA & VNET_OFFSET_MASK);
    }
    else if (this->ioaddrCache.Check_cache(GVA, HVA, context) == false)
    {
        GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(GVA,HVA);
        this->ioaddrCache.Update_cache(GVA,*HVA,WRITE);
    }
        
}

/** Updates the message box of the specified device and notified the device thread to take action
  * @param id: ID of the device for which message box should be updated
  * @param addr: Register address which is being accessed by CPU
  * @param context: Context of the current translation
  * 
  * @retval none
  */
void IOManager::updateMessageBox(int id, uint64_t addr, Context_IPE* context)
{
    DeviceMessageBox* device = &(this->MessageBox[id]);
    
    while(!device->isServiced); //Wait until previous message is serviced
    device->isServiced = false;
    
    pthread_mutex_lock(&(device->lock_device));
    pthread_cond_signal(&(device->cond_device)); //Notifying device thread
    device->RW = context->getLDST();
    device->data = context->getWriteData();
    device->addr = addr;
    pthread_mutex_unlock(&(device->lock_device));
}