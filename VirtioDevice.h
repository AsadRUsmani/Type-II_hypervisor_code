/* 
 * File:   VirtioDevice.h
 * Author: usamanwar
 *
 * Created on 02 July 2015, 11:03
 */

#ifndef VIRTIODEVICE_H
#define	VIRTIODEVICE_H

#include <linux/byteorder/big_endian.h>
#include <linux/virtio_config.h>
#include "VirtioHeaders/virtio_mmio.h"
#include "VirtQueue.h"
#include "Vhost.h"
#include "CIU.h"
#include "IOManager.h"

/*#define __force __attribute__((force))
#define swap_endianess(x) ((__force __le32)__swab32((x)))*/
#define swap_endianess32(x) __cpu_to_le32(x)
#define swap_endianess16(x) __cpu_to_le16(x)
#define __packed __attribute__((packed))

#define NETWORK_CARD_ID     01
#define BLOCK_DEVICE_ID     02
#define CONSOLE_ID          03

#define VDEV_MAGIC_VALUE    0x76697274
#define VDEV_VENDOR_ID      0x1AF4
#define VDEV_VERSION        0x01

#define RECEIVE_QUEUE       00
#define TRANSFER_QUEUE      01
#define CONTROL_QUEUE       02

#define TX_QUEUE_MAX_SIZE           0x0100
#define RX_QUEUE_MAX_SIZE           0x0100
#define CONTROL_QUEUE_MAX_SIZE      0x0100

#define VIRTIODEVICE_DBG            0

typedef struct {
    uint32_t MagicValue;
    uint32_t Version;
    uint32_t DeviceID;
    uint32_t VendorID;
}VdevHeader;

typedef struct {
    uint32_t Features;
    uint32_t FeaturesSel;
    uint32_t Pagesize;
}VdevFeatures;

typedef struct {
    uint32_t QueueNumMax;
    uint32_t QueueNum;
    uint32_t QueueAlign;
    uint32_t QueuePFN;
}VdevQueueInfo;

typedef struct {
    uint32_t QueueSel;
    VdevQueueInfo Info;
}VdevSelectedQueue;

class VirtioDevice
{
public:
    VirtioDevice();
    ~VirtioDevice();
    
protected:
    void StartDevice();
    void Initialize(uint32_t id);
    void SelHostFeatures(int);
    void SetGuestFeatures(int);
    void UpdateQueue(int index);
    void UpdateVirtQueue(int);
    void NotifyDevice(int);
    VirtQueue *getQueue(uint32_t idx);
    
private:
    void inline SetHeader(uint32_t id);
    void inline SaveQueue(int index);
    void inline LoadQueue(int index);
    void inline InitializeQueues(void);
    void inline InitVirtQueues();
    
protected:
    uint64_t BaseAddr;
    DeviceMessageBox *MessageBox;
    CIU *CIUunit;
    uint32_t NumQueues;
    uint64_t AckGuestFeatures;
    uint64_t BackendHostFeatures;
    int Backendfd;
    uint32_t *InterruptStatus;
    Vhost HostClient;
    bool DeviceStarted;
    
private:
    
    VdevHeader *Header;
    VdevFeatures *HostFeatures;
    VdevFeatures *GuestFeatures;
    VdevSelectedQueue *SelectedQueue;
    uint32_t *QueueNotify;
    uint32_t *InterruptACK;
    uint32_t *Status;
    VdevQueueInfo SavedQueues[3];
    VirtQueue Queues[3];
    uint32_t SelectedIndex;
};

#endif	/* VIRTIODEVICE_H */

