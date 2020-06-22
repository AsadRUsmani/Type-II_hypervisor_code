#include "TapInterface.h"
#include <net/if.h>
#include <linux/if_tun.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


#define NETWORK_SCRIPT  "./networkup"

/** Constructor of TapInterface
  * Opens Tap file and makes required IOCTLs
  * @param none
  * 
  * @retval none
  */
TapInterface::TapInterface()
{
    this->Initialize();
}

/** Destructor of TapInterface
  * Closes Tap file
  * @param none
  * 
  * @retval none
  */
TapInterface::~TapInterface()
{
    close(this->fd);
}

/** Initializes TapInterface
  * @param none
  * 
  * @retval none
  */
int TapInterface::Initialize()
{
    this->TapOpen();
    this->TapLaunchScript();
    this->TapSetSendBuff(INT_MAX);
}

/** Opens Tap interface, assigns name to it and make required IOCTLs to create TAP device
  * @param none
  * 
  * @retval 0 on success and -1 on failure
  */
int TapInterface::TapOpen()
{
    struct ifreq ifr;
    unsigned int features;
    int fd;
    if( (fd = open(TUN_PATH, O_RDWR)) < 0 )
    {
        printf("Error: Unable to open %s\n",TUN_PATH);
        return -1;
    }
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    
    //QEMU uses this IOCTLS call to set header size for TAP but our TUN module don't support this
    if (ioctl(fd, TUNGETFEATURES, &features) == 0 && features & IFF_VNET_HDR)
    {
        this->VnetHdr = true;
        ifr.ifr_flags |= IFF_VNET_HDR; //Need to check this
    } 
    else
    {
        this->VnetHdr = false;
    }
    
    strncpy(ifr.ifr_name, TAP_DEVICE_NAME, IFNAMSIZ);
    
    if (ioctl(fd, TUNSETIFF, (void *) &ifr) != 0) {
        printf("Error: Cannot configure %s\n",TUN_PATH);
        close(fd);
        return -1;
    }
    
    strncpy(this->DeviceName, ifr.ifr_name, IFNAMSIZ);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    this->fd = fd;
    return 0;
}

/** Calls the script to create bridge and add TAP and physical ether net card to this bridge
  * @param none
  * 
  * @retval 0 on success and -1 on failure
  */
int TapInterface::TapLaunchScript()
{
    int pid;
    pid = fork();
    if(pid == 0)
    {
//        char const *args[] = {NETWORK_SCRIPT, TAP_DEVICE_NAME, NULL};
//        char **parg = (char**)args;
        char *script = (char*)NETWORK_SCRIPT;
        char *parg[3];
        parg[0] = script;
        parg[1] = this->DeviceName;
        parg[2] = NULL;
        execv(parg[0], parg);
        printf("Script launching failed\n");
        _exit(1);
    }
    else
    {
        while (waitpid(pid, NULL, 0) != pid) {
            /* loop */
        }
    }
    printf("%s completed\n",__func__);
}

/** Set offloads for TAP interface
  * @param none
  * 
  * @retval 0 on success and -1 on failure
  */
int TapInterface::TapSetOffload(uint64_t features)
{
    uint64_t offload = 0;
    if((features >> VIRTIO_NET_F_GUEST_CSUM) & 1)
    {
        offload |= TUN_F_CSUM;
        if((features >> VIRTIO_NET_F_GUEST_TSO4) & 1)
            offload |= TUN_F_TSO4;
        if((features >> VIRTIO_NET_F_GUEST_TSO6) & 1)
            offload |= TUN_F_TSO6;
        if((features >> VIRTIO_NET_F_GUEST_ECN)  & 1)
            offload |= TUN_F_TSO_ECN;
        if((features >> VIRTIO_NET_F_GUEST_UFO)  & 1)
            offload |= TUN_F_UFO;
    }
        
//    offload = (TUN_F_CSUM | TUN_F_TSO4 | TUN_F_TSO6 | TUN_F_TSO_ECN);
    //CSUM, GSO etc related to Virtio-net
    if (ioctl(this->fd, TUNSETOFFLOAD, offload) != 0) {
        if(errno == EINVAL)
            printf("TUNSETOFFLOAD not supported\n");
        else
            printf("Error: TUNSETOFFLOAD ioctl() failed %s\n",TUN_PATH);
        return -1;
    }
    return 0;
}

/** Make IOCTL calls to set size for TAP send buffer
  * @param size: desired size for TAP send buffer
  * 
  * @retval 0 on success and -1 on failure
  */
int TapInterface::TapSetSendBuff(int size)
{
    if (ioctl(this->fd, TUNSETSNDBUF, (void *) &size) != 0) {
        printf("Error: TUNSETSNDBUF ioctl() failed %s\n",TUN_PATH);
        return -1;
    }
    return 0;
}

/** Gives file descriptor of TAP interface
  * @param none
  * 
  * @retval fd of TAP interface
  */
int TapInterface::getTapfd()
{
    return this->fd;
}

/** Gives header length for TAP interface
  * Not used at this time
  * @param none
  * 
  * @retval header size of TAP interface
  */
int TapInterface::getHeaderLen()
{
    return this->HeaderLen;
}

/** Gives header length support status
  * @param none
  * 
  * @retval true if header length is set and false if TAP using default header length
  */
bool TapInterface::IsVnetHdrSupport()
{
    return this->VnetHdr;
}

/** Sets header length for TAP interface
  * Not used at this time
  * @param len: length of header for TAP
  * 
  * @retval none
  */
void TapInterface::setHeaderLen(int len)
{
    this->HeaderLen = len;
}
