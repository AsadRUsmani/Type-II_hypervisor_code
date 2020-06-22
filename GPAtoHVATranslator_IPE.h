/* 
 * File:   GPAtoHVATranslator_IPE.h
 * Author: asadusmani
 *
 * Created on June 9, 2014, 11:47 AM
 */

#ifndef GPATOHVATRANSLATOR_IPE_H
#define	GPATOHVATRANSLATOR_IPE_H

#include <map>
#include "Common.h"
#include "Context_IPE.h"
using namespace std;

struct GPAtoHVAmap_Val_IPE
{
    uint64_t HVA;
    uint32_t size;
    uint32_t map_idx;
};

class GPAtoHVATranslator_IPE
{
public:
    GPAtoHVATranslator_IPE(void);
    ~GPAtoHVATranslator_IPE(void);
    static uint64_t GPA_to_HVA(uint64_t addr);
    static void initialize_GPA_subsections(uint64_t ramSize);
    static void * allocMemRegion(uint64_t);
        
        
private://methods related to putting info
    
    static void init_map_array(void);
    
    static void alloc_DRAM(uint64_t ramSize);
    static void add_Mem_Region(uint64_t size, uint64_t key);
    static uint64_t add_IO_Region(uint64_t key);
    static uint64_t find_index(uint64_t addr, uint64_t *size);
    
private://methods used by GPA_to_HVA    
    static GPAtoHVAmap_Val_IPE translate_GPA_to_HVA(uint64_t key);
    static void get_base_and_offset(uint64_t GPA, uint64_t *base, uint64_t *offset);
    
    
    
private:
    static map<uint64_t,GPAtoHVAmap_Val_IPE> GPAtoHVAMap_IPE;
};

#endif	/* GPATOHVATRANSLATOR_IPE_H */

