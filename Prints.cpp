/* 
 * File:   Prints.cpp
 * Author: asadusmani
 *
 * Created on February 28, 2014, 12:41 PM
 */
#include "P.h"
#include "Common.h"
//#include "BlockExecController.h"
//#include "CodeExecutor.h"
#include "Processor_IPE.h"

P::P() {
}

P::P(const P& orig) {
}

P::~P() {
}
void P::EPRINTVAL(uint64_t val) {
    printf("GVA|Handler not found!=0x%016llx\n",val);
    exit(1);
}
void P::EPRINT(int errorCase) {
    switch(errorCase){
        case COMMON_ERROR:
            printf("COMMON_ERROR!Error Found at the movement..\n");
            break;
        case TRAP_COUNT_REACHED:
            printf("TRAP_COUNT_REACHED! Trap count reached set itself = %ld\n",END_TRAP_COUNT);
            exit(1);
            break;
        case CNTRLMARK_NOT_FOUND_ERROR:
            printf("CNTRLMARK_NOT_FOUND_ERROR!handleRequest():unexpected contol marke found..\n");
            break;
        case CACHE_BLOCK_ERROR:
            printf("CACHE_BLOCK_ERROR!putInCache():Cache related error..\n");
            break;
        case MAX_TRANS_BLOCK_ERROR:
            printf("TRANS_BLOCK_ERROR!doBlockTranslation():MoreMemRequired..\n");
            exit(1);
            break;
        case MIN_TRANS_BLOCK_ERROR:
            printf("TRANS_BLOCK_ERROR!doBlockTranslation():LessMemRequired..\n");
            exit(1);
            break;
        case JUMP_NOTFOUND_ERROR:
            printf("TRANS_BLOCK_ERROR!doBlockTranslation():MoreMemRequired..\n");
            break;
        case HVA_NOTFOUND_ERROR:
            printf("HVA_NOTFOUND_ERROR!check_TLB():HVA not found against GVA..\n");
            break;
        case CTOASSEM_FUNC_ERROR:
            printf("CTOASSEM_FUNC_ERROR!c2Assembly():Jump into Assembly not correctly done..\n");
            break;
        case REQUEST_NOTHANDLE_ERROR:
            printf("REQUEST_NOTHANDLE_ERROR!handleRequest():Request not handled by the handler..\n");
            break;
        case GVA_ADRSTRANS_NOTFOUND_ERROR:
            printf("GVA_ADRSTRANS_NOTFOUND_ERROR!tanlsateAddress():Invalid Address found..\n\n");
            break;
        case FILE_NOTFOUND_ERROR:
            printf("FILE_NOTFOUND_ERROR!writeLogFile():file opening error..\n");
        default:
            printf("Common Error! default() method.\n");
            break;
    }
    exit(1);
}
