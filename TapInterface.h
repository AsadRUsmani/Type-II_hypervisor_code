/* 
 * File:   TapInterface.h
 * Author: usamanwar
 *
 * Created on 11 September 2015, 18:26
 */

#ifndef TAPINTERFACE_H
#define	TAPINTERFACE_H

#include <inttypes.h>
#include "VirtioHeaders/virtio_net.h"

#define TUN_PATH    "/dev/net/tun"
#define TAP_DEVICE_NAME "tun%d"

class TapInterface
{
public:
    TapInterface();
    ~TapInterface();
    int getTapfd();
    int getHeaderLen();
    void setHeaderLen(int);
    int TapSetOffload(uint64_t);
    bool IsVnetHdrSupport();
private:
    int inline Initialize();
    int inline TapOpen();
    int inline TapSetSendBuff(int);
    int inline TapLaunchScript();
private:
    int fd;
    int HeaderLen;
    bool VnetHdr;
    char DeviceName[16];
};

#endif	/* TAPINTERFACE_H */

