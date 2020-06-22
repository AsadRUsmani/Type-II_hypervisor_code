#include "VirtQueue.h"
#include "GPAtoHVATranslator_IPE.h"

/** Constructor of VirtQueue
  * This class have data for virt queues  like vring, event fd for kick and call and number of descriptors in ring
  * @param none
  * 
  * @retval none
  */
VirtQueue::VirtQueue()
{
    
}

/** Creates vring for queue
  * @param num: number of descriptors in queue
  * @param page: pointer to vring provided by driver
  * @param align: Alignment value for vring
  * 
  * @retval none
  */
void VirtQueue::createVring(uint32_t num, uint32_t page, uint32_t align)
{
    this->GPA = (uint64_t)page << PAGE_SHIFT;
    this->HVA = GPAtoHVATranslator_IPE::GPA_to_HVA(this->GPA);
    this->numDesc = num;
    vring_init(&this->Vring, (unsigned int)num, (void*) this->HVA, (unsigned long)align);
    this->last_avail_idx = 0;
}

/** Give the number of descriptor in the queue
  * @param none
  * 
  * @retval number of descriptors in queue
  */
uint64_t VirtQueue::getNumberofDescriptors()
{
    return this->numDesc;
}

/** Gives the Vring structure of queue
  * @param none
  * 
  * @retval Vring structure of queue
  */
vring* VirtQueue::getVring()
{
    return &this->Vring;
}

/** Gives last availed index of queue
  * @param none
  * 
  * @retval last availed index of queue
  */
uint16_t VirtQueue::getLastAvailIdx()
{
    return this->last_avail_idx;
}

/** Sets last availed index of queue
  * @param idx: Current availed index
  * 
  * @retval none
  */
void VirtQueue::setLastAvailIdx(uint16_t idx)
{
    this->last_avail_idx = idx;
}

/** Increments last availed index of queue
  * @param none
  * 
  * @retval none
  */
void VirtQueue::incLastAvailIdx()
{
    this->last_avail_idx++;
}

/** Gives file descriptor of host notifier for this queue which is used to kick vhost
  * @param none
  * 
  * @retval file descriptor of host notifier i.e. kick
  */
int VirtQueue::getHostNotifierFd()
{
    return this->HostNotifier.getfd();
}

/** Gives file descriptor of guest notifier for this queue which is used to receive call from vhost
  * @param none
  * 
  * @retval file descriptor of guest notifier i.e. call
  */
int VirtQueue::getGuestNotifierFd()
{
    return this->GuestNotifier.getfd();
}

/** Notifies host using event fd i.e. kicks vhost
  * @param value: Value to be written to event fd file
  * 
  * @retval none
  */
void VirtQueue::NotifyHost(long int value)
{
    this->HostNotifier.write(value);
}

/** Enable polling for guest notifier file i.e. polling of call from vhost
  * @param none
  * 
  * @retval none
  */
void VirtQueue::EnableGuestPolling()
{
    this->GuestNotifier.enablePolling();
}

/** Starts polling of guest Notifier
  * @param none
  * 
  * @retval value written to call event fd file
  */
int VirtQueue::PollGuest()
{
    return this->GuestNotifier.poll();
}