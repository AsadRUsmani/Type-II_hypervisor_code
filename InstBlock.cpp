/* 
 * File:   InstBlock.cpp
 * Author: asadusmani
 * Created on February 12, 2014, 11:51 AM
 */

#include "InstBlock.h"


std::vector<IInfo>* InstBlock::getBlock(uint32_t *pc){

    uint32_t *pcptr = pc;
	//start with a clean slate
	this->instructions.clear();
	while(true)
	{
		IInfo anInstruction(*pcptr);
		instructions.push_back(anInstruction);
                pcptr++;
		if(anInstruction.isControlInst)
		{
                    IInfo delayslot(*pcptr);
                    instructions.push_back(delayslot);//delay slot
                    break;
		}
	}//end of while
	return &(this->instructions);
}

//void InstBlock::appendBlock(uint32_t *pc, std::vector<IInfo>* instBlk){
//
//    uint32_t *pcptr = pc;
//    
//	while(true)
//	{
//		IInfo anInstruction(*pcptr);
//		pcptr++;
//		anInstruction.processInst();
//		instBlk->push_back(anInstruction);
//
//		if(anInstruction.isCntrlInst())
//		{
//			instBlk->push_back(IInfo(*pcptr));//delay slot
//			break;
//		}
//	}//end of while
//}

void InstBlock::printBlock(std::vector<IInfo>* blk) {    
    long i = 0;    
    int sz = blk->size();
    printf("==========\n");
    for (i = 0; i < sz; i++)        
        printf("%d: 0x%08lx \n",i, blk->at(i).encoding);
    }

unsigned int InstBlock::getBlockSize()
{
	return this->instructions.size();
}

