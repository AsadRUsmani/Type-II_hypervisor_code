/* 
 * File:   MMU_IPE.h
 * Author: asadusmani
 *
 * Created on June 6, 2014, 4:33 PM
 */

#ifndef MMU_IPE_H
#define	MMU_IPE_H

//#include "Processor.h"
#include "Processor_IPE.h"
#include "Context_IPE.h"

#define BIT_32 0
#define BIT_64 1

#define useg_32bit      0
#define kseg0_32bit     4
#define kseg1_32bit     5
#define sseg_32bit      6
#define kseg3_32bit     7

#define xuseg_64bit   0
#define xsseg_64bit   1
#define xkphys_64bit  2
#define xkseg_64bit   3

#define TLB_NOT_REQUIRED 0
#define TLB_REQUIRED 1

#define BIT32_ADDRESS_MASK 0x1FFFFFFF
#define BIT64_ADDRESS_MASK 0x1FFFFFFFFFFFF
#define CVMSEG_MASK        0xFFFFFFFFFFFFC000
#define CVMSEG_BASE_ADDR   0xFFFFFFFFFFFF8000

#define XUSEG_MAX_VALID_ADDR    0X000000FFFFFFFFFF
#define XSSEG_MAX_VALID_ADDR    0X400000FFFFFFFFFF
#define XKSEG_MAX_VALID_ADDR    0XC00000FF7FFFFFFF

class MMUTranslator_IPE{
public:
    MMUTranslator_IPE();
    ~MMUTranslator_IPE(void);
    static uint64_t GVAtoGPA(Processor_IPE * core, uint64_t GVA, uint64_t* GPA, Context_IPE *context);
    static bool GVAtoGPA_for_fileloading(uint64_t GVA, uint64_t* GPA);
    
private:
    static inline uint64_t verify_priviliges(Processor_IPE * core, uint64_t addr);
    static inline uint32_t find_address_space_size(uint64_t addr);
    static inline uint32_t get_segment_number(uint64_t addr, uint32_t address_space_size);
    static inline uint32_t look_staic_translation_32bit(Processor_IPE * core, uint64_t Vaddr, uint64_t * Paddr);
    static inline uint32_t look_staic_translation_64bit(Processor_IPE * core, uint64_t Vaddr, uint64_t * Paddr);
private:
    
};

#endif	/* MMU_IPE_H */

