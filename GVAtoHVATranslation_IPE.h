/* 
 * File:   GVAtoHVATranslation_IPE.h
 * Author: asadusmani
 *
 * Created on June 6, 2014, 4:23 PM
 */

#ifndef GVATOHVATRANSLATION_IPE_H
#define	GVATOHVATRANSLATION_IPE_H

#include "MMU_IPE.h"
#include "Context_IPE.h"

class GVAtoHVATranslator_IPE
{
public:
    
    static bool GVAtoHVA(Processor_IPE * core, uint64_t GVA, uint64_t* HVA, Context_IPE *context);
    static bool GVAtoHVA_for_fileloading(uint64_t GVA, uint64_t *HVA);
    
private:
    
};

#endif	/* GVATOHVATRANSLATION_IPE_H */

