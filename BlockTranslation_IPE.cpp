#include "P.h"
#include <vector>
#include <bits/stl_vector.h>

#include "BlockTranslation_IPE.h"
#include "Translation_IPE.h"
//#include "InstrucTranslation_IPE.h"

InsDecoding instrObj_IPE;

BlockTranslation_IPE::BlockTranslation_IPE() {
}

BlockTranslation_IPE::BlockTranslation_IPE(const BlockTranslation_IPE& orig) {
}

BlockTranslation_IPE::~BlockTranslation_IPE() {
}

void BlockTranslation_IPE::doBlockTranslation(TranslationBlock *allInstrcVec , std::vector<IInfo>*  vec , uint64_t blockEndPC){
    unsigned int sizeVec = 0, i;
    
    sizeVec = vec->size();
    if(sizeVec < MIN_TRANS_INSTR_SIZE)    P::EPRINT(MIN_TRANS_BLOCK_ERROR);
    
    for( i = 0 ; i < sizeVec-MIN_TRANS_INSTR_SIZE; i++ ){
        if(LOG_GEN)Translation_IPE::add_Log(*allInstrcVec);
        instrObj_IPE.findInstrCase(&vec->at(i) ,NULL , *allInstrcVec , blockEndPC);
        Translation_IPE::update_pc(*allInstrcVec);
    }

    if(LOG_GEN)Translation_IPE::add_Log(*allInstrcVec);
    instrObj_IPE.findInstrCase(&vec->at(sizeVec-MIN_TRANS_INSTR_SIZE) ,&vec->at(sizeVec-MIN_TRANS_INSTR_SIZE+1) , *allInstrcVec , blockEndPC);
    
    /*transBlockSize = (*allInstrcVec).size();
    if(transBlockSize >= MAX_TRANS_INSTR_SIZE) {
        printf("size =%ld\n",transBlockSize);
        P::EPRINT(MAX_TRANS_BLOCK_ERROR);}
	*/
}
