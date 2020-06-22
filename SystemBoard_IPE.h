/* 
 * File:   SystemBoard_IPE.h
 * Author: usamanwar
 *
 * Created on June 17, 2014, 2:32 PM
 */

#ifndef SYSTEMBOARD_IPE_H
#define	SYSTEMBOARD_IPE_H

#include "Processor_IPE.h"
#include "Specifications.h"
#include "CIU.h"
#include "UART.h"
#include "VirtioBlk.h"

#ifdef ENABLE_VNET
#include "Virtionet.h"
#endif

#define UBOOT_LOAD_ADDRESS          0xFFFFFFFFBFC00000
#define KERNEL_LOAD_ADDRESS         0xFFFFFFFFA0800000
#define NPEI_DBG_ADDRESS            0x80011F0000008510
#define NPEI_DBG_DATA               0x0000000014420000
#define Allocated_memory            1024*1024*1024 //In bytes

class SystemBoard_IPE {
public:
    SystemBoard_IPE(Specifications*);   
    virtual ~SystemBoard_IPE();
private:
    void initializeSystemMemory();
    void loadFirmware();
    void initializeMemorySegments();
    void loadfile(const char* filename,uint64_t loadAddr);
    uint64_t inline getSize (const char*);
    static void *startCore(void *core);
    static void *startCIU(void *CIU);
    static void *startUART(void *UART);
    static void *startDisk(void *disk);
#ifdef ENABLE_VNET
    static void *startNetCard(void *netcard);
#endif
private:
    Specifications* specs;
    Processor_IPE*  Processors;
    CIU *CIUnit;
    UART *UARTunit;
    VirtioBlk *Disk;
    
#ifdef ENABLE_VNET
    VirtioNet *NetCard;
#endif
};

#endif	/* SYSTEMBOARD_IPE_H */

