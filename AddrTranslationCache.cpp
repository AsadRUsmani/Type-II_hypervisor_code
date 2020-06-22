#include "AddrTranslationCache.h"

/** Constructor of AddrTranslationCache. This cache saves the translated addresses and provide them later
  * @param none
  * @retval none
  */
AddrTranslationCache::AddrTranslationCache()
{
//    for(int i = 0; i < INS_CACHE_SIZE; i++)
//        this->usagearray[i] = 0;
    
    srand(time(0));
}

/** Destructor of AddrTranslationCache. This cache saves the translated addresses and provide them later
  * @param none
  * 
  * @retval none
  */
AddrTranslationCache::~AddrTranslationCache()
{
    
}

/** Updates the entry in the cache
  * @param GVA: Guest Address
  * @param HVA: Translated Address
  * @param prot: Protection of page i.e. page is write-able or not
  * 
  * @retval none
  */
void AddrTranslationCache::Update_cache(uint64_t GVA, uint64_t HVA, uint64_t prot)
{
    uint64_t base = GVA & GVA_BASE_MASK;
    uint64_t offset = GVA - base;
    int index = this->getCacheIndex(base);
    
//    this->usagearray[index]++;
    this->InsCache[index].GVA = base;
    this->InsCache[index].HVA = HVA - offset;
    this->InsCache[index].prot = prot;
}

/** Clears all the entries in cache
  * @param none
  * 
  * @retval none
  */
void AddrTranslationCache::Clear_cache()
{
    for(int i = 0; i < INS_CACHE_SIZE; i++)
    {
        this->InsCache[i].GVA = 0x1;
        this->InsCache[i].HVA = 0x0;
        this->InsCache[i].prot = READ;
    }
    
}

/** Prints the usage of each index in array. Represent how useful is our cache size
  * @param none
  * 
  * @retval none
  */
void AddrTranslationCache::printUsageArray()
{
    for(int i = 0; i < INS_CACHE_SIZE; i++)
        printf("Number %d is used %ld times\n",i,this->usagearray[i]);
}