/* 
 * File:   CP0_IPE.h
 * Author: asadusmani
 *
 * Created on June 5, 2014, 10:56 AM
 */

#ifndef CP0_IPE_H
#define	CP0_IPE_H

#include "Common.h"

#define H2_NO_OF_CP0_REG        32 /* No. of registers in CP0 */
#define CP0_NO_OF_SELECT        8  /* No. of configure register in cp0. i.e. 8 sel:0-7*/

/* Each #define represent register# and select=0 is implicit. 
 * Non-zero select is _postfixed, explicity.  */
#define H2_CP0_INDEX            0         /* TLB Index,                 0,32 <Desc, select, size>*/
#define H2_CP0_RANDOM           1         /* TLB Random,                0,32 */
#define H2_CP0_ENTRYLO0         2         /* TLB Entry Lo0,             0,64 */
#define H2_CP0_ENTRYLO1         3         /* TLB Entry Lo1,             0,64 */
#define H2_CP0_CONTEXT          4         /* Kernel PTE pointer,        0,64 */
#define H2_CP0_PAGEMASK         5         /* TLB Page Mask,             0,32 */
#define H2_CP0_PAGEGRAIN_1      5         // 1,32
#define H2_CP0_WIRED            6         /* TLB Wired,                 0,32 */
#define H2_CP0_HWRENA           7         // 0,32
#define H2_CP0_BADVADDR         8         /* Bad Virtual Address,       0,64 */
#define H2_CP0_COUNT            9         /* Count, 0,32                 */
#define H2_CP0_ENTRYHI          10        /* TLB Entry Hi,              0,64 */
#define H2_CP0_COMPARE          11        /* Timer Compare,             0,32 */
#define H2_CP0_STATUS           12        /* Status,                    0,32 */
#define H2_CP0_INTCTL_1         12         //1,32
#define H2_CP0_SRSCTL_2         12         //2,32
#define H2_CP0_CAUSE            13        /* Cause,                     0,32 */
#define H2_CP0_EPC              14        /* Exception Program Counter, 0,64*/
#define H2_CP0_PRID             15        /* Proc Rev ID,               0,32 */
#define H2_CP0_EBASE_1          15        // 1,32
#define H2_CP0_CONFIG           16        /* Configuration,             0,32 */
#define H2_CP0_CONFIG_1         16         //1,32
#define H2_CP0_CONFIG_2         16         //2,32
#define H2_CP0_CONFIG_3         16         //3,32
#define H2_CP0_CONFIG_4         16         //3,32
#define H2_CP0_LLADDR           17         //LLAddr register
#define H2_CP0_IWATCHLO         18         /* Low Watch address,        0,64 */
#define H2_CP0_DWATCHLO_1       18         //1,64
#define H2_CP0_IWATCHHI         19         /* High Watch address,       0,32 */
#define H2_CP0_DWATCHHI_1       19         //1,32
#define H2_CP0_XCONTEXT         20         /* Extended context,         0,64 */
#define H2_CP0_DEBUG            23         //0,32
#define H2_CP0_DEBUG_EPC        24         //0,64
#define H2_CP0_PERFCNTCTL       25         /*Performance Counter Control, 0, 32 */
#define H2_CP0_PERFCNTCTL_2     25         /*Performance Counter Control, 2, 32 */
#define H2_CP0_PERFCNTCNT_1     25         //Performance Counter Counter, 1, 64 */
#define H2_CP0_PERFCNTCNT_3     25         //Performance Counter Counter, 3, 64 */
#define H2_CP0_ERROREPC         30         /* Error exception PC,         0, 64 */
#define H2_CP0_DESAVE           31         //0,64

//Cavium Specific			
#define H2_CP0_ICACHEERR        27         /*Cache Error Icache Register, 0, 64 */ 
#define H2_CP0_DCACHEERR_1      27         /*Cache Error Dcache Register, 1, 64 */ 
#define H2_CP0_ICACHETAGLO      28         /*Icache Tag Lo,               0, 64 */ 
#define H2_CP0_DCACHETAGLO_2    28         /*DCACHE TagLo,	2, 64 */
#define H2_CP0_ICACHEDATALO_1   28         /*ICACHE DataLo, 1, 64 */
#define H2_CP0_DCACHEDATALO_3   28         /*DCACHE DataLo, 3, 64 */
#define H2_CP0_DCACHETAGHI_2    29         /*Dcache TagHi, 2, 64 */
#define H2_CP0_ICACHEDATAHI_1   29         /*Icache DataHi, 1, 64 */
#define H2_CP0_DCACHEDATAHI_3   29         /*Dcache DataHi, 3, 64 */
#define H2_CP0_CVMCTL_7          9         /*Cvm Control Register, 7, 64 */
//#define H2_CP0_CVMCTL_9       9 
#define H2_CP0_CVMMEMCTL_7      11         /*Cvm Memory Control Register, 7, 64 */
#define H2_CP0_CVMCOUNT_6        9         /*CvmCount Register, 6, 64 */
#define H2_CP0_MULTICORE_DEBUG  22         /*Multicore Debug Register,0, 64 */

#define SEL_ZERO    0
#define SEL_ONE     1
#define SEL_TWO     2
#define SEL_THREE   3
#define SEL_SIX     6
#define SEL_SEVEN   7

//Processor modes
#define KERNEL_MODE             0
#define SUPERVISOR_MODE         1
#define USER_MODE               2

// CP0 Status Register 
#define STATUS_CU0                          0x10000000
#define STATUS_CU1                          0x20000000
#define STATUS_BEV                          0x00400000
#define STATUS_TS                           0x00200000
#define STATUS_SR                           0x00100000
#define STATUS_CH                           0x00040000
#define STATUS_CE                           0x00020000
#define STATUS_DE                           0x00010000
#define STATUS_RP                           0x08000000
#define STATUS_FR                           0x04000000
#define STATUS_RE                           0x02000000
#define STATUS_KX                           0x00000080
#define STATUS_SX                           0x00000040
#define STATUS_UX                           0x00000020
#define STATUS_KSU_MASK                     0x00000018
#define STATUS_ERL                          0x00000004
#define STATUS_EXL                          0x00000002
#define STATUS_IE                           0x00000001
#define STATUS_IMASK7                       0x00008000
#define STATUS_IMASK6                       0x00004000
#define STATUS_IMASK5                       0x00002000
#define STATUS_IMASK4                       0x00001000
#define STATUS_IMASK3                       0x00000800
#define STATUS_IMASK2                       0x00000400
#define STATUS_IMASK1                       0x00000200
#define STATUS_IMASK0                       0x00000100
#define STATUS_KSU                          3

#define STATUS_DS_MASK                      0x00770000
#define STATUS_CU_MASK                      0xF0000000
#define STATUS_IMASK                        0x0000FF00

#define CAUSE_EXEC_OFFSET                   2
#define CAUSE_EXEC_MASK                     0x0000007C
#define CAUSE_IP_MASK                       0x0000FF00
#define CAUSE_IP0                           0x00000100
#define CAUSE_IP1                           0x00000200
#define CAUSE_IP2                           0x00000400
#define CAUSE_IP3                           0x00000800
#define CAUSE_IP4                           0x00001000
#define CAUSE_IP5                           0x00002000
#define CAUSE_IP6                           0x00004000
#define CAUSE_IP7                           0x00008000
#define CAUSE_IV                            0x00800000
#define CAUSE_DC                            0x08000000
#define CAUSE_TI                            0x40000000

#define INDEX_MASK                          0x3F
#define INDEX_PROBE_MASK                    0xFFFFFFFF80000000ULL
#define RANDOM_MASK                         INDEX_MASK
#define WIRED_MASK                          INDEX_MASK

#define MIN_PAGE_MASK                       0x1FFF
#define MIN_PAGE_BITS                       12

#define ENTRYHI_ASID_MASK                   0xFF
#define ENTRYHI_VPN                         13
#define ENTRYHI_REGION                      62

#define ENTRYLO_GLOBAL_MASK                      0x01
#define ENTRYLO_VALID_MASK                  0x02
#define ENTRYLO_DIRTY_MASK                  0x04
#define ENTRYLO_COHERENCY_MASK              0x07
#define ENTRYLO_VALID                       1
#define ENTRYLO_DIRTY                       2
#define ENTRYLO_COHERENCY                   3
#define ENTRYLO_PFN                         6

#define EBASE_CORENUM_MASK                  0x3FF

#define XCONTEXT_PTEBASE_MASK               0xFFFFFC0000000000
#define XCONTEXT_VPN                        4
#define XCONTEXT_REGION                     40
#define XCONTEXT_PTE                        42

#define CONTEXT_PTEBASE_MASK                0xFFFFFFFFFF800000
#define CONTEXT_VPN_MASK                    0x007FFFF0
#define CONTEXT_VPN                         4
#define CONTEXT_PTE                         23

#define CVMMEMCTL_LMEMSZ_MASK               0x0000003F
#define CVMMEMCTL_CVMSEGENAU                0x00000040
#define CVMMEMCTL_CVMSEGENAS                0x00000080
#define CVMMEMCTL_CVMSEGENAK                0x00000100

#define MIN_COUNT_STEP                      1


class CP0_IPE {
public:
    CP0_IPE(volatile uint64_t *cp0);
    virtual ~CP0_IPE();
    
    void initCP0();
    
    void inc_CVMCount();
    uint64_t inc_and_get_count();
    uint32_t corenum();
    void printCP0State();
    
private:
    
    uint64_t ReservedBits(int , int , uint64_t ); //reg, select,contents
    
private:
    volatile uint64_t *reg;   
    
public: //In-line public methods
    
    /** Detects the current operating mode of processor whether it is operating in Kernel mode or User mode
    * @param none
    * 
    * @retval Current operating mode of processor
    */
  uint32_t get_KSU() {
      uint32_t status = (uint32_t) this->reg[H2_CP0_STATUS * CP0_NO_OF_SELECT + SEL_ZERO];
      uint32_t mode = (status & STATUS_KSU_MASK) >> STATUS_KSU;
      if(mode != KERNEL_MODE)
      {
          if((status & (STATUS_ERL | STATUS_EXL)) != 0){
              mode = KERNEL_MODE;
          }
      }
      return mode;
  }

  /** Checks pending interrupt status of CPU
    * @param none
    * 
    * @retval true if interrupt pending otherwise false
    */
  bool isInterruptPending()
  {
      bool IE;
      uint64_t status = this->reg[H2_CP0_STATUS * CP0_NO_OF_SELECT + SEL_ZERO];
      uint64_t cause = this->reg[H2_CP0_CAUSE * CP0_NO_OF_SELECT + SEL_ZERO];
      cause &= (status & STATUS_IMASK);
      IE = ((status & (STATUS_IE | STATUS_EXL | STATUS_ERL)) == STATUS_IE);
      return ((cause > 0) && IE);
  }

  /** Reads a CP0 register
    * @param reg: Register no.
    * @param sel: Select no.
    * 
    * @retval value of required CP0 register
    */
  uint64_t read(int reg, int sel) {
      return this->reg[reg * CP0_NO_OF_SELECT + sel];
  }

  /** Writes a CP0 register
    * @param val: Value to be written
    * @param reg: Register no.
    * @param sel: Select no.
    * 
    * @retval none
    */
  void write(uint64_t val, int reg, int sel) {
      this->reg[reg * CP0_NO_OF_SELECT + sel] = ReservedBits(reg,sel,val);
  }

};

#endif	/* CP0_IPE_H */