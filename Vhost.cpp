#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "Vhost.h"
#include "VirtioHeaders/vhost.h"
#include "GPAtoHVATranslator_IPE.h"

/** Constructor of Vhost
  * This class is used to perform all vhost related operations
  * @param none
  * 
  * @retval none
  */
Vhost::Vhost()
{
    
}

/** Destructor of Vhost
  * Unmaps memory assigned to memory table and also closes vhost file
  * @param none
  * 
  * @retval none
  */
Vhost::~Vhost()
{
    munmap(this->memoryTable,offsetof(struct vhost_memory, regions) + sizeof (struct vhost_memory_region) * 64);
    close(this->Vhostfd);
}

/** Opens Vhost file and make some basic IOCTL calls to initialize it
  * @param none
  * 
  * @retval 0 on success, -1 on failure
  */
int Vhost::Initialize(const char *path)
{
    int fd, r;
    if(VHOST_DBG) printf("Vhost: %s\n",__func__);
    if( (fd = open(path, O_RDWR)) < 0 )
    {
        printf("Error: Unable to open %s\n",path);
        return -1;
    }
    
    if(r = ioctl(fd, VHOST_SET_OWNER, NULL) < 0)
    {
        printf("Error: Unable to set owner %s\n",path);
        return -1;
    }
    
    this->memoryTable = (struct vhost_memory*)GPAtoHVATranslator_IPE::allocMemRegion(offsetof(struct vhost_memory, regions) + sizeof (struct vhost_memory_region) * 64);
    memset(this->memoryTable, '\0', offsetof(struct vhost_memory, regions));
    this->Vhostfd = fd;
    this->memTableIdx = 0;
    this->isNet = !!strstr(path, "net");    //True if file path contains net
    
    return 0;
}

/** Sets backend for Vhost
  * VHost thread is created once backend is set successfully
  * @param fd: file descriptor of backend
  * @param num: total number of virtqueue 
  * 
  * @retval 0 on success, -1 on failure
  */
int Vhost::setBackend(int fd, int num)
{
    struct vhost_vring_file file = { };
    if(VHOST_DBG) printf("%s\n",__func__);
    file.fd = fd;
    for (file.index = 0; file.index < num; ++file.index) {
        if (ioctl(this->Vhostfd, VHOST_NET_SET_BACKEND, &file) < 0){
            printf("Error: Setting backend. Errno = %d",errno);
            return -1;
        }
    }
    return 0;
}

/** Sets memory table for vhost
  * @param none
  * 
  * @retval 0 on success, error no. on failure
  */
int Vhost::setMemoryTable()
{
    if(VHOST_DBG) printf("%s\n",__func__);
    return ioctl(this->Vhostfd, VHOST_SET_MEM_TABLE, this->memoryTable);
}

/** Puts address translation for specified descriptor in memory table
  * @param desc: Descriptor for which address translation is required
  * 
  * @retval none
  */
void Vhost::updateTableEntry(vring_desc* desc)
{
    struct vhost_memory_region *reg;
    
    reg = this->memoryTable->regions + (this->memTableIdx++ % VHOST_MEMORY_MAX_NREGIONS);
    reg->guest_phys_addr = desc->addr;
    reg->userspace_addr = GPAtoHVATranslator_IPE::GPA_to_HVA(desc->addr);
    reg->memory_size = desc->len;

    if(desc->flags & VRING_DESC_F_INDIRECT)
    {
        if(VHOST_DBG) printf("%s: Indirect ring detected.\n",__func__);
        this->updateTableIndirect((vring_desc*) reg->userspace_addr, desc->len);
    }
}

/** Updates the net related memory table for specified queue
  * @param queue: Virtqueue for which memory table should be updated.
  * 
  * @retval number of entries added in table
  */
int Vhost::updateNetTableFromQueue(VirtQueue *queue)
{
    vring *ring = queue->getVring();
    uint16_t last_avail_idx = queue->getLastAvailIdx();
    struct vring_desc *desc;
    unsigned int i, found = 0;
    int ret = 0;
    
    if(VHOST_DBG) printf("%s: avail_idx = %d\tdescriptors = %d\n",__func__,ring->avail->idx,queue->getNumberofDescriptors());
    
    i = ring->avail->ring[last_avail_idx % queue->getNumberofDescriptors()];
    this->memTableIdx = this->memoryTable->nregions;
    
    do
    {
        if (i >= queue->getNumberofDescriptors()) {
    		printf("%s: Desc index is %u > %u\n",__func__,i, queue->getNumberofDescriptors());
    		return ret;
    	}
        if (++found >= VHOST_MEMORY_MAX_NREGIONS) {
    		printf("%s: Guest provided descriptors > %u\n",__func__,VHOST_MEMORY_MAX_NREGIONS);
    		return ret;
    	}
        
        if(VHOST_DBG) printf("%s:head = %d\tindex = %d\t",__func__, i, this->memTableIdx);
        desc = ring->desc + i;
        this->updateTableEntry(desc);
        ret++;
    } while ((i = nextDesc(desc)) != -1);
    
    queue->incLastAvailIdx();
    return ret;
}

/** Updates the blk related memory table for specified queue
  * @param queue: Virtqueue for which memory table should be updated.
  * 
  * @retval number of entries added in table
  */
int Vhost::updateBlkTableFromQueue(VirtQueue *queue)
{
    vring *ring = queue->getVring();
    uint16_t last_avail_idx;
    struct vring_desc *desc;
    unsigned int i, head, found = 0;
    int ret = 0;
    
    /* Grab the next descriptor number they're advertising, and increment
     * the index we've seen. */
    if(VHOST_DBG) printf("%s\n",__func__);
    while((last_avail_idx = queue->getLastAvailIdx()) != ring->avail->idx)
    {
        head = ring->avail->ring[last_avail_idx % queue->getNumberofDescriptors()];
        if(VHOST_DBG) printf("%s: Avail_idx = %d\tlast_avail_idx = %d\thead = %d\n",__func__,ring->avail->idx,last_avail_idx,head);

        i = head;
        do {
            if (i >= queue->getNumberofDescriptors()) {
                    printf("%s: Desc index is %u > %u\n",__func__,i, queue->getNumberofDescriptors());
                    return ret;
            }
            
            if (++found >= VHOST_MEMORY_MAX_NREGIONS) {
                    printf("%s: Guest provided descriptors > %u\n",__func__,VHOST_MEMORY_MAX_NREGIONS);
                    return ret;
            }

            if(VHOST_DBG) printf("%s:head = %d\tindex = %d\t",__func__, i, this->memTableIdx);
            desc = ring->desc + i;
            this->updateTableEntry(desc);
            
        } while ((i = nextDesc(desc)) != -1);

        /* On success, increment avail index. */
        queue->incLastAvailIdx();
        ret++;
    }
    
    return ret;
}

/** Provides translation in case of indirect descriptors
  * @param indirect: Pointer to indirect descriptor.
  * @param len: Length of descriptor in bytes.
  * 
  * @retval none
  */
void Vhost::updateTableIndirect(vring_desc* indirect, int len)
{
    struct vring_desc *desc = indirect;
    unsigned int i = 0, count, found = 0;
    struct vhost_memory_region *reg;
    count = len / sizeof(struct vring_desc);
    
    do {
    	if (++found > count) {
    		printf("%s: Loop detected: last one at %u indirect size %u\n",__func__,i, count);
			return;
	}
        desc += i;
        reg = this->memoryTable->regions + (this->memTableIdx++ % VHOST_MEMORY_MAX_NREGIONS);
        reg->guest_phys_addr = desc->addr;
        reg->userspace_addr = GPAtoHVATranslator_IPE::GPA_to_HVA(desc->addr);
        reg->memory_size = desc->len;
    	if (desc->flags & VRING_DESC_F_INDIRECT) {
    		printf("%s: Nested indirect descriptor: idx %d\n",__func__,i);
			return;
    	}
        if(VHOST_DBG) printf("%s:Adding indirect addr = 0x%lx\tlen = %d\n",__func__, desc->addr, desc->len);
    } while ((i = nextDesc(desc)) != -1);
}

/** Provides index of next descriptor in case of chained descriptors
  * @param desc: Pointer to current descriptor.
  * 
  * @retval index of next descriptor in chain if present otherwise -1
  */
int Vhost::nextDesc(vring_desc* desc)
{
    /* If this descriptor says it doesn't chain, we're done. */
    if (!(desc->flags & VRING_DESC_F_NEXT))
    	return -1;

    return desc->next;
}

/** Updates the memory table for queues
  * @param rcvqueue: Receive queue to update memory table for descriptors in receive queue
  * @param sndqueue: Transfer queue to update memory table for descriptors in transfer queue
  * 
  * @retval none
  */
void Vhost::updateMemoryTable()
{
    int ret = 0;
    if(VHOST_DBG) printf("%s\n",__func__);
    
    if(this->isNet)
    {
        for(int i = 0; i < this->numQueues; i++)
            ret += this->updateNetTableFromQueue(this->queues + i);
    }
    else
        ret += this->updateBlkTableFromQueue(this->queues);
    
    if(ret > 0){
        if(this->memTableIdx > VHOST_MEMORY_MAX_NREGIONS)
            this->memoryTable->nregions = VHOST_MEMORY_MAX_NREGIONS;
        else
            this->memoryTable->nregions = this->memTableIdx;
        
        if(this->setMemoryTable() < 0)
            printf("%s: Error setting memory table. Error no = %d\n",__func__,errno);
    }
}

/** Gives supported features of Vhost
  * @param none
  * 
  * @retval vhost features
  */
uint64_t Vhost::getFeatures()
{
    
    if(ioctl(this->Vhostfd, VHOST_GET_FEATURES, &this->hostFeatures) < 0)
    {
        printf("Error: Unable to get features\n");
    }
    return this->hostFeatures;
}

/** Sets requested features for Vhost
  * @param features: provided features for vhost 
  * 
  * @retval none
  */
void Vhost::setFeatures(uint64_t features)
{
    this->guestFeatures = features;
    if(ioctl(this->Vhostfd, VHOST_SET_FEATURES, &this->guestFeatures) < 0)
        printf("Error: Setting features for vhost\n");
}

/** Shares virtqueue with vhost using required IOCTL calls
  * @param queue: pointer to virtqueue
  * @param idx: index of virtqueue
  * 
  * @retval 0 on success, -1 on failure
  */
int Vhost::InitVring(int idx)
{
    int r;
    VirtQueue *queue = this->queues + idx;
    struct vhost_vring_file file;
    struct vhost_vring_state state;
    struct vring *ring = queue->getVring();
    file.index = idx;
    state.index = idx;
    state.num = ring->num;
    if(VHOST_DBG) printf("%s %d\n",__func__,idx);
    r = ioctl(this->Vhostfd, VHOST_SET_VRING_NUM, &state);
    if (r) {
        printf("Error: VHOST_SET_VRING_NUM. Errno = %d\n",errno);
        return -1;
    }
    
    state.num = 0;
    r = ioctl(this->Vhostfd, VHOST_SET_VRING_BASE, &state);
    if (r) {
        printf("Error: VHOST_SET_VRING_BASE\n");
        return -1;
    }
    
    this->setVringAddr(ring, idx);
    
    file.fd = queue->getHostNotifierFd();
    r = ioctl(this->Vhostfd, VHOST_SET_VRING_KICK, &file);
    if (r) {
        printf("Error: VHOST_SET_VRING_KICK\n");
        return -1;
    }

    file.fd = queue->getGuestNotifierFd();
    r = ioctl(this->Vhostfd, VHOST_SET_VRING_CALL, &file);
    if (r) {
        printf("Error: VHOST_SET_VRING_CALL\n");
        return -1;
    }
}

/** Shares vring with vhost using required IOCTL calls
  * @param ring: pointer to vring
  * @param idx: index of virtqueue
  * 
  * @retval 0 on success, -1 on failure
  */
int Vhost::setVringAddr(vring *ring, int idx)
{
    struct vhost_vring_addr addr;
    addr.index = idx;
    addr.desc_user_addr = (uint64_t)(unsigned long)ring->desc;
    addr.avail_user_addr = (uint64_t)(unsigned long)ring->avail;
    addr.used_user_addr = (uint64_t)(unsigned long)ring->used;
    addr.flags = 0;
//    addr.log_guest_addr = (uint64_t)(unsigned long)ring->used;
    addr.log_guest_addr = NULL;
    if(VHOST_DBG) printf("%s\tring->desc = 0x%p\tring->avail = 0x%p\tring->used = 0x%p\n",__func__,ring->desc,ring->avail,ring->used);
                
    int r = ioctl(this->Vhostfd, VHOST_SET_VRING_ADDR, &addr);
    if (r < 0) {
        printf("Error: VHOST_SET_VRING_ADDR %d\n",errno);
        return -1;
    }
    return 0;
}

/** Sets total number of queues used by device along with pointer to the queue
  * @param queue: Pointer to the queue
  * @param num: Number of queues for device
  * 
  * @retval none
  */
void Vhost::setQueues(VirtQueue *queue, int num)
{
    this->queues = queue;
    this->numQueues = num;
}