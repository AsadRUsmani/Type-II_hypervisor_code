/* 
 * File:   CIU_New.h
 * Author: usamanwar
 *
 * Created on 21 May 2015, 17:34
 */
#include "CIU.h"
#include "SystemBoard_IPE.h"


/** Initializes instance of CIU
  * @param processor: Pointer to array of Processor object
  * @param numcores: Number of cores
  * 
  * @retval none
  */
CIU::CIU(Processor_IPE *processor, int numcores)
{
    IOManager* manager = IOManager::getInstance();
    printf("Initializing CIU........\n");
    this->baseAddr = manager->getDeviceHVA(CIU_ID);
    this->processors = processor;
    this->numcores = numcores;
    this->MessageBox = manager->getMessageBox(CIU_ID);
}

/** Set all CIU registers pointer to actual memory mapped region for CIU
  * @param none
  * 
  * @retval none
  */
void CIU::setMappings()
{
    uint64_t *CIUAddr = (uint64_t*) this->baseAddr;
    uint64_t *regBaseAddr;
    int i;
    
    //  CIU_INT_SUM0_EN0 *INT_SUM0[24];
    for (i = 0; i < 24; i++){
        this->INT_SUM0[i] = (CIU_INT_SUM0_EN0*)(CIUAddr + i);
    }
    
    //  CIU_INT_SUM0_EN0 *INT_EN0[24];
    regBaseAddr = CIUAddr + CIU_INT0_EN0_OFFSET_NEW;
    for (i = 0; i < 24; i++) 
    {          
        this->INT_EN0[i] = (CIU_INT_SUM0_EN0*)(regBaseAddr + (i * 2));       
    } 
    
    //    CIU_INT_SUM0_EN0 *INT_SUM4[12];
    regBaseAddr = CIUAddr + CIU_INT0_SUM4_OFFSET_NEW;    
    for (i = 0; i < 12; i++) {
        this->INT_SUM4[i] = (CIU_INT_SUM0_EN0*)(regBaseAddr + i);       
    }
    
    //    CIU_INT_SUM0_EN0 *INT_EN4_0[12];
    regBaseAddr = CIUAddr + CIU_INT0_EN4_0_OFFSET_NEW;    
    for (i = 0; i < 12; i++) {
        this->INT_EN4_0[i] = (CIU_INT_SUM0_EN0*)(regBaseAddr + i);       
    }
    
    //    CIU_INT_MBOX_SET *INT_MBOX_SET[12];
    regBaseAddr = CIUAddr + CIU_MBOX_SET0_OFFSET_NEW;    
    for (i = 0; i < 12; i++) {
        this->INT_MBOX_SET[i] = (CIU_INT_MBOX_SET*)(regBaseAddr + i);       
    }
    
    //    INT_MBOX_CLR[12];
    regBaseAddr = CIUAddr + CIU_MBOX_CLR0_OFFSET_NEW;    
    for (i = 0; i < 12; i++) {
        this->INT_MBOX_CLR[i] = regBaseAddr + i;       
    }
    
    //    INT_EN0_W1C[24];
    regBaseAddr = CIUAddr + CIU_INT0_EN0_W1C_OFFSET_NEW;    
    for (i = 0; i < 24; i++) {
        this->INT_EN0_W1C[i] = regBaseAddr + (i * 2);       
    }
    
    //    CIU_INT_MBOX_CLR INT_MBOX_CLR[12];
    regBaseAddr = CIUAddr + CIU_INT0_EN4_0_W1C_OFFSET_NEW;    
    for (i = 0; i < 12; i++) {
        this->INT_EN4_0_W1C[i] = regBaseAddr + (i * 2);        
    }
    
    //    CIU_INT_MBOX_CLR INT_MBOX_CLR[12];
    regBaseAddr = CIUAddr + CIU_INT0_EN0_W1S_OFFSET_NEW;    
    for (i = 0; i < 24; i++) {
        this->INT_EN0_W1S[i] = regBaseAddr + (i * 2);        
    }
    
    //    CIU_INT_MBOX_CLR INT_MBOX_CLR[12];
    regBaseAddr = CIUAddr + CIU_INT0_EN4_0_W1S_OFFSET_NEW;    
    for (i = 0; i < 12; i++) {
        this->INT_EN4_0_W1S[i] = regBaseAddr + (i * 2);       
    }
    //    PP_RST
    this->PP_RST = CIUAddr + CIU_PP_RST_OFFSET_NEW;
    *(this->PP_RST) = PP_RST_INIT_VALUE_NEW; //Initialized as zero 
    
    //    NMI
    this->NMI = CIUAddr + CIU_NMI_OFFSET_NEW;
    *(this->NMI) = 0x0; //Initialized as zero 
    
    //    FUSE
    this->Fuse = CIUAddr + CIU_FUSE_OFFSET_NEW;
    *(this->Fuse) = ~(~0UL << this->numcores);
}

/** Starts working of CIU
  * @param none
  * 
  * @retval none
  */
void CIU::start()
{
    this->setMappings();
    
    while(true)
    {
        pthread_cond_wait(&(this->MessageBox->cond_device), &(this->MessageBox->lock_device)); //Waiting for notification from core
        
        this->updateFlags();
        
        this->MessageBox->isServiced = true;
    }
}

/** Updates CIU flags whenever a CIU register is manipulated
  * @param none
  * 
  * @retval none
  */
void CIU::updateFlags()
{
    int offset = (this->MessageBox->addr & CIU_OFFSET_MASK) / 8;
    uint64_t data = this->MessageBox->data;
    int core;
    bool updateInterrupt = false;
    
    if(DBG) printf("offset = 0x%d\tdata = 0x%lx\n",offset,data);
    
    if(offset <= CIU_INT23_EN1_OFFSET_NEW && offset >= CIU_INT0_EN0_OFFSET_NEW)
    {
        core = this->handleEnable0(offset, data);
        updateInterrupt = true;
    }
    else if(offset <= CIU_MBOX_SET11_OFFSET_NEW && offset >= CIU_MBOX_SET0_OFFSET_NEW)
    {
        core = this->handleMboxSet(offset, data);
        updateInterrupt = true;
    }
    else if(offset <= CIU_MBOX_CLR11_OFFSET_NEW && offset >= CIU_MBOX_CLR0_OFFSET_NEW)
    {
        core = this->handleMboxClr(offset, data);
        updateInterrupt = true;
    }
    else if(offset == CIU_NMI_OFFSET_NEW)
    {
        this->sendNMI(data);
    }
    else if(offset == CIU_SOFT_RST_OFFSET)
    {
        raise(SIGTERM);
    }
    else if(offset <= CIU_INT11_EN4_1_OFFSET_NEW && offset >= CIU_INT0_EN4_0_OFFSET_NEW)
    {
        core = this->handleEnable4(offset, data);
        updateInterrupt = true;
    }
    else if(offset <= CIU_INT23_EN1_W1C_OFFSET_NEW && offset >= CIU_INT0_EN0_W1C_OFFSET_NEW)
    {
        core = this->handleEnable0_W1C(offset, data);
        updateInterrupt = true;
    }
    else if(offset <= CIU_INT11_EN4_1_W1C_OFFSET_NEW && offset >= CIU_INT0_EN4_0_W1C_OFFSET_NEW)
    {
        core = this->handleEnable4_W1C(offset, data);
        updateInterrupt = true;
    }
    else if(offset <= CIU_INT23_EN1_W1S_OFFSET_NEW && offset >= CIU_INT0_EN0_W1S_OFFSET_NEW)
    {
        core = this->handleEnable0_W1S(offset, data);
        updateInterrupt = true;
    }
    else if(offset <= CIU_INT11_EN4_1_W1S_OFFSET_NEW && offset >= CIU_INT0_EN4_0_W1S_OFFSET_NEW)
    {
        core = this->handleEnable4_W1S(offset, data);
        updateInterrupt = true;
    }
    
    if(updateInterrupt)
        this->updateInterrupt(core);
}

/** Updates interrupt status of specified core
  * @param core: ID of the core
  * 
  * @retval none
  */
void CIU::updateInterrupt(int core)
{
    int i = core * 2;
    Processor_IPE *cores = &this->processors[core];
    uint64_t cause = cores->getCP0()->read(H2_CP0_CAUSE,SEL_ZERO);
    bool IP2 = ((this->INT_SUM0[i]->value & this->INT_EN0[i]->value) != 0);
    bool IP3 = ((this->INT_SUM0[i + 1]->value & this->INT_EN0[i + 1]->value) != 0);
    bool IP4 = ((this->INT_SUM4[core]->value & this->INT_EN4_0[core]->value) != 0);
    
    (IP2) ? cause |= CAUSE_IP2 : cause &= ~CAUSE_IP2;
    (IP3) ? cause |= CAUSE_IP3 : cause &= ~CAUSE_IP3;
    (IP4) ? cause |= CAUSE_IP4 : cause &= ~CAUSE_IP4;
    
    cores->getCP0()->write(cause , H2_CP0_CAUSE, SEL_ZERO);
}

/** Set MBOX flag of summary register when Mailbox Register is set
  * @param offset: Offset of manipulated register
  * @param data: Data which is written to register
  * 
  * @retval ID of core for which register is manipulated
  */
int CIU::handleMboxSet(int offset, uint64_t data)
{
    int core = offset - CIU_MBOX_SET0_OFFSET_NEW;
    this->INT_MBOX_SET[core]->value = data;
    *this->INT_MBOX_CLR[core] = data;
    
    int index = core * 2;
    CIU_INT_MBOX_SET *set = this->INT_MBOX_SET[core];
    if(DBG) printf("Mbox Set for core %d\tValue = 0x%016lx\tEnable = 0x%016lx\n",core,data,this->INT_EN0[index]->value);
    
    if(set->Data.LowerHalf != 0)
    {
        this->INT_SUM0[index]->Data.MBOX0 = 1;
        this->INT_SUM0[index + 1]->Data.MBOX0 = 1;
        this->INT_SUM4[core]->Data.MBOX0 = 1;
    }
    else if(set->Data.UpperHalf != 0)
    {
        this->INT_SUM0[index]->Data.MBOX1 = 1;
        this->INT_SUM0[index + 1]->Data.MBOX1 = 1;
        this->INT_SUM4[core]->Data.MBOX1 = 1;
    }
    return core;
}

/** Clears MBOX flag of summary register when Mailbox Register is cleared
  * @param offset: Offset of manipulated register
  * @param data: Data which is written to register
  * 
  * @retval ID of core for which register is manipulated
  */
int CIU::handleMboxClr(int offset, uint64_t data)
{
    int core = offset - CIU_MBOX_CLR0_OFFSET_NEW;
    this->INT_MBOX_SET[core]->value &= ~data;
    
    int index = core * 2;
    CIU_INT_MBOX_SET *set = this->INT_MBOX_SET[core];
    if(DBG) printf("Mbox Clear for core %d\n",core);
    
    if(set->Data.LowerHalf == 0)
    {
        this->INT_SUM0[index]->Data.MBOX0 = 0;
        this->INT_SUM0[index + 1]->Data.MBOX0 = 0;
        this->INT_SUM4[core]->Data.MBOX0 = 0;
    }
    if(set->Data.UpperHalf == 0)
    {
        this->INT_SUM0[index]->Data.MBOX1 = 0;
        this->INT_SUM0[index  + 1]->Data.MBOX1 = 0;
        this->INT_SUM4[core]->Data.MBOX1 = 0;
    }
    *this->INT_MBOX_CLR[core] = this->INT_MBOX_SET[core]->value;
    return core;
}

/** Identify core ID when some Enable0 register is manipulated
  * @param offset: Offset of manipulated register
  * @param data: Data which is written to register
  * 
  * @retval ID of core for which register is manipulated
  */
int CIU::handleEnable0(int offset, uint64_t data)
{
    int i = offset - CIU_INT0_EN0_OFFSET_NEW;
    int core = i / 4;
    if(DBG) printf("Write 0x%lx to INT_EN0[%d]\n" ,data , i/2);
        
    if(i % 2 == 0) //EN0
        this->INT_EN0[i / 2]->value = data;
    //else  //EN1 not implemented
    
   return core;
}

/** Identify core ID when some Enable4 register is manipulated
  * @param offset: Offset of manipulated register
  * @param data: Data which is written to register
  * 
  * @retval ID of core for which register is manipulated
  */
int CIU::handleEnable4(int offset, uint64_t data)
{
    int i = offset - CIU_INT0_EN4_0_OFFSET_NEW;
    int core = i / 2;
    if(DBG) printf("Write 0x%lx to INT_EN4_0[%d]\n" ,data ,core);
        
    if(i % 2 == 0) //EN4_0
        this->INT_EN4_0[core]->value = data;
    //else  //EN4_1 not implemented
   
    return core;
}

/** Identify core ID and clear corresponding bit in Enable0 register when Enable0_W1c is manipulated
  * @param offset: Offset of manipulated register
  * @param data: Data which is written to register
  * 
  * @retval ID of core for which register is manipulated
  */
int CIU::handleEnable0_W1C(int offset, uint64_t data)
{
    int i = offset - CIU_INT0_EN0_W1C_OFFSET_NEW;
    int core = i / 4;
    if(DBG) printf("Write 0x%lx to INT_EN0_W1C[%d]\n" ,data ,i/2);
        
    if(i % 2 == 0) //EN0
    {
        i /= 2;
        this->INT_EN0[i]->value &= ~data;
        *this->INT_EN0_W1C[i] = 0x0;
    }
    //else  //EN1 not implemented
    
    return core;
}

/** Identify core ID and clear corresponding bit in Enable4 register when Enable4_W1C is manipulated
  * @param offset: Offset of manipulated register
  * @param data: Data which is written to register
  * 
  * @retval ID of core for which register is manipulated
  */
int CIU::handleEnable4_W1C(int offset, uint64_t data)
{
    int i = offset - CIU_INT0_EN4_0_W1C_OFFSET_NEW;
    int core = i / 2;
    if(DBG) printf("Write 0x%lx to INT_EN4_0_W1C[%d]\n" ,data ,i);
        
    if(i % 2 == 0) //EN4_0
    {
        this->INT_EN4_0[core]->value &= ~data;
        *this->INT_EN4_0_W1C[core] = 0x0;
    }
    else  //EN4_1 not implemented
   
    return core;
}

/** Identify core ID and set corresponding bit in Enable0 register when Enable0_W1S is manipulated
  * @param offset: Offset of manipulated register
  * @param data: Data which is written to register
  * 
  * @retval ID of core for which register is manipulated
  */
int CIU::handleEnable0_W1S(int offset, uint64_t data)
{
   int i = offset - CIU_INT0_EN0_W1S_OFFSET_NEW;
   int core = i / 4;
   if(DBG) printf("Write 0x%lx to INT_EN0_W1S[%d]\n" ,data ,i/2);
        
    if(i % 2 == 0) //EN0
    {
        i /= 2;
        this->INT_EN0[i]->value |= data;
        *this->INT_EN0_W1S[i] = 0x0;
    }
    //else  //EN1 not implemented
   
   return core;
}

/** Identify core ID and set corresponding bit in Enable4 register when Enable4_W1S is manipulated
  * @param offset: Offset of manipulated register
  * @param data: Data which is written to register
  * 
  * @retval ID of core for which register is manipulated
  */
int CIU::handleEnable4_W1S(int offset, uint64_t data)
{
    int i = offset - CIU_INT0_EN4_0_W1S_OFFSET_NEW;
    int core = i / 2;
    if(DBG) printf("Write 0x%lx to INT_EN4_0_W1S[%d]\n" ,data ,core);
        
    if(i % 2 == 0) //EN4_0
    {
        this->INT_EN4_0[core]->value |= data;
        *this->INT_EN4_0_W1S[core] = 0x0;
    }
    //else  //EN4_1 not implemented
   
   return core;
}

/** Sends NMI interrupt to corresponding cores
  * @param data: Value written to NIM register
  * 
  * @retval none
  */
void CIU::sendNMI(uint64_t data)
{
    uint64_t coreNMI = (data & NMI_MASK_NEW);
    int i = 0;
    if(DBG) printf("Sending NMI\n");
    while(coreNMI) //Checking for NMI
    {
        if(coreNMI & 0x1 == 0x1)
            if(i < this->numcores) //Check in case if Core0 tries to send NMI to non-existing core
            {
                this->processors[i].setNMI();
//                *(this->NMI) = *(this->NMI) & (~(0x1 << i)); //Clear NMI bit of corresponding core after sending NMI signal
            }
        coreNMI = coreNMI >> 1;
        i++;
    }
    *(this->NMI) = 0x0;
}

/** Set UART flags in summary registers and update cores interrupt status when UART interrupt is generated
  * @param none
  * 
  * @retval none
  */
void CIU::generateInterrupt(uint64_t offset)
{
    uint64_t flag = 0x01UL << offset;
    if(DBG) printf("Generating Uart interrupt\n");
    for(int core = 0; core < this->numcores; core++)
    {
        INT_SUM0[core * 2]->value |= flag;
        INT_SUM0[(core * 2) + 1]->value |= flag;
        INT_SUM4[core]->value |= flag;
        this->updateInterrupt(core);
    }
}

/** Clear UART flags in summary registers and update cores interrupt status when UART interrupt is serviced
  * @param none
  * 
  * @retval none
  */
void CIU::clearInterrupt(uint64_t offset)
{
    uint64_t flag = 0x01UL << offset;
    if(DBG) printf("Clearing Uart interrupt\n");
    for(int core = 0; core < this->numcores; core++)
    {
        INT_SUM0[core * 2]->value &= ~flag;
        INT_SUM0[(core * 2) + 1]->value &= ~flag;
        INT_SUM4[core]->value &= ~flag;
        this->updateInterrupt(core);
    }
}

/** Print status of all cores when pressed Shift+2
  * @param none
  * 
  * @retval none
  */
void CIU::printPC()
{
    for(int core = 0; core < this->numcores; core++){
        CP0_IPE *cp0 = this->processors[core].getCP0();
        printf("Core %d\tPC = 0x%016lx\tCount = 0x%016lx\tCompare = 0x%016lx\n",core,this->processors[core].getPC(),cp0->read(H2_CP0_COUNT,SEL_ZERO),cp0->read(H2_CP0_COMPARE,SEL_ZERO));
        printf("EN0[%d] = 0x%lx\n",core,this->INT_EN0[core]->value);
    }
}
