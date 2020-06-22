#include "Virtionet.h"

static pthread_t txPollingAgent, rxPollingAgent;

/** Constructor for VirtioNet
  * VirtioNet class is container class of Virtio Net device. IOManager interacts with this class for using device.
  * This class is inherrited from VirtioDevice class. Virtio Network functionality is implemented on base Virtio device.
  * This class contains TAP interface and also Virtio Net related registers. It initializes device alongwith vhost client.
  * @param CIUunit: Pointer to CIU to generate device interrupt
  * 
  * @retval none
  */
VirtioNet::VirtioNet(CIU* CIUunit)
{
    IOManager* manager = IOManager::getInstance();
    printf("Initializing VNET........\n");
    this->CIUunit = CIUunit;
    this->BaseAddr = manager->getDeviceHVA(VNET_ID);
    this->MessageBox = manager->getMessageBox(VNET_ID);
    this->config = (virtio_net_config *)(this->BaseAddr + VIRTIO_MMIO_CONFIG);
    this->NumQueues = 2;
    this->Initialize(NETWORK_CARD_ID);
    this->tap = new TapInterface();
    this->Backendfd = this->tap->getTapfd();
    this->InitHostClient();
}

/** Destructor for VirtioNet
  * Deletes TAP interface
  * @param none
  * 
  * @retval none
  */
VirtioNet::~VirtioNet()
{
    delete this->tap;
}

/** Starts working of Virtio Net device.
  * Sets offloads for Tap interface and sets vhost features on base of features negotiated by driver.
  * It also starts base functionality of virtio device and initializes polling agents for queues. 
  * @param none
  * 
  * @retval none
  */
void VirtioNet::VnetStart()
{
    if(VIRTIONET_DBG) printf("%s\n",__func__);
    this->tap->TapSetOffload(this->AckGuestFeatures);
    this->HostClient.setFeatures(this->GetBackendGuestFeatures());
    this->StartDevice();
    this->InitPollingAgent();
}

/** Starts working of Virtio Net device.
  * Thread for Net device calls this function to start its working
  * @param none
  * 
  * @retval none
  */
void VirtioNet::start()
{
    printf("Initialized VNET\n");
        
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
  * If Virtio Net related register is altered then this class handles it by itself
  * @param none
  * 
  * @retval none
  */
void VirtioNet::vnet_io_write()
{
    uint32_t addr = this->MessageBox->addr & VNET_OFFSET_MASK;
    uint32_t data = (uint32_t)this->MessageBox->data;
    
    (addr >= VIRTIO_MMIO_CONFIG ) ? this->updateConfig() : this->PerformOperation(addr, data);
}

/** Make required operations when device config register is changed
  * This function is invoked when core writes to Virtio Net device specific register
  * @param none
  * 
  * @retval none
  */
void VirtioNet::updateConfig()
{
    printf("%s\n",__func__);
}

/** Checks which register is altered and performs operation according to it
  * @param addr: Address of register which is manipulated
  * @param data: Value which is written to register
  * 
  * @retval none
  */
void VirtioNet::PerformOperation(uint32_t Addr, uint32_t data)
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
            this->NotifyDevice(data);
            break;
            
        case VIRTIO_MMIO_INTERRUPT_ACK:
            this->AckInterrupt(data);
            break;
            
        case VIRTIO_MMIO_STATUS:
            this->UpdateStatus(data);
            break;
    }
}

/** Invoked when VIRTIO_MMIO_STATUS register is manipulated
  * Starts the device when driver gives OK status to device
  * @param stat: Status of device
  * 
  * @retval none
  */
void VirtioNet::UpdateStatus(int stat)
{
    uint32_t status = swap_endianess32(stat);
    if(VIRTIONET_DBG) printf("%s\n",__func__);
    if(status & VIRTIO_CONFIG_S_DRIVER_OK)
        this->VnetStart();
}

/** Invoked when VIRTIO_MMIO_INTERRUPT_ACK register is manipulated
  * It updates the interrupt status after acknowledgement
  * @param data: Acknowledgement bits
  * 
  * @retval none
  */
void VirtioNet::AckInterrupt(int data)
{
    *this->InterruptStatus &= ~data;
    this->UpdateInterrupt();
}

/** Updates interrupt status of device
  * @param none
  * 
  * @retval none
  */
void VirtioNet::UpdateInterrupt()
{
    uint32_t intStatus = *this->InterruptStatus;
    if(intStatus > 0)
        this->CIUunit->generateInterrupt(CIU_SUM_GPIO1);
    else
        this->CIUunit->clearInterrupt(CIU_SUM_GPIO1);
    if(VIRTIONET_DBG) printf("%s\n",__func__);
}

/** Generates interrupt for this device
  * @param none
  * 
  * @retval none
  */
void VirtioNet::GenerateInterrupt()
{
    uint32_t intStatus = VIRTIO_MMIO_INT_VRING;
    *this->InterruptStatus = swap_endianess32(intStatus);
    if(VIRTIONET_DBG) printf("%s\n",__func__);
    this->UpdateInterrupt();
}

/** Initializes vhost client and gets its features
  * @param none
  * 
  * @retval none
  */
void VirtioNet::InitHostClient()
{
    this->HostClient.Initialize(VHOST_NET_PATH);
    this->GetBackendHostFeatures();
}

/** Gets backend features of Vhost and set some network related feature flags
  * @param none
  * 
  * @retval none
  */
void VirtioNet::GetBackendHostFeatures()
{
    uint64_t features = this->HostClient.getFeatures();
    features |= (0x1 << VIRTIO_NET_F_CSUM);
    features |= (0x1 << VIRTIO_NET_F_HOST_TSO4);
    features |= (0x1 << VIRTIO_NET_F_HOST_TSO6);
    features |= (0x1 << VIRTIO_NET_F_HOST_ECN);
    features |= (0x1 << VIRTIO_NET_F_HOST_UFO);

    /*features |= (0x1 << VIRTIO_NET_F_GUEST_CSUM);
    features |= (0x1 << VIRTIO_NET_F_GUEST_TSO4);
    features |= (0x1 << VIRTIO_NET_F_GUEST_TSO6);
    features |= (0x1 << VIRTIO_NET_F_GUEST_ECN);
    features |= (0x1 << VIRTIO_NET_F_GUEST_UFO);*/
    this->BackendHostFeatures = features;
}

/** Reset network related features flags and returns remaining features
  * @param none
  * 
  * @retval return features negotiated with Vhost
  */
uint64_t VirtioNet::GetBackendGuestFeatures()
{
    uint64_t features = this->AckGuestFeatures;
    features &= ~(0x1 << VIRTIO_NET_F_CSUM);
    features &= ~(0x1 << VIRTIO_NET_F_HOST_TSO4);
    features &= ~(0x1 << VIRTIO_NET_F_HOST_TSO6);
    features &= ~(0x1 << VIRTIO_NET_F_HOST_ECN);
    features &= ~(0x1 << VIRTIO_NET_F_HOST_UFO);

    features &= ~(0x1 << VIRTIO_NET_F_GUEST_CSUM);
    features &= ~(0x1 << VIRTIO_NET_F_GUEST_TSO4);
    features &= ~(0x1 << VIRTIO_NET_F_GUEST_TSO6);
    features &= ~(0x1 << VIRTIO_NET_F_GUEST_ECN);
    features &= ~(0x1 << VIRTIO_NET_F_GUEST_UFO);
    return features;
}

/** Initializes polling agents for eventfd files
  * @param none
  * 
  * @retval none
  */
void VirtioNet::InitPollingAgent()
{
    if (pthread_create(&txPollingAgent, NULL, &VirtioNet::StartTxPolling, (void*) this)) 
    {
        printf("Error creating tx polling agent for eventfd.\n");
        pthread_exit(NULL);
    }
    if (pthread_create(&rxPollingAgent, NULL, &VirtioNet::StartRxPolling, (void*) this)) 
    {
        printf("Error creating rx polling agent for eventfd.\n");
        pthread_exit(NULL);
    }
}

/** Starts polling for eventfd file of Transfer queue
  * @param client: pointer to this device
  * 
  * @retval none
  */
void *VirtioNet::StartTxPolling(void* client)
{
    VirtioNet *device = (VirtioNet*) client;
    VirtQueue *queue = device->getQueue(TRANSFER_QUEUE);
    queue->EnableGuestPolling();
    
    while(true)
    {
        if(queue->PollGuest() > 0)
            device->GenerateInterrupt();
    }
}

/** Starts polling for event fd file of Receive queue
  * @param client: pointer to this device
  * 
  * @retval none
  */
void *VirtioNet::StartRxPolling(void* client)
{
    VirtioNet *device = (VirtioNet*) client;
    VirtQueue *queue = device->getQueue(RECEIVE_QUEUE);
    queue->EnableGuestPolling();
    
    while(true)
    {
        if(queue->PollGuest() > 0)
            device->GenerateInterrupt();
    }
}