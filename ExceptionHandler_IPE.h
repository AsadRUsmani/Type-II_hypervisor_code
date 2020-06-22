/* 
 * File:   ExceptionHandler_IPE.h
 * Author: asadusmani
 *
 * Created on June 5, 2014, 12:27 PM
 */

#ifndef EXCEPTIONHANDLER_IPE_H
#define	EXCEPTIONHANDLER_IPE_H

#include "Common.h"
#include "CP0_IPE.h"
#include "Context_IPE.h"

#define INT             0
#define TLB_MOD         1 
#define TLBL            2
#define TLBS            3
#define ADEL            4
#define ADES            5
#define IBE             6
#define DBE             7
#define SCALL           8
#define BRK             9
#define RI              10
#define CPU             11
#define OV              12
#define TRAP            13
#define TLB_INV         14

#define REGION_MASK                         0x03ULL
#define REGION_OFFSET                       62

#define SEG_MASK                    0x1FFFFFFFFFFFF

#define EXCEPTION_VETOR             0xBFC00200
#define TLB_REFILL_OFFSET           0x00
#define XTLB_REFILL_OFFSET          0x80
#define CACHE_ERROR_OFFSET          0x100
#define GENERAL_EXECPTION_OFFSET    0x180
#define INTERRUPT_OFFSET            0x200

class TLB_Exception_IPE{
public:
    static uint64_t Perform_Exception(CP0_IPE * cp0, uint64_t GVA, int ex_co, Context_IPE *context);
    static uint64_t TLB_Excep(CP0_IPE * cp0, uint64_t GVA, uint32_t exec_code, Context_IPE *context);
};

#endif	/* EXCEPTIONHANDLER_IPE_H */

