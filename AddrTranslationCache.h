/* 
 * File:   AddrTranslationCache.h
 * Author: asadusmani
 *
 * Created on June 23, 2014, 9:14 AM
 */

#ifndef ADDRTRANSLATIONCACHE_H
#define	ADDRTRANSLATIONCACHE_H

#include <stdint.h>
#include "Context_IPE.h"

//#define INS_CACHE_SIZE  89
#define INS_CACHE_SIZE      128
#define ADDR_INDEX_MASK     (INS_CACHE_SIZE - 1)
#define GVA_BASE_MASK       0xFFFFFFFFFFFFF000

class AddrTranslationCache
{
public:
    AddrTranslationCache();
    virtual ~AddrTranslationCache();
    
    void Update_cache(uint64_t GVA, uint64_t HVA, uint64_t prot);
    void Clear_cache(void);
    void printUsageArray();
    
private:
    
    struct CacheEntry{
        uint64_t GVA;
        uint64_t HVA;
        uint64_t prot;
    };
    
    CacheEntry InsCache[INS_CACHE_SIZE];
    uint64_t usagearray[INS_CACHE_SIZE];
    
    /** Converts provided address to index of array
      * @param GVA: Guest Address
      * 
      * @retval array index
      */
    int getCacheIndex(uint64_t GVA)
    {
//        return (GVA % INS_CACHE_SIZE);
        return (GVA >> 12) & ADDR_INDEX_MASK;
    }
    
public:
    
    /** Gives the pointer to message box of device with given ID
      * @param GVA: Address to be translated
      * @param HVA: Pointer to location where translated address should be placed
      * @param context: Context of current instruction
      * 
      * @retval true if found else false
      */
    bool Check_cache(uint64_t GVA, uint64_t* HVA, Context_IPE *context)
    {
        uint64_t base = GVA & GVA_BASE_MASK;
        uint64_t offset = GVA - base;
        int index = this->getCacheIndex(base);

        *HVA = this->InsCache[index].HVA + offset;
        return !((this->InsCache[index].GVA != base) || (this->InsCache[index].prot < context->getLDST()));
    }
    
};

#endif	/* ADDRTRANSLATIONCACHE_H */

