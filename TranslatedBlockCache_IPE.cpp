/* 
 * File:   TranslatedBlockCache_IPE.cpp
 * Author: asadusmani
 * 
 * Created on June 9, 2014, 2:52 PM
 */

#include "TranslatedBlockCache_IPE.h"

/** Initialises Translated block cache. It also creates hashmap and block to store translated blocks.
  * @param none
  * 
  * @retval none
  */
TranslatedBlockCache_IPE::TranslatedBlockCache_IPE()
{
        assert(this->cache != NULL);
        
        for(int i = 0; i < MAX_FIRST_LEVEL_BLOCK_COUNT; i++)
            for(int j = 0; j < MAX_SECOND_LEVEL_BLOCK_COUNT; j++)
            {
                CacheVal *cache = &(this->cache[i][j]);
                cache->key = i;
                cache->blockEndPC = 0;
                cache->translatedBlock = new TranslationBlock();
    //            this->usagearray[i] = 0;
                if(DBG)printf("DataPtr = 0x%016llx\n",(uint64_t)cache->translatedBlock->data());
            }
}

/** Destructor of Translated block cache. It un-allocates all the memory allocated to block for translation
  * @param none
  * 
  * @retval none
  */
TranslatedBlockCache_IPE::~TranslatedBlockCache_IPE()
{

    for(int i = 0; i < MAX_FIRST_LEVEL_BLOCK_COUNT; i++)
            for(int j = 0; j < MAX_SECOND_LEVEL_BLOCK_COUNT; j++)
            {
                delete (this->cache[i][j].translatedBlock);
            }   

    delete [] this->cache;
}

/** Puts the translated block in cache and updates protections of block
  * @param newSearchKey: Key that will be used to find and match the block
  * @param endPC: End PC of the translated block
  * 
  * @retval none
  */
void TranslatedBlockCache_IPE::putInCache(uint64_t newSearchKey, uint64_t endPC, uint8_t asid)
{
//    int index = this->doesExists(oldSearchKey);
//    if(index < 0)
//        return false;
    Index *index = &this->currentIndex;
    CacheVal *cacheData = &(this->cache[index->firstIndex][index->secondIndex]);
    cacheData->key = newSearchKey;
    cacheData->blockEndPC = endPC;
    cacheData->asid = asid;
    
    mprotect(cacheData->translatedBlock->data(), cacheData->translatedBlock->maxsize(), PROT_EXEC|PROT_READ);
    return;
}

/** Gets the container for given key to place translated instructions
  * @param GVA: Key that will be used to find and match the block in future
  * 
  * @retval pointer to container for placing translated instructions
  */
TranslationBlock *TranslatedBlockCache_IPE::getContainer(uint64_t GVA)
{
    Index *index = &this->currentIndex;
    this->updateIndex(GVA);
    TranslationBlock *block = this->cache[index->firstIndex][index->secondIndex].translatedBlock;
//    this->usagearray[index->firstIndex][index->secondIndex]++;
    mprotect(block->data(), block->maxsize(), PROT_WRITE|PROT_READ);
    block->clear();
    return block;
	
}

/*void TranslatedBlockCache_IPE::printUsageArray()
{
    for(int i = 0; i < MAX_CACHE_BLOCK_COUNT; i++)
        printf("Number %d is used %ld times\n",i,this->usagearray[i]);
}*/




