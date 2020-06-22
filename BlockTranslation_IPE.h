/* 
 * File:   BlockTranslation_IPE.h
 * Author: asadusmani
 *
 * Created on June 5, 2014, 4:08 PM
 */

#ifndef BLOCKTRANSLATION_IPE_H
#define	BLOCKTRANSLATION_IPE_H

#include "InstBlock.h"
#include "TranslationBlock.h"

#define MIN_TRANS_INSTR_SIZE       2
#define MAX_TRANS_INSTR_SIZE       INITIALBLOCKSIZE/4 

class BlockTranslation_IPE {
public:
    BlockTranslation_IPE();
    BlockTranslation_IPE(const BlockTranslation_IPE& orig);
    virtual ~BlockTranslation_IPE();
    static void doBlockTranslation(TranslationBlock* , std::vector<IInfo>* , uint64_t);
private:

};


#endif	/* BLOCKTRANSLATION_IPE_H */

