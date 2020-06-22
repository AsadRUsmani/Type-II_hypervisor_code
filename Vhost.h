/* 
 * File:   Vhost.h
 * Author: usamanwar
 *
 * Created on 07 July 2015, 10:39
 */

#ifndef VHOST_H
#define	VHOST_H

#include "VirtQueue.h"

#define VHOST_DBG            0
#define VHOST_MEMORY_MAX_NREGIONS   64

class Vhost{
public:
    Vhost();
    ~Vhost();
    int Initialize(const char*);
    int setBackend(int, int);
    int setMemoryTable();
    void updateMemoryTable();
    int InitVring(int);
    uint64_t getFeatures();
    void setFeatures(uint64_t);
    void setQueues(VirtQueue *, int);
    
private:
    int inline setVringAddr(vring *, int);
    void updateTableEntry(struct vring_desc*);
    int inline updateBlkTableFromQueue(VirtQueue *queue);
    int inline updateNetTableFromQueue(VirtQueue *queue);
    void inline updateTableIndirect(struct vring_desc*, int);
    int inline nextDesc(struct vring_desc*);
    
private:
    int Vhostfd;
    int numQueues;
    uint64_t hostFeatures;
    uint64_t guestFeatures;
    struct vhost_memory *memoryTable;
    int memTableIdx;
    bool isNet;
    VirtQueue *queues;
};

#endif	/* VHOST_H */

