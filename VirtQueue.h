/* 
 * File:   VirtQueue.h
 * Author: usamanwar
 *
 * Created on 07 July 2015, 11:08
 */

#ifndef VIRTQUEUE_H
#define	VIRTQUEUE_H

#include <inttypes.h>
#include "EventNotifier.h"
#include "VirtioHeaders/virtio_ring.h"

#define PAGE_SHIFT  12

class VirtQueue{
public:
    VirtQueue();
    void createVring(uint32_t num, uint32_t page, uint32_t align);
    uint64_t getNumberofDescriptors();
    vring *getVring();
    uint16_t getLastAvailIdx();
    void setLastAvailIdx(uint16_t);
    void incLastAvailIdx();
    int getHostNotifierFd();
    int getGuestNotifierFd();
    void NotifyHost(long int);
    void EnableGuestPolling();
    int PollGuest();
private:
    uint64_t GPA;
    uint64_t HVA;
    uint32_t numDesc;
    uint16_t last_avail_idx;
    vring Vring;
    EventNotifier HostNotifier;
    EventNotifier GuestNotifier;
};

#endif	/* VIRTQUEUE_H */

