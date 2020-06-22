#include "MMU_IPE.h"
#include "ExceptionHandler_IPE.h"

MMUTranslator_IPE::MMUTranslator_IPE()
{
}

/** Translate Guest Virtual Address to Guest Physical Address
  * @param core: Pointer to instance of processor
  * @param GVA: Guest Virtual Address
  * @param GPA: Pointer to location where Guest Physical Address must be plac
  * @param context: Pointer to context of current instruction
  * 
  * @retval true on success, false in case of exception
  */
uint64_t MMUTranslator_IPE::GVAtoGPA(Processor_IPE * core, uint64_t GVA, uint64_t* GPA, Context_IPE *context)
{
    uint64_t phys_addr = 0;
    uint32_t check_tlb = 0;
    
    if(verify_priviliges(core, GVA) == 0){
        
        printf("Address error: 0x%016llx\tCore %d\tLDST = %d\tPC = 0x%016lx\n",GVA,core->getCP0()->corenum(),context->getLDST(),core->getPC()); 
        *GPA = TLB_Exception_IPE::Perform_Exception(core->getCP0(),GVA,ADEL + context->getLDST(),context);        
        return 0;
    }
    
    uint32_t address_space = find_address_space_size(GVA);
    (address_space == BIT_32)\
            ?(check_tlb = look_staic_translation_32bit(core, GVA, &phys_addr))\
            :(check_tlb = look_staic_translation_64bit(core, GVA, &phys_addr));
    
    if(check_tlb == TLB_NOT_REQUIRED){
        *GPA = phys_addr;
        context->setProt(WRITE);
        return 1;
    }
    else{
        
        if (core->getTLB()->GVAtoGPA(GVA, GPA,context)){
            return 1;
        }
        return 0;
    }
}

/** Translate Guest Virtual Address to Guest Physical Address for file loading like kernel, u-boot etc.
  * @param GVA: Guest Virtual Address
  * @param GPA: Pointer to location where Guest Physical Address must be placed
  * 
  * @retval true on success, false in case of exception
  */
bool MMUTranslator_IPE::GVAtoGPA_for_fileloading(uint64_t GVA, uint64_t* GPA)
{
    bool result = false;
    uint32_t address_space = find_address_space_size(GVA);
    uint32_t segment_number = get_segment_number(GVA, address_space);
    if (address_space == BIT_32) 
    {
        if (segment_number == kseg1_32bit) 
        {
            *GPA = GVA & BIT32_ADDRESS_MASK;
            if (DBG)printf("GPA for file is 0x%016llx\n", *GPA);
            result = true;
        }
        else{
            printf("Required IO region out of kseg1.\n");
        }
    }
    else if (address_space == BIT_64) 
    {
        if (segment_number == xkphys_64bit) 
        {
            *GPA = GVA & BIT64_ADDRESS_MASK;
            if (DBG)printf("GPA for file is 0x%016llx\n", *GPA);
            result = true;
        } 
        else 
        {
            printf("Required IO region out of xkphys.\n");
        }
    }
    
    return result;
}

/** Verify the privileges of core whether it can access required segment in current processing mode
  * @param core: Pointer to instance of processor
  * @param addr: Guest Virtual Address
  * 
  * @retval true on success, false in case of un-privileged access
  */
uint64_t MMUTranslator_IPE::verify_priviliges(Processor_IPE * core, uint64_t addr)
{
    
    uint32_t mode = core->getCP0()->get_KSU();
    uint32_t address_space = find_address_space_size(addr);
    uint32_t segment_number = get_segment_number(addr, address_space);
    if(address_space == BIT_32)
    {
        if(mode == USER_MODE && segment_number != useg_32bit )
        {
            printf("Invalid address access from user mode 32-bit.\n");// ToDo: Exception handling
            return 0;
        }
        else if(mode == SUPERVISOR_MODE && (segment_number != sseg_32bit || segment_number != useg_32bit))
        {
            printf("Invalid address access from supervisor mode 32-bit.\n");// ToDo: Exception handling
            return 0;
        }
        else
            return 1;
    }
    else if (address_space == BIT_64)
    {
        if(mode == USER_MODE && segment_number != xuseg_64bit )
        {
            printf("Invalid address access from user mode 64-bit.\n");
            return 0;
        }
        else if(mode == SUPERVISOR_MODE && (segment_number != xsseg_64bit || segment_number != xuseg_64bit))
        {
            printf("Invalid address access from supervisor mode 64-bit.\n");
            return 0;
        }
        else if (segment_number == xuseg_64bit && addr > XUSEG_MAX_VALID_ADDR)
            return 0;
        else if (segment_number == xsseg_64bit && addr > XSSEG_MAX_VALID_ADDR)
            return 0;    
        else if (segment_number == xkseg_64bit && addr > XKSEG_MAX_VALID_ADDR)
            return 0; 
        else
            return 1;
    }
}

/** Find the address space whether it is 32-bit or 64-bit
  * @param addr: Guest Virtual Address
  * 
  * @retval address space size
  */
uint32_t MMUTranslator_IPE::find_address_space_size(uint64_t addr)
{
    uint32_t top32 = addr >> 32;
    if(top32 == 0xFFFFFFFF || top32 == 0x00000000)
    {
        //if(DBG)printf("32-bit address space.\n");
        return BIT_32;
    }
    else
    {
        //if(DBG)printf("64-bit address space.\n");
        return BIT_64;
    }
}

/** Find the segment number on basis of address and address space size
  * @param addr: Guest Virtual Address
  * @param address_space_size: Size of address space
  * 
  * @retval segment number
  */
uint32_t MMUTranslator_IPE::get_segment_number(uint64_t addr, uint32_t address_space_size)
{
    if(address_space_size == BIT_32)
    {
        uint32_t region = (addr >> 29) & 0x07;
        if(region < kseg0_32bit)
            return useg_32bit;
        else
            return region;
    }
    else if(address_space_size == BIT_64)
    {
        uint32_t region = addr >> 62;
        return region;
    }
}

/** Translate Guest Virtual Address to Guest Physical Address when address space is 32-bit.
  * Also checks if we need TLB for translation
  * @param core: Pointer to processor instance
  * @param Vaddr: Guest Virtual Address
  * @param Paddr: Pointer to location where Guest Physical Address must be placed
  * 
  * @retval 1 if TLB translation required otherwise 0
  */
uint32_t MMUTranslator_IPE::look_staic_translation_32bit(Processor_IPE * core, uint64_t Vaddr, uint64_t* Paddr){
    uint32_t segment_number = get_segment_number(Vaddr, BIT_32);
    
    if(segment_number == kseg0_32bit || segment_number == kseg1_32bit)
    {
        *Paddr = Vaddr & BIT32_ADDRESS_MASK;
        if(DBG)printf("Static translation provided.TLB not required in 32-bit mode.\n");
        return TLB_NOT_REQUIRED;
    }
    else if(segment_number == useg_32bit)
    {
        uint64_t status = core->getCP0()->read(H2_CP0_STATUS,SEL_ZERO);
        if((status & STATUS_ERL) == STATUS_ERL)
        {
            if(DBG)printf("ERL BIT is set in 32-bit mode.Address is direct mapped.\n");
            *Paddr = Vaddr & 0xFFFFFFFF;
            return TLB_NOT_REQUIRED;
        }
    }
    if(DBG)printf("TLB translation required in 32-bit mode.\n");
    return TLB_REQUIRED;
}

/** Translate Guest Virtual Address to Guest Physical Address when address space is 64-bit.
  * Also checks if we need TLB for translation
  * @param core: Pointer to processor instance
  * @param Vaddr: Guest Virtual Address
  * @param Paddr: Pointer to location where Guest Physical Address must be placed
  * 
  * @retval 1 if TLB translation required otherwise 0
  */
uint32_t MMUTranslator_IPE::look_staic_translation_64bit(Processor_IPE * core, uint64_t Vaddr, uint64_t* Paddr){
    uint32_t segment_number = get_segment_number(Vaddr, BIT_64);
    if(segment_number == xkphys_64bit)
    {
        *Paddr = Vaddr & BIT64_ADDRESS_MASK;
        if(DBG)printf("TLB translation not required in 64-bit mode.\n");
        return TLB_NOT_REQUIRED;
    }
    else if(segment_number == xuseg_64bit)
    {
        uint64_t status = core->getCP0()->read(H2_CP0_STATUS,SEL_ZERO);
        if((status & STATUS_ERL) == STATUS_ERL)
        {
            if(DBG)printf("ERL BIT is set in 64-bit mode.Address is direct mapped.\n");
            *Paddr = Vaddr & BIT64_ADDRESS_MASK;
            return TLB_NOT_REQUIRED;
        }
    }
    if(DBG)printf("TLB translation required in 64-bit mode.\n");
    return TLB_REQUIRED;
}
