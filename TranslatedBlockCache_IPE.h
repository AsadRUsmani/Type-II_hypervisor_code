/* 
 * File:   TranslatedBlockCache_IPE.h
 * Author: asadusmani
 *
 * Created on June 9, 2014, 2:52 PM
 */

#ifndef TRANSLATEDBLOCKCACHE_IPE_H
#define	TRANSLATEDBLOCKCACHE_IPE_H

#include "Common.h"
//#include "TransInsAllocator.h"
#include "TranslationBlock.h"





//.........................................

//#define MAX_FIRST_LEVEL_BLOCK_COUNT     59
//#define MAX_SECOND_LEVEL_BLOCK_COUNT    31
#define FIRST_LEVEL_BLOCK_INDEX_BITS    7   //128 entries
#define SECOND_LEVEL_BLOCK_INDEX_BITS   6   //64 entries

/*Don't mess with these*/
#define MAX_FIRST_LEVEL_BLOCK_COUNT     0x01 << FIRST_LEVEL_BLOCK_INDEX_BITS
#define MAX_SECOND_LEVEL_BLOCK_COUNT    0x01 << SECOND_LEVEL_BLOCK_INDEX_BITS
#define FIRST_LEVEL_ADDR_MASK           (MAX_FIRST_LEVEL_BLOCK_COUNT) - 1
#define SECOND_LEVEL_ADDR_MASK          (MAX_SECOND_LEVEL_BLOCK_COUNT) - 1
#define FIRST_LEVEL_KEY_SHIFT_BITS      2   //Each intruction is 32-bit so lower 2 bits of address are always zero
#define SECOND_LEVEL_KEY_SHIFT_BITS     (FIRST_LEVEL_BLOCK_INDEX_BITS + FIRST_LEVEL_KEY_SHIFT_BITS) //2 bits plus bits in first level index

typedef struct {
    uint64_t key;
    uint64_t blockEndPC;
    TranslationBlock *translatedBlock; 
    uint8_t asid;   
} CacheVal;

class TranslatedBlockCache_IPE {
public:
	TranslatedBlockCache_IPE();
	virtual ~TranslatedBlockCache_IPE();

public:
	
        void putInCache(uint64_t newSearchKey, uint64_t endPC, uint8_t asid);
        TranslationBlock *getContainer(uint64_t GVA);  

//        void printUsageArray();

private://data
    
    struct Index {
        int firstIndex;
        int secondIndex;
    };

    CacheVal cache[MAX_FIRST_LEVEL_BLOCK_COUNT][MAX_SECOND_LEVEL_BLOCK_COUNT];
    Index currentIndex;
//    uint64_t usagearray[MAX_CACHE_BLOCK_COUNT];
        
        
    /** Updates the first and second index on Index object on the basis of provided key
      * @param Key: Key that is used to generate both indexes
      * 
      * @retval none
      */
    void updateIndex(uint64_t Key)
    {
        Index *index = &this->currentIndex;
//        index->firstIndex = Key % MAX_FIRST_LEVEL_BLOCK_COUNT;
//        index->secondIndex = Key % MAX_SECOND_LEVEL_BLOCK_COUNT;
        index->firstIndex = (Key >> FIRST_LEVEL_KEY_SHIFT_BITS) & FIRST_LEVEL_ADDR_MASK;
        index->secondIndex = (Key >> SECOND_LEVEL_KEY_SHIFT_BITS) & SECOND_LEVEL_ADDR_MASK;
    }
        
public:
    
    /** Searches the required block in cache and gives it's pointer if block exists otherwise return null
      * @param searchKey: Key that is used to find and match the block
      * 
      * @retval pointer to block if block exist otherwise null
      */
    CacheVal* doesExists(uint64_t searchKey, uint8_t asid)
    {
        Index *index = &this->currentIndex;
        this->updateIndex(searchKey);
        CacheVal *cacheData = &(this->cache[index->firstIndex][index->secondIndex]);

        if(cacheData->key != searchKey || cacheData->asid != asid)
            cacheData = NULL;
        return cacheData;

    }
    
};

#endif	/* TRANSLATEDBLOCKCACHE_IPE_H */

