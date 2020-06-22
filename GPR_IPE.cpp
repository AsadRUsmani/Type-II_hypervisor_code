/* 
 * File:   GPR_IPE.cpp
 * Author: asadusmani
 * 
 * Created on June 4, 2014, 6:57 PM
 */

#include "GPR_IPE.h"

GPR_IPE::GPR_IPE(volatile uint64_t *gpr):gregs(gpr) 
{    
}



GPR_IPE::~GPR_IPE()
{
    
}





uint64_t GPR_IPE::read(int regid) {
    if(regid < GPR_ZERO || regid > GPR_RA)
    {
        fprintf(stderr, "GPR register no is wrong\n");
        exit(1);
    }
    return gregs[regid];
}


void GPR_IPE::write(uint64_t val, int regid) {
    if(regid < GPR_ZERO || regid > GPR_RA)
    {
        fprintf(stderr, "GPR register no is wrong\n");
        exit(1);
    }
    gregs[regid] = val;
}