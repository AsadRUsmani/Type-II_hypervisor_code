/* 
 * File:   Context_IPE.h
 * Author: asadusmani
 *
 * Created on June 9, 2014, 10:32 AM
 */

#ifndef CONTEXT_IPE_H
#define	CONTEXT_IPE_H

#include "Common.h"

#define READ                0
#define WRITE               1

class Context_IPE {
        
private://data
    uint64_t *loadstore;
    uint64_t *PC;
    uint64_t *writeData;
    uint64_t *prot;
    
public:
    
    /** Constructor for Context class
      * @param loadstore: pointer to operation indicator whether operation is load or store
      * @param PC: pointer to current program counter
      * @param WriteData: pointer to data which is being written to IO device register
      * 
      * @retval none
      */
    Context_IPE(uint64_t *loadstore, uint64_t *PC, uint64_t *WriteData) 
    {
        this->loadstore = loadstore;
        this->PC = PC;
        this->writeData = WriteData;   
        this->prot = loadstore;
    }

    /** Destructor for Context class
      * @param none
      * 
      * @retval none
      */
    ~Context_IPE() 
    {
    }

    /** Gives the operation indicator whether operation is load or store
      * @param none
      * 
      * @retval operation indicator
      */
    uint64_t getLDST()
    {
        return *this->loadstore;     
    }
 
    /** Gives the current program counter
     * @param none
     * 
     * @retval current program counter
     */
    uint64_t getPc()
    {
        return *this->PC;
    }

    /** Gives the protection rights of currently accessed page
     * @param none
     * 
     * @retval protection rights of page
     */
    uint64_t getProt()
    {
        return *this->prot;
    }

    /** Gives data which is to be written to IO device register
     * @param none
     * 
     * @retval data for IO device register
     */
    uint64_t getWriteData()
    {
        return *this->writeData;
    }

    /** Sets Read/Write protection for the currently accessed page
     * @param prot: protections for currently accessed page
     * 
     * @retval none
     */
    void setProt(uint64_t prot)
    {
        *this->prot = prot;
    }
    
};

#endif	/* CONTEXT_IPE_H */

