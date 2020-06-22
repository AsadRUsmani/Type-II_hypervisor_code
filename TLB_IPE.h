/* 
 * File:   TLB_IPE.h
 * Author: asadusmani
 *
 * Created on June 5, 2014, 12:02 PM
 */

#ifndef TLB_IPE_H
#define	TLB_IPE_H

#include <inttypes.h>
#include "Common.h"
#include "CP0_IPE.h"

#include "Context_IPE.h"

using namespace std;

#define TLB_MAX_ENTERIES            64

#define TLB_MISMATCH                0
#define TLB_MATCH                   -1

#define ENTRY_GLOBAL                1
#define ENTRY_INVALID               1
#define ENTRY_VALID                 0

class TLB_Entry_IPE{
public:
    TLB_Entry_IPE();
    ~TLB_Entry_IPE(void);
    
    inline uint64_t get_pagemask(void);
    inline uint64_t get_entryHi(void);
    inline uint64_t get_entryLo0(void);
    inline uint64_t get_entryLo1(void);
    
    inline void set_Entry(CP0_IPE* CP0);
    inline void reset_Entry(void);
    
    void print_entry(void);
    
private:
    inline void set_pagemask(uint64_t mask);
    inline void set_entryHi(uint64_t entry);
    inline void set_entryLo0(uint64_t entry);
    inline void set_entryLo1(uint64_t entry);
    
private:
    uint64_t pagemask;
    uint64_t entryHi;
    uint64_t entryLo0;
    uint64_t entryLo1;
public:
    uint64_t VPN;
    uint8_t ASID;
    uint8_t R;
    uint16_t G:1;
    uint16_t Validity:1;
    uint16_t C0:3;
    uint16_t C1:3;
    uint16_t V0:1;
    uint16_t V1:1;
    uint16_t D0:1;
    uint16_t D1:1;
    uint64_t PFN[2];

    
};/*TLB_Entry*/

class TLB_IPE{
public:
    TLB_IPE(CP0_IPE* cp0);
    ~TLB_IPE(void);
    
    bool GVAtoGPA( uint64_t GVA, uint64_t* GPA, Context_IPE *context);
    void tlb_probe();
    void tlb_random();
    void tlb_write_index();
    void tlb_read();
    
    
    void print_TLBEntry(uint32_t idx);
    void print_TLB(void);
    void write_tlb_state(FILE *fp);
    
    
    uint64_t get_index();
    
private:
    inline uint32_t get_random();
    inline void set_random(uint64_t value);
    inline void inc_random();
    inline uint32_t get_wired();
    inline void set_index(uint32_t index_val);
    inline void set_probe();
    inline void set_pagemask(uint64_t mask);
    inline void set_entryHi(uint64_t entry);
    inline uint64_t get_entryHi();
    inline void set_entryLo0(uint64_t entry);
    inline void set_entryLo1(uint64_t entry);
    inline uint8_t check_region();
    inline uint8_t get_ASID();
    inline void get_TLBEntry(uint32_t idx);
    inline void reset_TLBEntry(uint32_t idx);
    inline void clear_TLB(void);
    
private:
    
    CP0_IPE *cp0;    
    TLB_Entry_IPE tlb_entry[TLB_MAX_ENTERIES];
    
};/*TLB*/

#endif	/* TLB_IPE_H */

