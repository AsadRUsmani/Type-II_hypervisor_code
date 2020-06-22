/* 
 * File:   Virtionet.h
 * Author: usamanwar
 *
 * Created on 02 July 2015, 09:53
 */

#ifndef VIRTIONET_H
#define	VIRTIONET_H

#define VHOST_NET_PATH  "/dev/vhost-net"

#include "VirtioDevice.h"
#include "TapInterface.h"
#include "VirtioHeaders/virtio_net.h"

#define VIRTIONET_DBG   0

class VirtioNet : VirtioDevice{
private:
    TapInterface *tap;
    virtio_net_config *config;
private:
    void inline vnet_io_read();
    void inline vnet_io_write();
    void inline updateConfig();
    void inline VnetStart();
    
    static void *StartTxPolling(void *client);
    static void *StartRxPolling(void *client);
    
    /*Virtual function from VirtioDevice. Mandatory to implement in each device with device specific functionality*/
    void inline PerformOperation(uint32_t Addr, uint32_t data);
    void inline UpdateStatus(int status);
    void inline AckInterrupt(int);
    void inline UpdateInterrupt();
    void inline GenerateInterrupt();
    void inline InitHostClient();
    void inline GetBackendHostFeatures();
    uint64_t inline GetBackendGuestFeatures();
    void inline InitPollingAgent();    
    
public:
    VirtioNet(CIU*);
    ~VirtioNet();
    void start();
};



#endif	/* VIRTIONET_H */

