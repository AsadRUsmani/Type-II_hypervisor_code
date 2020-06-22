/* 
 * File:   VirtioBlk.h
 * Author: usamanwar
 *
 * Created on 28 September 2015, 17:06
 */

#ifndef VIRTIOBLK_H
#define	VIRTIOBLK_H

#include "VirtioDevice.h"
#include <linux/virtio_blk.h>

#define VHOST_BLK_PATH  "/dev/vhost-blk"
#define DISK_PATH       "./disk.img"
#define DISK_CAPACITY   128 * 1024 *1024    //128 MB
#define DISK_SECTORS    DISK_CAPACITY / 512
#define REQUEST_QUEUE       00

#define VIRTIOBLK_DBG       0

class VirtioBlk : VirtioDevice {
    
private:
    virtio_blk_config *config;
    
private:
    void inline VblkStart();
    void inline vnet_io_read();
    void inline vnet_io_write();
    void inline InitConfig();
    void inline updateConfig();
    int inline OpenDisk();
    
    static void *StartReqPolling(void *dev);
    
    /*Virtual function from VirtioDevice. Mandatory to implement in each device with device specific functionality*/
    void inline PerformOperation(uint32_t Addr, uint32_t data);
    void inline InitHostClient();
    void inline AckInterrupt(int);
    void inline UpdateStatus(int status);
    void inline UpdateInterrupt();
    void inline GenerateInterrupt();
    void inline InitPollingAgent();
    void inline GetBackendHostFeatures();
    uint64_t inline GetBackendGuestFeatures();

public:
    VirtioBlk(CIU* ciu);
    ~VirtioBlk();
    void start();
};

#endif	/* VIRTIOBLK_H */

