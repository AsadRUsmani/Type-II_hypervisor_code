/* 
 * File:   CodeExecutor.cpp
 * Author: asadusmani
 * 
 * Created on June 4, 2014, 4:50 PM
 */

#include "CodeExecutor.h"
#include "BlockTranslation_IPE.h"
#include "GVAtoHVATranslation_IPE.h"

CodeExecutor::CodeExecutor() {
    //zero out guest data locations
    memset((void*)this->guestData, 0, sizeof(this->guestData));
    
    this->trapCount = 0;
    this->base = 0;
    this->retAddr = 0;
    this->prevRetAddr = 0;
    this->flag = 0;
    
    
    volatile uint64_t *gpr  = (uint64_t *)(guestData + GP_REGS_OFFSET);
    volatile uint64_t *cp0  = (uint64_t *)(guestData + CP0_REGS_OFFSET);
    volatile uint64_t *pc   = (uint64_t *)(guestData + PROG_COUNTER_OFFSET);
    
//    this->systemMainBoard = new Board_IPE(gpr, cp0, pc);
    
    this->controlMark = (uint64_t*)(guestData + CONTROL_FLAG_OFFSET);    
    this->GVA         = (uint64_t*)(guestData + NEXT_BLOCK_OFFSET);
    this->currentBlockStartAddr = (uint64_t*)(guestData + BLOCK_START_PC_OFFSET);
    this->currentBlockEndAddr   = (uint64_t*)(guestData + BLOCK_END_PC_OFFSET);
    this->Assembly2CAddress = (uint64_t*)(guestData + C_FUNC_CALL_OFFSET);
    this->tlbData = (uint64_t*)(guestData + LDST_ADDR_OFFSET);
    this->LDST = (uint64_t*)(guestData + UART_LDST_OFFSET);
    this->uartData = (uint64_t*)(guestData + UART_REG_OFFSET);
    
    
    
    
    
    
    /*
    
    execPtr      = (uint32_t *)(execBlockAddr + EXEC_BLOCK_OFFSET);
    
    
    nxtPCrData   = (uint64_t *)(execBlockAddr + TLB_DATA_OFFSET);
    blockEndPC   = (uint64_t *)(execBlockAddr + BLOCK_END_PC_OFFSET);
    blockStartPC = (uint64_t *)(execBlockAddr + BLOCK_START_PC_OFFSET);
    cntrlMarkAddr= (uint64_t *)(execBlockAddr + CONTROL_FLAG_OFFSET);
    
    
    Base_Addr = execBlockAddr;
    BlockManipulation::setInitBlock((uint32_t *)execBlockAddr);
    BlockManipulation::setUContext(UBOOT_LOAD_ADDRESS, FETCH_NEXT_BLOCK);
    */
    
   
}


CodeExecutor::~CodeExecutor() 
{
    delete this->systemMainBoard;
}

void CodeExecutor::execute()
{
    uint64_t bootbus;
    if(GVAtoHVATranslator_IPE::GVAtoHVA_for_fileloading(0xB0000000, &bootbus) == false)
    {
        printf("Unable to get bootbus addr.\n");
        exit(1);
    }
    this->context.setBootBus(bootbus);
    this->C2Assembly2C();
}

void CodeExecutor::C2Assembly2C() 
{
    //Initialization
    *(this->controlMark) = FETCH_NEXT_BLOCK;
    *(this->GVA) = MIPS_REST_VEC;
    *(this->currentBlockStartAddr) = 0;
    *(this->currentBlockEndAddr) = 0;
    
    //Following is a non standard way to get address of a label in gcc
    *(this->Assembly2CAddress) = (uint64_t)(&&AssemblyToC);
    
    printf("AssemblytoC addr = 0x%016llx\n",*(this->Assembly2CAddress));
    
    
      
    AssemblyToC: 
    {
        asm volatile ("sd $12, %0" : "=m" (this->base));//save t4, which has base
        asm volatile ("sd $25, %0" : "=m" (this->retAddr));//save t9, which has ret address
        
//        this->getControlMark();
        
        RequestHandling:
        {
            if(!(this->handleRequest())) P::EPRINT(REQUEST_NOTHANDLE_ERROR);
            this->incrementTrapCount();
        }       
    }
        
        
    
    
    CToAssembly:
    {
        asm volatile ("ld $12, %0" : "=m" (this->base));//restore t4 i.e. base
        asm volatile ("ld $25, %0" : "=m" (this->retAddr));//restore t9, i.e return address
        
       
        
        asm volatile ("jr $25\n\t");
        asm volatile ("nop   \n\t");        
    }
    
}

void CodeExecutor::getControlMark()
{
    if(flag ==0)
        this->base = (uint64_t)this->guestData;
    asm volatile ("ld $12, %0" : "=m" (this->base));
    asm volatile ("ld $13, 104($12)");
    asm volatile ("sd $13, %0" : "=m" (this->controlMark1));
}

void CodeExecutor::incrementTrapCount()
{
    if(this->trapCount++ > END_TRAP_COUNT)
    {
        P::EPRINT(TRAP_COUNT_REACHED);
    }
}

bool CodeExecutor::handleRequest()
{
    bool result = true;
    if(DBG)printf("ControlMark = %d\t GVA = 0x%016llx\tTrapCount = %d\n",*(this->controlMark),*(this->GVA),this->trapCount);
    
    switch (*(this->controlMark))
    {
        case FETCH_NEXT_BLOCK: 
        {
            if(DBG)printf("Fetch new Block\n");
             if(*(this->GVA) == 0x0) result = false;
             if(*(this->GVA)!= *(this->currentBlockStartAddr))
                this->fetchNextBlock();
             else
                 this->retAddr = this->prevRetAddr;
             
             break;
         }
        
        case CHECK_TLB_DATA: {
            if(DBG)printf("Handle Load store\n");
            result = this->handleLDST();
            break;
         }
        
        case HANLDE_TLB_PROBE: {
             if(DBG)printf("HANLDE_TLB_PROBE!\n");
             this->systemMainBoard->getCore0()->getTLB()->tlb_probe();
             if(DBG)printf("Index Register = 0x%016llx\n",this->systemMainBoard->getCore0()->getTLB()->get_index());
              break;
         }
         case HANLDE_TLB_READ: {
             if(DBG)printf("HANLDE_TLB_READ\n");
             this->systemMainBoard->getCore0()->getTLB()->tlb_read();
             break;
         }
         case HANLDE_TLB_WRITE: {
             if(DBG)printf("HANLDE_TLB_WRITE\n");
             this->systemMainBoard->getCore0()->getTLB()->tlb_random();
             break;
         }
         case HANLDE_TLB_WRITEI: {
             if(DBG)printf("HANLDE_TLB_WRITEI\n");
             this->systemMainBoard->getCore0()->getTLB()->tlb_write_index();
             break;
         }
         default:{
             printf("ControlMark = %d\tGVA = 0x%016llx\tLDST_ADDR = 0x%016llx\tPrevPC = 0x%016llx\tTrap = %d\n",*(this->controlMark),*(this->GVA),*(this->tlbData),*(this->currentBlockStartAddr),this->trapCount);
             result = false;
             break;
         }
    
    }
    
    return result;
}
//..............................................................................
/*
 Fetch new block of instructions either by fresh translation or from cache
 */

void CodeExecutor::fetchNextBlock()
{
//    printf("ControlMark = %d\t GVA = 0x%016llx\tTrapCount = %d\n",*(this->controlMark),*(this->GVA),this->trapCount);
    uint64_t HVA;
    if(GVAtoHVATranslator_IPE::GVAtoHVA(this->systemMainBoard->getCore0(),
                                        *(this->GVA), 
                                        &HVA,
                                        this->context) != true)
    {
                P::EPRINT(HVA_NOTFOUND_ERROR);
    }
    
    if(false)
    {
        //finding in cache case
    }   
    

    else
    {
        *(this->currentBlockStartAddr) = *(this->GVA);
        std::vector<IInfo>* instVec = this->instBlk.getBlock((uint32_t*)HVA);
        *(this->currentBlockEndAddr) = *(this->currentBlockStartAddr) 
                                    +((instVec->size()-2)*4);
        
        
        
        
        int blkSize = 0;
        std::vector<uint32_t>* transInst = BlockTranslation_IPE::doBlockTranslation(instVec, blkSize, *(this->currentBlockEndAddr));
        
        this->retAddr = (uint64_t)transInst->data();
        this->prevRetAddr = this->retAddr;
        this->base = (uint64_t)this->guestData;
        
        
        //place translated block of instructions into cache
//        cacheblk.putInCache((void *)currPC, (void *)blkptr, blksize*4, currBlockEndPC);
        
       
        
    }
}
//..............................................................................
    
    bool CodeExecutor::handleLDST()
    {
        bool result = true;
//        printf("LDST = %d\tStartPC = 0x%016llx\tData = %c\n",*(this->guestData + UART_LDST_OFFSET),*(this->currentBlockStartAddr),*((uint64_t*)(this->guestData + UART_REG_OFFSET)));
        this->context.setData(*(this->LDST),//*(this->guestData + UART_LDST_OFFSET),
                               *(this->currentBlockStartAddr),//Variable name should be Prev_block_PC
                               char(*(this->uartData)),//(char)*((uint64_t*)(this->guestData + UART_REG_OFFSET)),
                               false);
         
         uint64_t HVA;
         if (GVAtoHVATranslator_IPE::GVAtoHVA(this->systemMainBoard->getCore0(),
                                              *(this->tlbData), 
                                              &HVA, 
                                              this->context) == true)
         {
             *(this->tlbData) = HVA;
         }
         else
         {
             //Exception handling code will come here
             result = false;
         }
             
             
         
         return result;        
    }
    



