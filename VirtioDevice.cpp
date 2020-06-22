#include "VirtioDevice.h"
#include <pthread.h>

/** Constructor for VirtioDevice
  * @param none
  * 
  * @retval none
  */
VirtioDevice::VirtioDevice()
{
}

/** Destructor for VirtioDevice
  * @param none
  * 
  * @retval none
  */
VirtioDevice::~VirtioDevice()
{
    
}

/** Initializer for VirtioDevice
  * Memory map device register
  * Set device header
  * Initialize queue structures
  * @param id: Device id for this device
  * 
  * @retval none
  */
void VirtioDevice::Initialize(uint32_t id)
{
    uint64_t Addr = this->BaseAddr;
    this->DeviceStarted = false;
    if(VIRTIODEVICE_DBG) printf("%s\n",__func__);
    this->Header = (VdevHeader *) Addr;
    this->HostFeatures = (VdevFeatures *)(Addr + VIRTIO_MMIO_HOST_FEATURES);
    this->GuestFeatures = (VdevFeatures *)(Addr + VIRTIO_MMIO_GUEST_FEATURES);
    this->SelectedQueue = (VdevSelectedQueue *)(Addr + VIRTIO_MMIO_QUEUE_SEL);
    this->QueueNotify = (uint32_t *)(Addr + VIRTIO_MMIO_QUEUE_NOTIFY);
    this->InterruptStatus = (uint32_t *)(Addr + VIRTIO_MMIO_INTERRUPT_STATUS);
    this->InterruptACK = (uint32_t *)(Addr + VIRTIO_MMIO_INTERRUPT_ACK);
    this->Status = (uint32_t *)(Addr + VIRTIO_MMIO_STATUS);
    
    this->SetHeader(id);
    this->InitializeQueues();
}

/** Starts VirtioDevice. This function is invoked when driver sets everything and writes VIRTIO_CONFIG_S_DRIVER_OK
  * to status register
  * @param none
  * 
  * @retval none
  */
void VirtioDevice::StartDevice()
{
    if(VIRTIODEVICE_DBG) printf("%s\n",__func__);
    
    this->HostClient.setQueues(this->Queues, this->NumQueues);
    this->InitVirtQueues();
    this->HostClient.updateMemoryTable();
    
    this->HostClient.setBackend(this->Backendfd, this->NumQueues);
    this->DeviceStarted = true;
}

/** Sets header of Virtio device
  * This header is used by driver for device detection
  * @param id: Device id for this device
  * 
  * @retval none
  */
void VirtioDevice::SetHeader(uint32_t id)
{
    VdevHeader *header = this->Header;
    if(VIRTIODEVICE_DBG) printf("%s\n",__func__);
    header->MagicValue = swap_endianess32(VDEV_MAGIC_VALUE);
    header->Version = swap_endianess32(VDEV_VERSION);
    header->DeviceID = swap_endianess32(id);
    header->VendorID = swap_endianess32(VDEV_VENDOR_ID);
}

/** Sets max. value for descriptors for each queue and loads default queue to device registers
  * @param none
  * 
  * @retval none
  */
void VirtioDevice::InitializeQueues()
{
    if(VIRTIODEVICE_DBG) printf("%s\n",__func__);
    this->SavedQueues[RECEIVE_QUEUE].QueueNumMax = swap_endianess32(RX_QUEUE_MAX_SIZE);
    this->SavedQueues[TRANSFER_QUEUE].QueueNumMax = swap_endianess32(TX_QUEUE_MAX_SIZE);
    this->SavedQueues[CONTROL_QUEUE].QueueNumMax = swap_endianess32(CONTROL_QUEUE_MAX_SIZE);
    this->LoadQueue(RECEIVE_QUEUE); //Load Default Queue data in device registers
}

/** Invoked when VIRTIO_MMIO_HOST_FEATURES_SEL register is manipulated and load feature flags to features register
  * @param data: Value which is written to register
  * 
  * @retval none
  */
void VirtioDevice::SelHostFeatures(int data)
{
    VdevFeatures *features = this->HostFeatures;
    features->Features = (swap_endianess32(data) == 0)\
            ? swap_endianess32((uint32_t)this->BackendHostFeatures) \
            : swap_endianess32((uint32_t)(this->BackendHostFeatures >> 32));
}

/** Invoked when VIRTIO_MMIO_GUEST_FEATURES register is manipulated and sets features at backend
  * @param data: Value which is written to register
  * 
  * @retval none
  */
void VirtioDevice::SetGuestFeatures(int data)
{
    VdevFeatures *features = this->GuestFeatures;
    uint64_t ackfeatures = (uint64_t)swap_endianess32(data);
    this->AckGuestFeatures = (features->FeaturesSel == 0)\
            ?((this->AckGuestFeatures & ~0xFFFFFFFFLL) | ackfeatures) \
            : ((this->AckGuestFeatures & 0xFFFFFFFFLL) | (ackfeatures << 32));
}

/** Invoked when VIRTIO_MMIO_QUEUE_SEL register is manipulated
  * Saves the previously loaded queue back to memory and loads selected queue from memory to device registers
  * @param index: Index of selected queue
  * 
  * @retval none
  */
void VirtioDevice::UpdateQueue(int index)
{
    int idx = swap_endianess32(index);
    if(VIRTIODEVICE_DBG) printf("%s\tindex = %d\n",__func__,idx);
    this->SaveQueue(this->SelectedIndex);
    this->LoadQueue(idx);
    this->SelectedIndex = idx;
}


/** Saves queue from device to memory
  * @param index: Index of queue to be saved
  * 
  * @retval none
  */
void VirtioDevice::SaveQueue(int index)
{
    VdevQueueInfo *devinfo = &(this->SelectedQueue->Info);
    VdevQueueInfo *queuedata = &this->SavedQueues[index];
    memcpy(queuedata, devinfo, sizeof(VdevQueueInfo));
}

/** Loads queue from memory to device
  * @param index: Index of queue to be loaded
  * 
  * @retval none
  */
void VirtioDevice::LoadQueue(int index)
{
    VdevQueueInfo *devinfo = &(this->SelectedQueue->Info);
    VdevQueueInfo *queuedata = &this->SavedQueues[index];
    memcpy(devinfo, queuedata, sizeof(VdevQueueInfo));
}

/** Get specific queue from device
  * @param index: Index of queue
  * 
  * @retval pointer to requested queue
  */
VirtQueue *VirtioDevice::getQueue(uint32_t idx)
{
    return &this->Queues[idx];
}

/** Invoked when VIRTIO_MMIO_QUEUE_PFN register is manipulated
  * Gets provided data for selected queue and creates vring for it
  * @param data: Data written to register
  * 
  * @retval none
  */
void VirtioDevice::UpdateVirtQueue(int data)
{
    VdevQueueInfo *queue = &this->SelectedQueue->Info;
    uint32_t num = swap_endianess32(queue->QueueNum);
    uint32_t pfn = swap_endianess32(data);
    uint32_t align = swap_endianess32(queue->QueueAlign);
    if(VIRTIODEVICE_DBG) printf("%s\n",__func__);
    this->Queues[this->SelectedIndex].createVring(num, pfn, align);
}

/** Shares queues with vhost
  * @param none
  * 
  * @retval none
  */
void VirtioDevice::InitVirtQueues()
{
    if(VIRTIODEVICE_DBG) printf("%s\n",__func__);
    for(int i = 0; i < this->NumQueues; i++)
        this->HostClient.InitVring(i);
}

/** Invoked when VIRTIO_MMIO_QUEUE_NOTIFY register is manipulated
  * Notifies host i.e. kick vhost
  * @param data: value written to register
  * 
  * @retval none
  */
void VirtioDevice::NotifyDevice(int data)
{
    if(VIRTIODEVICE_DBG) printf("%s\n",__func__);
    if(!this->DeviceStarted)
        return;
    int idx = swap_endianess32(data);
    this->HostClient.updateMemoryTable();
    this->Queues[idx].NotifyHost(idx);
}