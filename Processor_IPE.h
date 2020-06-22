/* 
 * File:   Processor_IPE.h
 * Author: asadusmani
 *
 * Created on June 5, 2014, 11:35 AM
 */

#ifndef PROCESSOR_IPE_H
#define	PROCESSOR_IPE_H

#include "Common.h"
#include "GPR_IPE.h"
#include "CP0_IPE.h"
#include "TLB_IPE.h"
#include "InstBlock.h"
#include "TranslatedBlockCache_IPE.h"
#include "AddrTranslationCache.h"
#include "IOManager.h"

#define DBG_BLOCK           0

#define MIPS_REST_VEC       0XFFFFFFFFBFC00000
#define SEC_CORE_INI_VEC    0xFFFFFFFFBFC00E00

#define TERMINATION_INS     0xEC000000

#define END_TRAP_COUNT            600000000

#define MAX_EXEC_BLOCK_SIZE        1024 * 3//1024  * 64

#define PROG_COUNTER_OFFSET     0     //Address for next block
#define LLBIT_OFFSET            32
#define LL_DATA_OFFSET          40
#define TRANS_ADDR_OFFSET       48
#define LDST_ADDR_OFFSET        56
#define LDST_DATA_OFFSET        64
#define TEMP_REG1_OFFSET        72
#define TEMP_REG2_OFFSET        80
#define C_FUNC_CALL_OFFSET      88
#define CONTROL_FLAG_OFFSET     104
#define CONTEXT_STAT_OFFSET     120
#define LDST_OP_OFFSET          168     //Address for load store
#define BLOCK_START_PC_OFFSET   176
#define BLOCK_END_PC_OFFSET     184
#define GP_REGS_OFFSET          200
#define CP0_REGS_OFFSET         520

#define CHECK_TLB_DATA          0
#define FETCH_NEXT_BLOCK        1
#define HANLDE_SYSCALL_INT      2
#define HANLDE_TLB_PROBE        3
#define HANLDE_TLB_READ         4
#define HANLDE_TLB_WRITEI       5
#define HANLDE_TLB_WRITE        6
#define HANDLE_ASID_CHANGE      7
#define HANLDE_EXTERNAL_INT     8
#define HANDLE_Compare_Write    9
#define HANDLE_count_write      10
#define WRITE_LOG_FILES         11
#define HANLDE_BR               12
#define HANLDE_TRAP_INT         13
#define EXP_RI                  14
#define HANDLE_count_read       15


#define DELAYSLOTINS            2

class Logger;
class Processor_IPE;
typedef bool (Processor_IPE::*HANDLE_REQ)();
class timer;

class Processor_IPE {
public:
    Processor_IPE();
    virtual ~Processor_IPE();
    
    
    uint64_t getTrapCount();
    uint64_t handleSigFPE();
    void setNMI(); // This method is called by CIU to send NMI pulse
    void setTimerTick();
    void execute();
    void setInterupt();
    
private:
    void C2Assembly2C();
    
    inline void incrementTrapCount();
//  inline bool handleRequest();
    inline bool handleLDST();
    inline bool NewBlock();
    inline bool Syscall();
    inline bool TlbProbe();
    inline bool TlbRead();
    inline bool TlbWriteI();
    inline bool TlbWrite();
    inline bool HandleASIDchange();
    inline bool ExternalINT();
    inline bool CompareWrite();
    inline bool CountWrite();
    inline bool Handle_BR();
    inline bool Handle_Trap();
    inline bool Handle_RI();
    inline bool Log();
    void fetchNextBlock();
    inline void fixBlock(std::vector<IInfo>*, uint64_t blockStartAddress, uint64_t blockEndAddr);
    inline bool addrTranslation(uint64_t GVA, uint64_t *HVA);   
    inline void handleNMI();
    inline void handlewait();
    inline void executePendingInterrrupts();
    inline uint64_t current_time();
    
private:
    GPR_IPE  *gpr;
    CP0_IPE  *cp0;
    TLB_IPE  *tlb;
    volatile uint64_t *pc;
    
    timer *TIMER;
    
    uint64_t last_intrpt_time;
    char guestData[MAX_EXEC_BLOCK_SIZE];
    HANDLE_REQ cases[15];
    
    volatile uint64_t *controlMark;
    volatile uint64_t *currentBlockStartAddr;
    volatile uint64_t *currentBlockEndAddr;
    volatile uint64_t *Assembly2CAddress;
    volatile uint64_t *tlbData;
    volatile uint64_t *transAddr;
    volatile uint64_t *LDST;
    volatile uint64_t *uartData;
    volatile uint64_t *LLBit;
    
    uint64_t trapCount;
    InstBlock instBlk;
    TranslatedBlockCache_IPE cacheBlk;
    AddrTranslationCache addrCache;
    uint64_t base;
    uint64_t retAddr;
    uint64_t prevRetAddr;
    
    uint64_t controlMark1;
    
    Context_IPE *context;
    Logger *logGenerator;
    IOManager* iomanager;
    
    volatile bool NMI;
    bool flag; //Used in case of debugging
    uint8_t Asid;
    
    
    uint64_t CVMSEGBase;
    char CVMSEG[8192];
    
public: //In-line public functions
    
    CP0_IPE* getCP0() {
        return (this->cp0);
    }

    GPR_IPE* getGPR() {
        return (this->gpr);
    }

    TLB_IPE* getTLB() {
        return (this->tlb);
    }

    uint64_t getPC()
    {
        return *(this->pc);
    }
};

#endif	/* PROCESSOR_IPE_H */

