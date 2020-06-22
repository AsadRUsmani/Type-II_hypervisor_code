/* 
 * File:   Processor_IPE.cpp
 * Author: asadusmani
 * 
 * Created on June 5, 2014, 11:35 AM
 */

#include "Processor_IPE.h"
#include "BlockTranslation_IPE.h"
#include "GVAtoHVATranslation_IPE.h"
#include "Logger.h"
#include "ExceptionHandler_IPE.h"
#include "timer.h"

#include <pthread.h>
extern pthread_mutex_t lock;

Processor_IPE::Processor_IPE()
{
    
    //zero out guest data locations
    memset((void*)this->guestData, 0, sizeof(this->guestData));
    
    this->trapCount = 0;
    this->base = (uint64_t)this->guestData;
    this->retAddr = 0;
    this->prevRetAddr = 0;
    
    
    volatile uint64_t *pgpr  = (uint64_t *)(guestData + GP_REGS_OFFSET);
    volatile uint64_t *pcp0  = (uint64_t *)(guestData + CP0_REGS_OFFSET);
    volatile uint64_t *ppc   = (uint64_t *)(guestData + PROG_COUNTER_OFFSET);
    
    this->gpr = new GPR_IPE(pgpr);
    this->cp0 = new CP0_IPE(pcp0);
    this->tlb = new TLB_IPE(this->cp0);
    this->pc = ppc;
    
    *(this->pc) = MIPS_REST_VEC;
    
    this->controlMark = (uint64_t*)(guestData + CONTROL_FLAG_OFFSET);
    this->currentBlockStartAddr = (uint64_t*)(guestData + BLOCK_START_PC_OFFSET);
    this->currentBlockEndAddr   = (uint64_t*)(guestData + BLOCK_END_PC_OFFSET);
    this->Assembly2CAddress = (uint64_t*)(guestData + C_FUNC_CALL_OFFSET);
    this->tlbData = (uint64_t*)(guestData + LDST_ADDR_OFFSET);
    this->transAddr = (uint64_t*)(guestData + TRANS_ADDR_OFFSET);
    this->LDST = (uint64_t*)(guestData + LDST_OP_OFFSET);
    this->uartData = (uint64_t*)(guestData + LDST_DATA_OFFSET);
    this->LLBit = (uint64_t*)(guestData + LLBIT_OFFSET);
    
    // setting the request handling functions in array
    this->cases[0]  = &Processor_IPE::handleLDST;
    this->cases[1]  = &Processor_IPE::NewBlock;
    this->cases[2]  = &Processor_IPE::Syscall;
    this->cases[3]  = &Processor_IPE::TlbProbe;
    this->cases[4]  = &Processor_IPE::TlbRead;
    this->cases[5]  = &Processor_IPE::TlbWriteI;
    this->cases[6]  = &Processor_IPE::TlbWrite;
    this->cases[7]  = &Processor_IPE::HandleASIDchange;
    this->cases[8]  = &Processor_IPE::ExternalINT;
    this->cases[9]  = &Processor_IPE::CompareWrite;
    this->cases[10] = &Processor_IPE::CountWrite;
    this->cases[11] = &Processor_IPE::Log;
    this->cases[12] = &Processor_IPE::Handle_BR;
    this->cases[13] = &Processor_IPE::Handle_Trap;
    this->cases[14] = &Processor_IPE::Handle_RI;
    
    this->context = new Context_IPE((uint64_t*) this->LDST, (uint64_t*) this->pc, (uint64_t*) this->uartData);
    this->logGenerator = new Logger(this);
    this->iomanager = IOManager::getInstance();
    
    this->NMI = false;
    this->flag = false;
    this->Asid = 0x0;
    
    this->TIMER = new timer(this);
    
    this->CVMSEGBase = (uint64_t)this->CVMSEG;
    memset((void*)this->CVMSEG, 0, sizeof(this->CVMSEG));
}

Processor_IPE::~Processor_IPE() 
{
    delete this->tlb;
    delete this->cp0;
    delete this->gpr;
    delete this->context;
    delete this->logGenerator;
}

uint64_t Processor_IPE::getTrapCount()
{
    return this->trapCount;
}

void Processor_IPE::execute()
{
    
    this->C2Assembly2C();
    
}

void Processor_IPE::C2Assembly2C() 
{
    //Initialization
    *(this->controlMark) = FETCH_NEXT_BLOCK;
    *(this->pc) = MIPS_REST_VEC;
    if(this->cp0->corenum() != 0)
    {
        while(this->NMI == false) //Waiting to receive NMI pulse from CIU
            usleep(500);    //In actual, core is in low power mode until it gets NMI pulse
        printf("Started core %d\n",this->cp0->corenum());
        this->NMI = false;
    }
    *(this->currentBlockStartAddr) = 0;
    *(this->currentBlockEndAddr) = 0;
    
    //Following is a non standard way to get address of a label in gcc
    *(this->Assembly2CAddress) = (uint64_t)(&&AssemblyToC);
    
    printf("AssemblytoC addr = 0x%016llx\n",*(this->Assembly2CAddress));
        
    AssemblyToC: 
    {
//        asm volatile ("sd $12, %0" : "=m" (this->base));//save t4, which has base
        asm volatile ("sd $25, %0" : "=m" (this->retAddr));//save t9, which has ret address
               
        RequestHandling:
        {
            if(DBG){
                if(this->gpr->read(0) != 0x0){
                        printf("ALERT: Zero register corrupted\tPC = 0x%016lx\n",*(this->pc));
                        exit(1);
                }
                if((this->cp0->read(H2_CP0_CAUSE, SEL_ZERO) & (CAUSE_IP1 | CAUSE_IP0)) != 0x0){
                        printf("ALERT: Generated software interrupt but not handled\tPC = 0x%016lx\n",*(this->pc));
                        exit(1);
                }
            }
            
            if(!((this->*cases[*(this->controlMark)])())) P::EPRINT(REQUEST_NOTHANDLE_ERROR);
//            this->incrementTrapCount();
            if(DBG)printf("Jumping to Assembly %d\tRetAddr = 0x%016llx\n",this->cp0->corenum(),this->retAddr);
        }       
    }
    
    
    CToAssembly:
    {
        asm volatile ("ld $12, %0;" //restore t4 i.e. base
                      "ld $25, %1;" //restore t9, i.e return address
                      "jr $25;"
                      "nop;"
                      ::"m" (this->base), "m" (this->retAddr));       
    }
    
    while(true); //To tell gcc that it'll never go back from this function
    
}

void Processor_IPE::incrementTrapCount()
{
    if(this->trapCount++ > END_TRAP_COUNT)
    {
        printf("Core %d\tBlock PC = 0x%016llx\n",this->cp0->corenum(),*(this->pc));
        P::EPRINT(TRAP_COUNT_REACHED);
    }
}

//bool Processor_IPE::handleRequest()
//{
//    bool result = true;
//    if(DBG)printf("ControlMark = %d\t PC = 0x%016llx\tTrapCount = %d\tCoreId = %d\n",*(this->controlMark),*(this->pc),this->trapCount,this->cp0->corenum());
//    
//        switch (*(this->controlMark)){
//            case CHECK_TLB_DATA: {
//                if(DBG)printf("Handle Load store\n");
//                result = this->handleLDST();
//                break;
//             }
//
//            case FETCH_NEXT_BLOCK: 
//            {
//                if(*(this->pc) == 0x0){ 
//                    printf("Core %d\tBlock PC = 0x%016llx\tGVA = 0x%016llx\tTrap = %d\n",this->cp0->corenum(),*(this->currentBlockStartAddr),*(this->pc),this->trapCount);
//                    result = false;
//                }
//                 if(this->NMI) this->handleNMI();
//                 if(this->cp0->isInterruptPending()) this->executePendingInterrrupts();
//                 if(*(this->pc)!= *(this->currentBlockStartAddr)){
//                    this->fetchNextBlock();
//                 }
//                 else{
//                     this->retAddr = this->prevRetAddr; //Executing current block again
//                 }
////                if(this->flag) this->logGenerator->writeLogFile(*(this->pc));
//
//                 break;
//             }
//             case HANLDE_SYSCALL_INT: {
//                 if(DBG)printf("Sys Call! Core %d Trap = %d\n",this->cp0->corenum(),this->trapCount);
//                 *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), SCALL, this->context);
//                 *(this->controlMark) = FETCH_NEXT_BLOCK;
//                 this->fetchNextBlock();
//                 break;
//             } 
//            case HANLDE_TLB_PROBE: {
//                 if(DBG)printf("HANLDE_TLB_PROBE!\n");
//                 this->tlb->tlb_probe();
//                 if(DBG)printf("Index Register = 0x%016llx\n",this->tlb->get_index());
//                  break;
//             }
//             case HANLDE_TLB_READ: {
//                 if(DBG)printf("HANLDE_TLB_READ\n");
//                 this->tlb->tlb_read();
//                 break;
//             }
//            case HANLDE_TLB_WRITEI: {
//                 if(DBG)printf("HANLDE_TLB_WRITEI\n");
//                 this->tlb->tlb_write_index();
//                 this->addrCache.Clear_cache();
//                 break;
//             }
//             case HANLDE_TLB_WRITE: {
//                 if(DBG)printf("HANLDE_TLB_WRITE\n");
//                 this->tlb->tlb_random();
//                 this->addrCache.Clear_cache();
//                 break;
//             }
//            case HANDLE_ASID_CHANGE: {
//                if(this->Asid != (this->cp0->read(H2_CP0_ENTRYHI, SEL_ZERO) & ENTRYHI_ASID_MASK))
//                {
//                    this->addrCache.Clear_cache();
//                    this->Asid = (this->cp0->read(H2_CP0_ENTRYHI, SEL_ZERO) & ENTRYHI_ASID_MASK);
//                }
//                    
//                break;
//            }        
//            case HANLDE_EXTERNAL_INT: {
//                handlewait();
//                 break;
//             }
//            case HANDLE_Compare_Write:{
//                if(DBG) printf("Compare write\n");
//                this->TIMER->resetTimer();
//                break;
//            }
//            case HANDLE_count_read:{
//                this->TIMER->updateCounter();
//                break;
//            }
//             case HANDLE_count_write:{
//                if(DBG) printf("Count write\n");
//                this->TIMER->updateTimer();
//                break;
//            }
//            case EXP_RI: {
//                 printf("privileged instruction !\n pc %llx status %lx\n", *(this->pc),this->cp0->read(H2_CP0_STATUS, SEL_ZERO));
//                 *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), RI, this->context);
//                 *(this->controlMark) = FETCH_NEXT_BLOCK;
//                 this->fetchNextBlock();
//                 return true;
//                 break;
//             }
//             case WRITE_LOG_FILES: {
//                 if(DBG)printf("Write Log_Files = 0x%016llx\n",*(this->pc));
//                 this->logGenerator->writeLogFile(*(this->pc));
//                 break;
//             }
//             case HANLDE_TRAP_INT: {
//                 printf("trap !\n");
//                 *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), TRAP, this->context);
//                 *(this->controlMark) = FETCH_NEXT_BLOCK;
//                 this->fetchNextBlock();
//                 break;
//             }
//            case HANLDE_BR: {
//                 printf("break !\n");
//                 printf("PC = 0x%016lx\n",*(this->pc));
//                 *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), BRK, this->context);
//                 *(this->controlMark) = FETCH_NEXT_BLOCK;
//                 this->fetchNextBlock();
//                 break;
//             }
//             default:{
//                 printf("ControlMark = %d\tPC = 0x%016llx\tPrevPC = 0x%016llx\tTrap = %d\n",*(this->controlMark),*(this->pc),*(this->currentBlockStartAddr),this->trapCount);
//                 result = false;
//                 break;
//             }
//    
//        }
//    return result;
//}

//..............................................................................

bool Processor_IPE::NewBlock()
{    
        if(this->last_intrpt_time != 0){        //timer has started producing interrupts
        // checking if 3 seconds are passed without any timer interrupts, if yes than produce one    
                if ((this->last_intrpt_time + 3000000000) < (this->current_time())){ 
                printf("system froze\n");
                this->setTimerTick();
                }
        }
        
        if(this->cp0->isInterruptPending()) this->executePendingInterrrupts();
                
        if(*(this->pc) == 0x0){ 
               printf("Core %d\tBlock PC = 0x%016llx\tGVA = 0x%016llx\tTrap = %d\n",this->cp0->corenum(),*(this->currentBlockStartAddr),*(this->pc),this->trapCount);
               return false;
        }
         
        if(this->NMI) this->handleNMI();
                
        if(*(this->pc)!= *(this->currentBlockStartAddr))
                this->fetchNextBlock();
        else
                this->retAddr = this->prevRetAddr; //Executing current block again
                 
        return true;
}

bool Processor_IPE::Syscall()
{
        if(DBG)printf("Sys Call! Core %d Trap = %d\n",this->cp0->corenum(),this->trapCount);
        *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->cp0, *(this->pc), SCALL, this->context);
        *(this->controlMark) = FETCH_NEXT_BLOCK;
        this->fetchNextBlock();
        return true;
}

bool Processor_IPE::TlbWriteI()
{
        if(DBG)printf("HANLDE_TLB_WRITEI\n");
        this->tlb->tlb_write_index();
        this->addrCache.Clear_cache();
        return true;
}

bool Processor_IPE::TlbProbe()
{
        if(DBG)printf("HANLDE_TLB_PROBE!\n");
        this->tlb->tlb_probe();
        if(DBG)printf("Index Register = 0x%016llx\n",this->tlb->get_index());
        return true;        
}

bool Processor_IPE::TlbRead()
{
        if(DBG)printf("HANLDE_TLB_READ\n");
        this->tlb->tlb_read();
        return true;
}

bool Processor_IPE::TlbWrite()
{
        if(DBG)printf("HANLDE_TLB_WRITE\n");
        this->tlb->tlb_random();
        this->addrCache.Clear_cache();
        return true;
}

bool Processor_IPE::HandleASIDchange()
{
        if(this->Asid != (this->cp0->read(H2_CP0_ENTRYHI, SEL_ZERO) & ENTRYHI_ASID_MASK)){
          this->addrCache.Clear_cache();
          this->Asid = (this->cp0->read(H2_CP0_ENTRYHI, SEL_ZERO) & ENTRYHI_ASID_MASK);
        }
        return true;
}

bool Processor_IPE::ExternalINT()
{
        handlewait();
        return true;
}

bool Processor_IPE::CompareWrite()
{
        if(DBG) printf("Compare write\n");
        this->TIMER->resetTimer();
        return true;
}

bool Processor_IPE::CountWrite()
{
        if(DBG) printf("Count write\n");
        this->TIMER->updateTimer();
        return true;
}

bool Processor_IPE::Handle_BR()
{
        printf("break !\n");
        printf("PC = 0x%016lx\n",*(this->pc));
        *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), BRK, this->context);
        *(this->controlMark) = FETCH_NEXT_BLOCK;
        this->fetchNextBlock();
        return true;
}

bool Processor_IPE::Handle_Trap()
{
        printf("trap !\n");
        *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), TRAP, this->context);
        *(this->controlMark) = FETCH_NEXT_BLOCK;
        this->fetchNextBlock();
        return true;
}

bool Processor_IPE::Handle_RI()
{
        printf("privileged instruction !\n pc %llx status %lx\n", *(this->pc),this->cp0->read(H2_CP0_STATUS, SEL_ZERO));
        *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), RI, this->context);
        *(this->controlMark) = FETCH_NEXT_BLOCK;
        this->fetchNextBlock();
        return true;
}

bool Processor_IPE::Log()
{
        if(DBG)printf("Write Log_Files = 0x%016llx\n",*(this->pc));
        this->logGenerator->writeLogFile(*(this->pc));
        return true;
}

/*
 Fetch new block of instructions either by fresh translation or from cache
 */

void Processor_IPE::fetchNextBlock()
{
    uint64_t HVA;
    uint64_t localGVA = *(this->pc);
    CacheVal *cache = this->cacheBlk.doesExists(localGVA, this->Asid);
    
    if(cache != NULL)//found in translation cache
    {
        *(this->currentBlockStartAddr) = localGVA;
        *(this->currentBlockEndAddr) = cache->blockEndPC;
        
        this->retAddr = (uint64_t)cache->translatedBlock->data();
        this->prevRetAddr = this->retAddr;
//        this->base = (uint64_t)this->guestData;
    }else {//do new translation    
        uint64_t blockEndAddr;
        if(DBG)printf("Translating new block\n");
        *this->LDST = READ;
        if(this->addrTranslation(localGVA, &HVA) == false)
        {
            localGVA = HVA;
            *(this->pc) = localGVA;
            this->addrTranslation(localGVA, &HVA);
        }
        
        *(this->currentBlockStartAddr) = localGVA;
        std::vector<IInfo>* instVec = this->instBlk.getBlock((uint32_t*)HVA);
        *(this->currentBlockEndAddr) = localGVA 
                                    +((instVec->size()-DELAYSLOTINS)*INS_SIZE);
        blockEndAddr = *(this->currentBlockEndAddr);
        
        TranslationBlock *transInst = this->cacheBlk.getContainer(localGVA);
        
        if(this->cp0->get_KSU() != 0)
        {
            if((*(this->currentBlockStartAddr) & ~(0x0FFF)) != (blockEndAddr & ~(0x0FFF))) // Block span over multiple pages
            {
                this->fixBlock(instVec, localGVA, blockEndAddr);
            }
        }
        
        BlockTranslation_IPE::doBlockTranslation(transInst, instVec, blockEndAddr);
        this->cacheBlk.putInCache(localGVA, blockEndAddr, this->Asid);
        
      if(DBG_BLOCK)this->logGenerator->writeInsLog(instVec, transInst, localGVA);
        
//        if(this->flag)this->logGenerator->writeInsLog(instVec, transInst, localGVA);
        
        this->retAddr = (uint64_t)transInst->data();
        this->prevRetAddr = this->retAddr;
//        this->base = (uint64_t)this->guestData;       
    }
}
//..............................................................................
    
    bool Processor_IPE::handleLDST()
    {
        uint64_t localtlbData = *(this->tlbData);
         
        uint64_t HVA;
         if(this->iomanager->isIOaddr(localtlbData))
         {
             this->iomanager->GVAtoHVA(localtlbData, &HVA, this->context);
             
             *(this->transAddr) = HVA;
         }   
         else if (this->addrTranslation(localtlbData, &HVA) == true) 
         {
             *(this->transAddr) = HVA;
         }
         else
         {
             if (HVA == 0x0) return false;
             
             if(DBG) printf("ControlMark = %d\tPC = 0x%016llx\tLDST Addr = 0x%016llx\tTrapCount = %d\tCoreId = %d\n",*(this->controlMark),*(this->pc),localtlbData,this->trapCount,this->cp0->corenum());
             *(this->LLBit) = 0x0;     //Clearing LLbit
             *(this->pc) = HVA;
//             if(this->flag) this->logGenerator->writeLogFile(*(this->pc));
             this->fetchNextBlock();
         }
         
         return true;        
    }
    
    bool Processor_IPE::addrTranslation(uint64_t GVA, uint64_t* HVA)
    {
        bool result = this->addrCache.Check_cache(GVA, HVA, this->context);
        if (result == false)
        {
            if((GVA & CVMSEG_MASK) == CVMSEG_BASE_ADDR)
            {
                uint64_t offset = GVA & ~CVMSEG_MASK;
                *HVA = this->CVMSEGBase + offset;
                if(DBG) printf("CVMSEG access GVA = 0x%lx\tHVA = 0x%lx\n",GVA,*HVA);
                result = true;
            }
            else
            if (GVAtoHVATranslator_IPE::GVAtoHVA(this, GVA, HVA, this->context) == true)
            {
                this->addrCache.Update_cache(GVA, *HVA, this->context->getProt());
                result = true;
            }
        }
        return result;
    }
    
    void Processor_IPE::fixBlock(std::vector<IInfo>* instVec, uint64_t blockStartAddress, uint64_t blockEndAddr)
    {
        if(DBG)printf("Block span over multiple pages.Addr = 0x%016lx\n",blockStartAddress);
        int validinstr =  (blockEndAddr & ~(0x0FFF)) - blockStartAddress;
        validinstr /= INS_SIZE;
        uint64_t size = instVec->size();
        instVec->erase(instVec->begin() + validinstr, instVec->end());
            
        if((size - validinstr) > 1)
        {   
            IInfo terminate(TERMINATION_INS);
            instVec->push_back(terminate); //Adding Block Termination instruction
            instVec->push_back(IInfo(0));
        }
        else
        {   //  Fix delay slot
            uint64_t HVA;
            if(this->addrTranslation(blockEndAddr & ~(0x0FFF), &HVA) == false)
            {
//                localGVA = HVA;
//                *(this->pc) = localGVA;
//                this->addrTranslation(localGVA, &HVA);
                printf("Exception in fetching delay slot. Need to be implemented.\n");
            }
            IInfo instr(*(uint32_t*)HVA);
            instVec->push_back(instr);
        }
    }
    
    uint64_t Processor_IPE::handleSigFPE()
    {
        *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), OV, this->context);
        *(this->controlMark) = FETCH_NEXT_BLOCK;
        this->fetchNextBlock();
        
        return this->retAddr;
    }
    
    void Processor_IPE::setNMI()
    {
        this->NMI = true;
    }
    
    void Processor_IPE::handleNMI()
    {
        uint64_t status = this->cp0->read(H2_CP0_STATUS, SEL_ZERO);
        status |= 0x00480004; //Setting ERL,BEV and NMI bits
        status &= 0xFFCFFFFF; //Clearing TS and SR bits
        this->cp0->write(status, H2_CP0_STATUS, SEL_ZERO);
        
        *(this->pc) = MIPS_REST_VEC;
        *(this->controlMark) = FETCH_NEXT_BLOCK;
        *(this->currentBlockStartAddr) = 0;
        *(this->currentBlockEndAddr) = 0;
        
        this->NMI = false;
    }
    
    void Processor_IPE::handlewait()
    {
        while(!this->cp0->isInterruptPending())
            usleep(300);
        
        *(this->pc) += 4;
        *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), 0x0, INT , this->context);
        this->fetchNextBlock();
    }
    
    void Processor_IPE::setTimerTick(){
        //setting IP7 for hardware interrupt and TI bit for timer interrupt
        
        uint64_t cause = this->cp0->read(H2_CP0_CAUSE, SEL_ZERO);
        cause |= (CAUSE_TI | CAUSE_IP7);
        this->cp0->write(cause, H2_CP0_CAUSE, SEL_ZERO);
        this->last_intrpt_time = this->current_time();
    }
    
    void Processor_IPE::executePendingInterrrupts()
    {
        *(this->pc) = TLB_Exception_IPE::Perform_Exception(this->getCP0(), *(this->pc), INT, this->context);
    }
    
    uint64_t Processor_IPE::current_time()
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint64_t)(ts.tv_sec * 1000000000LL + ts.tv_nsec);
    }
