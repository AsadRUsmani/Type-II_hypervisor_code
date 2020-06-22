/* 
 * File:   CodeExecutor.h
 * Author: asadusmani
 *
 * Created on June 4, 2014, 4:50 PM
 */

#ifndef CODEEXECUTOR_H
#define	CODEEXECUTOR_H

#include "Common.h"
#include "InstBlock.h"
#include "Context_IPE.h"

#define END_TRAP_COUNT             1000000

#define MIN_TRANS_INSTR_SIZE       2
#define MAX_TRANS_INSTR_SIZE       15000 
#define MAX_EXEC_BLOCK_SIZE        1024  * 64
#define MAX_TRANS_BLOCK_SIZE       15000 * 4

#define PRE_LOG_OFFSET          0
#define LDST_ADDR_OFFSET         56
#define UART_REG_OFFSET         64
#define TEMP_REG1_OFFSET        72
#define TEMP_REG2_OFFSET        80
#define C_FUNC_CALL_OFFSET      88
#define CONTROL_FLAG_OFFSET     104
//#define TLB_DATA_OFFSET         112
#define NEXT_BLOCK_OFFSET       112
#define CONTEXT_STAT_OFFSET     120
#define UART_LDST_OFFSET        168
#define BLOCK_START_PC_OFFSET   176
#define BLOCK_END_PC_OFFSET     184
#define PROG_COUNTER_OFFSET     192
#define GP_REGS_OFFSET          200
#define CP0_REGS_OFFSET         480 
#define EXEC_BLOCK_OFFSET       2600 
#define HANDLER_OFFSET          64000 

#define CHECK_TLB_DATA          0
#define FETCH_NEXT_BLOCK        1
#define WRITE_LOG_FILES         2
#define HANLDE_OTHER_INT        3
#define HANLDE_SYSCALL_INT      4
#define HANLDE_TLB_READ         5
#define HANLDE_TLB_WRITE        6
#define HANLDE_TLB_PROBE        7
#define HANLDE_TLB_WRITEI       8
#define BR                      9


class CodeExecutor {
public:
    CodeExecutor();
    virtual ~CodeExecutor();
    
    void execute();
    
private:
    void C2Assembly2C();
    
    void incrementTrapCount();
    
    bool handleRequest();
    void fetchNextBlock();
    bool handleLDST();
    
    void getControlMark();

private:
    volatile char guestData[MAX_EXEC_BLOCK_SIZE];
    
//    Board_IPE *systemMainBoard;
    
    volatile uint64_t *controlMark;
    volatile uint64_t *GVA;
    volatile uint64_t *currentBlockStartAddr;
    volatile uint64_t *currentBlockEndAddr;
    volatile uint64_t *Assembly2CAddress;
    volatile uint64_t *tlbData;
    volatile uint64_t *LDST;
    volatile uint64_t *uartData;
    
    uint64_t trapCount;
    InstBlock instBlk;
    uint64_t base;
    uint64_t retAddr;
    uint64_t prevRetAddr;
    
    uint64_t controlMark1;
    
    int flag;
    
    Context_IPE context;
};

#endif	/* CODEEXECUTOR_H */

