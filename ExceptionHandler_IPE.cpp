#include "ExceptionHandler_IPE.h"


/** Configures Status and Cause registers and get exception entry point
  * @param cp0: pointer to CP0
  * @param GVA: Virtual address which caused exception
  * @param exp_type: Type of exception
  * @param context: Pointer to context of CPU at instance exception occurred
  * 
  * @retval Address to exception routine
  */
uint64_t TLB_Exception_IPE::Perform_Exception(CP0_IPE* cp0, uint64_t GVA, int exp_type, Context_IPE * context){
   
    int cause_ExcCode=-1;
    uint64_t cause = cp0->read(H2_CP0_CAUSE, SEL_ZERO);
    uint64_t status = cp0->read(H2_CP0_STATUS, SEL_ZERO);
    uint64_t ebase = cp0->read(H2_CP0_EBASE_1, SEL_ONE); 
    uint64_t new_pc=0;
    
    switch(exp_type){
        case INT:
        case TLB_MOD:
        case TLBL:
        case TLBS:
        case ADEL:
        case ADES:
        case SCALL:
        case BRK:
        case RI:
        case CPU:
        case OV:
        case TRAP:
            cause_ExcCode = exp_type;
            break;
            
        case TLB_INV:
            cause_ExcCode = TLBL + context->getLDST();
            break;
            
        default:
            printf(" Exception not handled\n");
            exit(1);  
    }
//    if( cause_ExcCode == ADEL || cause_ExcCode == ADES)
        cp0->write(GVA, H2_CP0_BADVADDR, SEL_ZERO);
        if(cp0->get_KSU() == USER_MODE)
            if(!(status & STATUS_UX))
                printf("ALERT: User mode with extended addressing disabled\n");
    /*Setting cause register*/
    if(DBG)printf("ex_code %lx and c %d \n", exp_type, cause_ExcCode);
    cause &= ~(CAUSE_EXEC_MASK);
    cause |= (cause_ExcCode << CAUSE_EXEC_OFFSET);
    cp0->write(cause, H2_CP0_CAUSE, SEL_ZERO);
    
    if (status & STATUS_BEV) { //In booting
        if (((exp_type == TLBL)||(exp_type == TLBS)) && (!(status & STATUS_EXL)))
            new_pc = (int32_t)(EXCEPTION_VETOR + XTLB_REFILL_OFFSET);        
        else 
            if((exp_type == INT) && (cause & CAUSE_IV) )
                new_pc = (int32_t)(EXCEPTION_VETOR + INTERRUPT_OFFSET);
        else
            new_pc = (int32_t)(EXCEPTION_VETOR + GENERAL_EXECPTION_OFFSET);
        
    }     
    else{ //Not in booting
         if (( exp_type == TLBL || exp_type == TLBS ) && (!(status & STATUS_EXL)))
            new_pc = (int32_t)((ebase & ~EBASE_CORENUM_MASK) + XTLB_REFILL_OFFSET);
        else 
            if((exp_type == INT) && (cause & CAUSE_IV) )
                new_pc = (int32_t)((ebase & ~EBASE_CORENUM_MASK) + INTERRUPT_OFFSET) ;
        else
            new_pc = (int32_t)((ebase & ~EBASE_CORENUM_MASK) + GENERAL_EXECPTION_OFFSET) ;  // general entry point    
    } 
    
    if (!(status & STATUS_EXL)){ // checking exl bit is not set
        cp0->write(context->getPc(), H2_CP0_EPC, SEL_ZERO);  //setting epc
       status |= STATUS_EXL;      //setting exl bit in status
    }else{              //if set
        printf("TLB exception when EXL bit is set\n");
        status |= STATUS_ERL;      //setting erl bit in status
        cp0->write(context->getPc(), H2_CP0_ERROREPC, SEL_ZERO);  //setting epc
    }
    cp0->write(status, H2_CP0_STATUS, SEL_ZERO); 
    if(DBG)printf("new PC %llx \n", new_pc);
    return new_pc;
    
}

/** Configures CP0 register required for exception handling
  * @param cp0: pointer to CP0
  * @param GVA: Virtual address which caused exception
  * @param exp_type: Type of exception
  * @param context: Pointer to context of CPU at instance exception occurred
  * 
  * @retval Address to exception routine
  */
uint64_t TLB_Exception_IPE::TLB_Excep(CP0_IPE* cp0, uint64_t GVA, uint32_t exp_type, Context_IPE * context){

    uint64_t XContext = cp0->read(H2_CP0_XCONTEXT, SEL_ZERO) & XCONTEXT_PTEBASE_MASK;
    uint64_t Context = cp0->read(H2_CP0_CONTEXT, SEL_ZERO) & CONTEXT_PTEBASE_MASK;
    uint64_t entryHi = cp0->read(H2_CP0_ENTRYHI, SEL_ZERO) & ENTRYHI_ASID_MASK;
    uint64_t region = GVA & (REGION_MASK << REGION_OFFSET);
    uint64_t VPN = (GVA & ~MIN_PAGE_MASK) & SEG_MASK;
    
    //Setting EntryHi
    entryHi |= (VPN | region);
    cp0->write(entryHi, H2_CP0_ENTRYHI, SEL_ZERO);
    
    //Setting XContext
    XContext |= (region >> (REGION_OFFSET - XCONTEXT_REGION)) | (VPN >> (ENTRYHI_VPN - XCONTEXT_VPN));
    cp0->write(XContext, H2_CP0_XCONTEXT, SEL_ZERO);
    
    //Setting Context
    Context |= (VPN >> (ENTRYHI_VPN - CONTEXT_VPN)) & CONTEXT_VPN_MASK;
    cp0->write(Context, H2_CP0_CONTEXT, SEL_ZERO);
    
   return Perform_Exception(cp0, GVA, exp_type,context);
}

