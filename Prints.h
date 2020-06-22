/* 
 * File:   Prints.h
 * Author: asadusmani
 *
 * Created on February 26, 2014, 12:41 PM
 */

#ifndef PRINTS_H
#define	PRINTS_H

#define COMMON_ERROR                    0
#define TRAP_COUNT_REACHED              1
#define FILE_NOTFOUND_ERROR             2
#define CNTRLMARK_NOT_FOUND_ERROR       3
#define CACHE_BLOCK_ERROR               21
#define MAX_TRANS_BLOCK_ERROR           22
#define MIN_TRANS_BLOCK_ERROR           28
#define JUMP_NOTFOUND_ERROR             23
#define HVA_NOTFOUND_ERROR              24
#define CTOASSEM_FUNC_ERROR             25
#define REQUEST_NOTHANDLE_ERROR         26
#define GVA_ADRSTRANS_NOTFOUND_ERROR    27

#include "Common.h"
class P {
public:
    P();
    P(const P& orig);
    virtual ~P();
    static void EPRINT(int);
    static void EPRINTVAL(uint64_t);
private:

};

#endif	/* PRINTS_H */

