/* 
 * File:   CP0_IPE.cpp
 * Author: asadusmani
 * 
 * Created on June 5, 2014, 10:56 AM
 */

#include "CP0_IPE.h"

/** Initialises all CP0 registers
  * @param none
  * 
  * @retval none
  */
void CP0_IPE::initCP0() {
    
    

    reg[H2_CP0_INDEX            * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_RANDOM           * CP0_NO_OF_SELECT + 0]     = 0x000000000000003fLL;
    reg[H2_CP0_ENTRYLO0         * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_ENTRYLO1         * CP0_NO_OF_SELECT + 0]     = 0x000000000012ebc6LL;
    reg[H2_CP0_CONTEXT          * CP0_NO_OF_SELECT + 0]     = 0x0LL; //not avaiable
    reg[H2_CP0_PAGEMASK         * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_PAGEGRAIN_1      * CP0_NO_OF_SELECT + 1]     = 0x0LL;
    reg[H2_CP0_WIRED            * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_HWRENA           * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_BADVADDR         * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_COUNT            * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_ENTRYHI          * CP0_NO_OF_SELECT + 0]     = 0xc001ffffffffe000LL;
    reg[H2_CP0_COMPARE          * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_STATUS           * CP0_NO_OF_SELECT + 0]     = 0x0000000000400004LL;
    reg[H2_CP0_INTCTL_1         * CP0_NO_OF_SELECT + 1]     = 0x0LL; //not avaiable
    reg[H2_CP0_SRSCTL_2         * CP0_NO_OF_SELECT + 2]     = 0x0LL;
    reg[H2_CP0_CAUSE            * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_EPC              * CP0_NO_OF_SELECT + 0]     = 0x0LL; //not avaiable
    reg[H2_CP0_PRID             * CP0_NO_OF_SELECT + 0]     = 0x00000000000d0408LL;
    reg[H2_CP0_EBASE_1          * CP0_NO_OF_SELECT + 1]     = 0xffffffff80000000LL;
    reg[H2_CP0_CONFIG           * CP0_NO_OF_SELECT + 0]     = 0xffffffff8000c48aLL;
    reg[H2_CP0_CONFIG_1         * CP0_NO_OF_SELECT + 1]     = 0xfffffffffe3303daLL;
    reg[H2_CP0_CONFIG_2         * CP0_NO_OF_SELECT + 2]     = 0xffffffff80000467LL;
    reg[H2_CP0_CONFIG_3         * CP0_NO_OF_SELECT + 3]     = 0x0000000000001080LL;
    reg[H2_CP0_CONFIG_4         * CP0_NO_OF_SELECT + 4]     = 0x0000000000000080LL;
    reg[H2_CP0_IWATCHLO         * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_DWATCHLO_1       * CP0_NO_OF_SELECT + 1]     = 0x0LL;
    reg[H2_CP0_IWATCHHI         * CP0_NO_OF_SELECT + 0]     = 0xffffffff80000ff8LL;
    reg[H2_CP0_DWATCHHI_1       * CP0_NO_OF_SELECT + 1]     = 0x0LL; //not avaiable
    reg[H2_CP0_XCONTEXT         * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_DEBUG            * CP0_NO_OF_SELECT + 0]     = 0x000000004011fc01LL;
    reg[H2_CP0_DEBUG_EPC        * CP0_NO_OF_SELECT + 0]     = 0x0LL; //not avaiable
    reg[H2_CP0_PERFCNTCTL       * CP0_NO_OF_SELECT + 0]     = 0xffffffffc0000000LL;
    reg[H2_CP0_PERFCNTCTL_2     * CP0_NO_OF_SELECT + 2]     = 0x0000000040000000LL;
    reg[H2_CP0_PERFCNTCNT_1     * CP0_NO_OF_SELECT + 1]     = 0x0LL;
    reg[H2_CP0_PERFCNTCNT_3     * CP0_NO_OF_SELECT + 3]     = 0x0LL; //not avaiable
    reg[H2_CP0_ERROREPC         * CP0_NO_OF_SELECT + 0]     = 0x0000000120003a5cLL;
    reg[H2_CP0_DESAVE           * CP0_NO_OF_SELECT + 0]     = 0x0LL;


    reg[H2_CP0_ICACHEERR        * CP0_NO_OF_SELECT + 0]     = 0x0LL;
    reg[H2_CP0_DCACHEERR_1      * CP0_NO_OF_SELECT + 1]     = 0x0LL;
    reg[H2_CP0_ICACHETAGLO      * CP0_NO_OF_SELECT + 0]     = 0x0LL; //not avaiable
    reg[H2_CP0_DCACHETAGLO_2    * CP0_NO_OF_SELECT + 2]     = 0xffffffffffffffffLL;
    reg[H2_CP0_ICACHEDATALO_1   * CP0_NO_OF_SELECT + 1]     = 0xfe677ecfdeff0ff0LL;
    reg[H2_CP0_DCACHEDATALO_3   * CP0_NO_OF_SELECT + 3]     = 0xffffffffffffffffLL;
    reg[H2_CP0_DCACHETAGHI_2    * CP0_NO_OF_SELECT + 2]     = 0x0LL; //not avaiable
    reg[H2_CP0_ICACHEDATAHI_1   * CP0_NO_OF_SELECT + 1]     = 0x0LL;
    reg[H2_CP0_DCACHEDATAHI_3   * CP0_NO_OF_SELECT + 3]     = 0x0LL;
    reg[H2_CP0_CVMCTL_7         * CP0_NO_OF_SELECT + 7]     = 0x00000000b80003F0LL;
    //reg [H2_CP0_CVMCTL_9         * CP0_NO_OF_SELECT + 9] = 0x0000000000000000LL;
    reg[H2_CP0_CVMMEMCTL_7      * CP0_NO_OF_SELECT + 7]     = 0x0000000000046000LL; //not avaiable
    reg[H2_CP0_CVMCOUNT_6       * CP0_NO_OF_SELECT + 6]     = 0x0LL;
    reg[H2_CP0_MULTICORE_DEBUG  * CP0_NO_OF_SELECT + 0]     = 0x0LL;
 
}

/** Prints all CP0 registers
  * @param none
  * 
  * @retval none
  */
void CP0_IPE::printCP0State() {

    printf("0x%016llx\n", reg[H2_CP0_INDEX              * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_RANDOM             * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_ENTRYLO0           * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_ENTRYLO1           * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_CONTEXT            * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_PAGEMASK           * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_PAGEGRAIN_1        * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_WIRED              * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_HWRENA             * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_BADVADDR           * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_COUNT              * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_ENTRYHI            * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_COMPARE            * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_STATUS             * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_INTCTL_1           * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_SRSCTL_2           * CP0_NO_OF_SELECT + 2]);
    printf("0x%016llx\n", reg[H2_CP0_CAUSE              * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_EPC                * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_PRID               * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_EBASE_1            * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_CONFIG             * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_CONFIG_1           * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_CONFIG_2           * CP0_NO_OF_SELECT + 2]);
    printf("0x%016llx\n", reg[H2_CP0_CONFIG_3           * CP0_NO_OF_SELECT + 3]);
    printf("0x%016llx\n", reg[H2_CP0_IWATCHLO           * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_DWATCHLO_1         * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_IWATCHHI           * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_DWATCHHI_1         * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_XCONTEXT           * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_DEBUG              * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_DEBUG_EPC          * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_PERFCNTCTL         * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_PERFCNTCTL_2       * CP0_NO_OF_SELECT + 2]);
    printf("0x%016llx\n", reg[H2_CP0_PERFCNTCNT_1       * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_PERFCNTCNT_3       * CP0_NO_OF_SELECT + 3]);
    printf("0x%016llx\n", reg[H2_CP0_ERROREPC           * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_DESAVE             * CP0_NO_OF_SELECT + 0]);

    printf("0x%016llx\n", reg[H2_CP0_ICACHEERR          * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_DCACHEERR_1        * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_ICACHETAGLO        * CP0_NO_OF_SELECT + 0]);
    printf("0x%016llx\n", reg[H2_CP0_DCACHETAGLO_2      * CP0_NO_OF_SELECT + 2]);
    printf("0x%016llx\n", reg[H2_CP0_ICACHEDATALO_1     * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_DCACHEDATALO_3     * CP0_NO_OF_SELECT + 3]);
    printf("0x%016llx\n", reg[H2_CP0_DCACHETAGHI_2      * CP0_NO_OF_SELECT + 2]);
    printf("0x%016llx\n", reg[H2_CP0_ICACHEDATAHI_1     * CP0_NO_OF_SELECT + 1]);
    printf("0x%016llx\n", reg[H2_CP0_DCACHEDATAHI_3     * CP0_NO_OF_SELECT + 3]);
    printf("0x%016llx\n", reg[H2_CP0_CVMCTL_7           * CP0_NO_OF_SELECT + 7]);
    printf("0x%016llx\n", reg[H2_CP0_CVMMEMCTL_7        * CP0_NO_OF_SELECT + 7]);
    printf("0x%016llx\n", reg[H2_CP0_CVMCOUNT_6         * CP0_NO_OF_SELECT + 6]);
    printf("0x%016llx\n", reg[H2_CP0_MULTICORE_DEBUG    * CP0_NO_OF_SELECT + 0]);
}

CP0_IPE::CP0_IPE(volatile uint64_t *pcp0):reg(pcp0) 
{
    initCP0();
}


CP0_IPE::~CP0_IPE() {
}

/** Increment Cavium_Count register by value of MIN_STE_COUNT
  * @param none
  * 
  * @retval none
  */
void CP0_IPE::inc_CVMCount()
{
    this->reg[H2_CP0_CVMCOUNT_6 * CP0_NO_OF_SELECT + SEL_SIX] += MIN_COUNT_STEP;
}

/** Increment Count register by value of MIN_STE_COUNT and return incremented value
  * @param none
  * 
  * @retval Incremented value of Count register
  */
uint64_t CP0_IPE::inc_and_get_count()
{
    uint64_t count = this->reg[H2_CP0_COUNT * CP0_NO_OF_SELECT + SEL_ZERO];
    if(!(this->reg[H2_CP0_CAUSE * CP0_NO_OF_SELECT + SEL_ZERO] & CAUSE_DC)) //Checking Disable_Count Bit of Cause register
    {
        count = (count + MIN_COUNT_STEP) & 0xFFFFFFFF;
        this->reg[H2_CP0_COUNT * CP0_NO_OF_SELECT + SEL_ZERO] = count;
    }
    return count;
}

/** Read core number using Ebase register
  * @param none
  * 
  * @retval core number of processor
  */
uint32_t CP0_IPE::corenum()
{
    return (uint32_t)(this->reg[H2_CP0_EBASE_1 * CP0_NO_OF_SELECT + SEL_ONE] & EBASE_CORENUM_MASK);
}

/** Skips reserved bits from give value on basis of register no. and select no.
  * @param val: Value to be written
  * @param reg: Register no.
  * @param sel: Select no.
  * 
  * @retval modified value with reserved bits set
  */
uint64_t CP0_IPE::ReservedBits(int reg, int sel, uint64_t contents){

    uint64_t a= contents;
    
    switch(reg){
//        case 0:
//            if(sel==0){         // index
//                a &= 0x8000003f;
//                a = (a << 32) >>32 ; //giving sign extension
//            }
//            break;
        case 1:
            if(sel==0){         //random
                a &= 0x0000003f;
                a = (a << 32) >>32 ; //giving sign extension
            }
           break;
//        case 2:  case 3:         //entryLo0 and 1
//            if(sel==0){         
//                a &= 0xc00007ffffffffff;
//            }
//            break;
        case 4:
            if(sel==0){         // context reg
                a &= 0xfffffffffffffff0;
            }
            break;
        case 5:
//            if(sel==0){         //PageMask reg
//             a &= 0x1fffe000;
//             a = (a << 32) >>32 ; //giving sign extension
//            }else 
                if(sel ==1){    //page grain
                 a &= 0xe0000000;
                 a = (a << 32) >>32 ; //giving sign extension
                }
            break;
        case 6:
            if(sel==0){ //      wired Reg
             a &= 0x0000003f;
             a = (a << 32) >>32 ; //giving sign extension
            }
            break;
        case 7:
            if(sel==0){ //      hwrena reg
             a &= 0xc000000f;
             a = (a << 32) >>32 ; //giving sign extension
            }
            break;
        case 9:
            if(sel==0){ //      count
             a &= 0xffffffff;
            }
            break;
//        case 10:
//            if(sel==0){ //      Entryhi reg
//             a &= 0xc001ffffffffe0ff;
//            }
//            break;
        case 11:
            if(sel==0){ //      compare
             a &= 0xffffffff;
            }
            break;
        case 12:
            if(sel==0){ //      status
             a &= 0x50f8ffff;
             a = (a << 32) >>32 ; //giving sign extension
            }else
                if(sel == 1){ //        intctl
                a &= 0xfc000000;
                a = (a << 32) >>32 ; //giving sign extension
                } else
                    if(sel ==2 ){ //    srsctl
                     a &= 0x0;
                    }
            break;
        case 13:
            if(sel==0){ //      cause
              a &= 0xfcc0ff7c;
              a = (a << 32) >>32 ; //giving sign extension
            }
            break;
        case 15:
            if(sel==0){ //      prid
//             a &= 0x0000000f;
//             a |= 0x000d0400; 
            }else
                if(sel==1){ //  ebase
//                    printf("ebase read\n");
                 a &= 0xbffff3ff;
                 a |= 0x80000000;
                 a = (a << 32) >>32 ; //giving sign extension
                }
            break;
        case 16:
            if(sel==0){ //      config
                 a &= 0x8000c48f;
                 a |= 0x80004488;
                 a = (a << 32) >>32 ; //giving sign extension
            }
            break;
        case 20:
            if(sel==0){ //      xcontext
                a &= 0xfffffffffffffff0;
            }
            break;
  
        default:        // Entry Lo0, Entry Lo1, badvaddr, epc
            break;
    }
  
    return a;
}

