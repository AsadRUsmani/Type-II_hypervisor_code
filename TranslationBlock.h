/* 
 * File:   TranslationBlock.h
 * Author: usamanwar
 *
 * Created on 25 March 2015, 11:34
 */

#ifndef TRANSLATIONBLOCK_H
#define	TRANSLATIONBLOCK_H

#include "Common.h"

#define INITIALBLOCKSIZE    1024 * 8    //Container for 2048 instructions
#define INS_SIZE            4

class TranslationBlock
{
public:
    TranslationBlock();
    ~TranslationBlock();
    uint32_t size();
    void clear();
    
private:
    inline uint32_t * allocateBlock(uint32_t);
    void expandBlock(uint32_t);
    inline void deallocateBlock();
private:
    uint32_t *container;
    uint32_t *currentlocation;
    uint32_t currentsize;
    uint32_t allocatedsize;
    
public: //In-line public functions
    
    /** Adds new instruction to the container. If container is full, it expands the container
      * @param value: Instruction to be pushed in container
      * 
      * @retval none
      */
    void push_back(uint32_t value)
    {
        *(this->currentlocation++) = value;
        if(unlikely(++this->currentsize >= this->allocatedsize) )
        {
            if(DBG)printf("Required larger container\n");
            this->expandBlock(value);
        }

    }
    
    /** Gives pointer to the instruction container
      * @param none
      * 
      * @retval pointer to container
      */
    uint32_t* data()
    {
        return this->container;
    }
    
    /** Gives the maximum size of the container
      * @param none
      * 
      * @retval max size of container
      */
    uint32_t maxsize()
    {
        return (this->allocatedsize * INS_SIZE);
    }
};

#endif	/* TRANSLATIONBLOCK_H */

