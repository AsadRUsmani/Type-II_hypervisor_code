/* 
 * File:   InstBlock.h
 * Author: asadusmani
 * Created on February 12, 2014, 11:51 AM
 */

#ifndef INSTBLOCK_H
#define	INSTBLOCK_H

#include <vector>
#include "Common.h"
#include "IInfo.h"

class InstBlock {

public:
	std::vector<IInfo>* getBlock(uint32_t *pc);         /*Fills instructions in this block*/
//	void appendBlock(uint32_t *pc, std::vector<IInfo>*);
        unsigned int getBlockSize();
        void printBlock(std::vector<IInfo>*);               /*Prints instructions of this block*/

private:
    std::vector<IInfo> instructions;

};

#endif	/* INSTBLOCK_H */

