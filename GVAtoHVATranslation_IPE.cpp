#include "GVAtoHVATranslation_IPE.h"
#include "Common.h"
#include "GPAtoHVATranslator_IPE.h"


bool GVAtoHVATranslator_IPE::GVAtoHVA(Processor_IPE* core, uint64_t GVA, uint64_t* HVA, Context_IPE *context)
{
    uint64_t GPA ;
    
    if(MMUTranslator_IPE::GVAtoGPA(core, GVA, &GPA, context) == 1)
    {
        
        *HVA = GPAtoHVATranslator_IPE::GPA_to_HVA(GPA);
        
        return true;
    }
    else
    {
        *HVA = GPA;
        
        return false;
    }
    
}

bool GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(uint64_t GVA, uint64_t *HVA)
{
    if(DBG)printf("start: GVAtoHVA_for_fileloading\n");
    uint64_t GPA;
    if(MMUTranslator_IPE::GVAtoGPA_for_fileloading(GVA, &GPA) == false)
        return false;
    
    //Assuming GPA is correct
    *HVA = GPAtoHVATranslator_IPE::GPA_to_HVA(GPA);
    if(DBG)printf("FIle loaded at HVA 0x%016lx\n",*HVA);
    if(DBG)printf("end:GVAtoHVA_for_fileloading\n");
    return true;
}