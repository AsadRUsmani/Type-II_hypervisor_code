#include "TranslationBlock.h"

/** Constructor of TranslationBlock. It provides a container to place translated instructions
  * @param none
  * 
  * @retval none
  */
TranslationBlock::TranslationBlock()
{
    this->container = this->allocateBlock(INITIALBLOCKSIZE);
    
    this->currentlocation = this->container;
    this->currentsize = 0;
    this->allocatedsize = (INITIALBLOCKSIZE /  INS_SIZE);
}

/** Destructor of TranslationBlock. It de-allocates the allocated memory and free up resources
  * @param none
  * 
  * @retval none
  */
TranslationBlock::~TranslationBlock()
{
    this->deallocateBlock();
}

/** Allocates a new memory block of required size
  * @param size: Size of the requested region
  * 
  * @retval pointer to newly allocated block
  */
uint32_t * TranslationBlock::allocateBlock(uint32_t size)
{
    uint32_t *map = (uint32_t *)mmap(NULL, size, PROT_READ|PROT_WRITE|PROT_EXEC,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
    if (map == MAP_FAILED)
    {
        perror("dest_mmap not found\n");
        exit(1);
    }
    
    return map;
}

/** De-allocates the memory block
  * @param none
  * 
  * @retval none
  */
void TranslationBlock::deallocateBlock()
{
    munmap((void*) this->container, this->allocatedsize * INS_SIZE);
}

/** Gives the current size of the block
  * @param none
  * 
  * @retval size of the block
  */
uint32_t TranslationBlock::size()
{
    return this->currentsize;
}

/** Clear the container for re-using
  * @param none
  * 
  * @retval none
  */
void TranslationBlock::clear()
{
    if(this->allocatedsize > (INITIALBLOCKSIZE /  INS_SIZE))
    {
        this->deallocateBlock();
        this->container = this->allocateBlock(INITIALBLOCKSIZE);
        this->allocatedsize = (INITIALBLOCKSIZE /  INS_SIZE);
    }
    else
        memset((void*) this->container, '\0', this->currentsize);
    this->currentsize = 0;
    this->currentlocation = this->container;
}

/** Expands the block. It allocates a new block of provided size and copies the content from old block to new block
  * @param value: Size of new block
  * 
  * @retval none
  */
void TranslationBlock::expandBlock(uint32_t value)
{
    uint32_t *newcontainer = this->allocateBlock(this->allocatedsize * 2 * INS_SIZE);
    
    //Copy old block to new location and update class data according to new container
    memcpy(newcontainer, this->container, this->allocatedsize * INS_SIZE);
    this->deallocateBlock();
    this->currentlocation = newcontainer + (this->currentlocation - this->container);
    this->container = newcontainer;
    this->allocatedsize *= 2;
}