/* 
 * File:   GPR_IPE.h
 * Author: asadusmani
 *
 * Created on June 4, 2014, 6:57 PM
 */

#ifndef GPR_IPE_H
#define	GPR_IPE_H

#include "Common.h"

/** MIPS General Purpose Registers */

#define GPR_ZERO        0  
#define GPR_AT          1  
#define GPR_V0          2  
#define GPR_V1          3  
#define GPR_A0          4  
#define GPR_A1          5  
#define GPR_A2          6  
#define GPR_A3          7  
#define GPR_T0          8  
#define GPR_T1          9  
#define GPR_T2          10 
#define GPR_T3          11 
#define GPR_T4          12 
#define GPR_T5          13 
#define GPR_T6          14 
#define GPR_T7          15 
#define GPR_S0          16 
#define GPR_S1          17 
#define GPR_S2          18 
#define GPR_S3          19 
#define GPR_S4          20 
#define GPR_S5          21 
#define GPR_S6          22 
#define GPR_S7          23 
#define GPR_T8          24 
#define GPR_T9          25 
#define GPR_K0          26 
#define GPR_K1          27 
#define GPR_GP          28 
#define GPR_SP          29 
#define GPR_FP          30 
#define GPR_RA          31 

class GPR_IPE {
public:
    uint64_t read(int);
    void write(uint64_t, int);
    
    GPR_IPE(volatile uint64_t *gpr);    
    virtual ~GPR_IPE();
private:
    volatile uint64_t *gregs;
};

#endif	/* GPR_IPE_H */

