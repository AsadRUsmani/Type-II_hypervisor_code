/* 
 * File:   TLB_IPE.cpp
 * Author: asadusmani
 * 
 * Created on June 5, 2014, 12:02 PM
 */

#include "TLB_IPE.h"
#include "ExceptionHandler_IPE.h"


/*TLB_Entry constructor*/
TLB_Entry_IPE::TLB_Entry_IPE(){
    this->reset_Entry();
}

/*Set page mask of TLB entry*/
void TLB_Entry_IPE::set_pagemask(uint64_t mask){
    this->pagemask = mask;
}

/*Get page mask of TLB entry*/
uint64_t TLB_Entry_IPE::get_pagemask(){
    return this->pagemask;
}

/*Set entry hi of TLB entry*/
void TLB_Entry_IPE::set_entryHi(uint64_t entry){
    this->entryHi = entry;
}

/*Get entry hi of TLB entry*/
uint64_t TLB_Entry_IPE::get_entryHi(){
    return (this->VPN | this->ASID /*| ((uint64_t)this->R << ENTRYHI_REGION)*/);
}

/*Set entry Lo0 of TLB entry*/
void TLB_Entry_IPE::set_entryLo0(uint64_t entry){
    this->entryLo0 = entry;
}

/*Get entry Lo0 of TLB entry*/
uint64_t TLB_Entry_IPE::get_entryLo0(){
    return this->entryLo0;
//    return (this->G | (this->V0 << ENTRYLO_VALID) | (this->D0 << ENTRYLO_DIRTY) | (this->C0 << ENTRYLO_COHERENCY) | (this->PFN[0] >> ENTRYLO_PFN));
}
 
/*Set entry Lo1 of TLB entry*/
void TLB_Entry_IPE::set_entryLo1(uint64_t entry){
    this->entryLo1 = entry;
}

/*Get entry Lo1 of TLB entry*/
uint64_t TLB_Entry_IPE::get_entryLo1(){
    return this->entryLo1;
//    return (this->G | (this->V1 << ENTRYLO_VALID) | (this->D1 << ENTRYLO_DIRTY) | (this->C1 << ENTRYLO_COHERENCY) | (this->PFN[1] >> ENTRYLO_PFN));
}

void TLB_Entry_IPE::set_Entry(CP0_IPE* CP0){
    set_pagemask(CP0->read(H2_CP0_PAGEMASK, 0));
    set_entryHi(CP0->read(H2_CP0_ENTRYHI, 0));
    set_entryLo0(CP0->read(H2_CP0_ENTRYLO0, 0));
    set_entryLo1(CP0->read(H2_CP0_ENTRYLO1, 0));
    
    this->ASID = this->entryHi & ENTRYHI_ASID_MASK;
    this->G = this->entryLo0 & this->entryLo1 & ENTRYLO_GLOBAL_MASK;
    this->VPN = (this->entryHi & ~MIN_PAGE_MASK) & SEG_MASK;
    this->R = (this->entryHi >> REGION_OFFSET) & REGION_MASK;
    this->Validity = ENTRY_VALID;
    
    this->V0 = (this->entryLo0 & ENTRYLO_VALID_MASK) != 0;
    this->D0 = (this->entryLo0 & ENTRYLO_DIRTY_MASK) != 0;
    this->C0 = (this->entryLo0 >> ENTRYLO_COHERENCY) & ENTRYLO_COHERENCY_MASK;
    this->PFN[0] = (((this->entryLo0 >> ENTRYLO_PFN) << MIN_PAGE_BITS)  & ~(this->pagemask >> 1));
    
    this->V1 = (this->entryLo1 & ENTRYLO_VALID_MASK) != 0;
    this->D1 = (this->entryLo1 & ENTRYLO_DIRTY_MASK) != 0;
    this->C1 = (this->entryLo1 >> ENTRYLO_COHERENCY) & ENTRYLO_COHERENCY_MASK;
    this->PFN[1] = (((this->entryLo1 >> ENTRYLO_PFN) << MIN_PAGE_BITS)  & ~(this->pagemask >> 1));
}

void TLB_Entry_IPE::reset_Entry(){
    this->pagemask = 0x0;
    this->entryHi = 0x0;
    this->entryLo0 = 0x0;
    this->entryLo1 = 0x0;
    
    this->VPN = 0x0;
    this->ASID = 0x0;
    this->Validity = ENTRY_INVALID;
    this->PFN[0] = 0x0;
    this->PFN[1] = 0x0;
}

void TLB_Entry_IPE::print_entry(){
    printf("Pagemask: 0x%016llx\t EntryHi : 0x%016llx\nEntryLo0: 0x%016llx\t EntryLo1: 0x%016llx\n\n", this->pagemask, this->entryHi, this->entryLo0, this->entryLo1);
}

/*Destructor for TLB_Entry*/
TLB_Entry_IPE::~TLB_Entry_IPE(){}

bool TLB_IPE::GVAtoGPA(uint64_t GVA, uint64_t* GPA, Context_IPE *context){
    int idx;
    int result = TLB_MATCH;
    TLB_Entry_IPE *entry;
    uint64_t mask;
    uint64_t VPN;
    uint64_t tag;
    uint8_t asid = this->get_ASID();
    uint8_t R = (GVA >> REGION_OFFSET) & REGION_MASK;
    
    if(DBG)printf("Finding GVA = 0x%016lx\n",GVA);
    
    for(idx = 0; idx < TLB_MAX_ENTERIES; idx++){
        entry = &(this->tlb_entry[idx]);
        mask = entry->get_pagemask() | MIN_PAGE_MASK;
        tag = (GVA & ~mask) & SEG_MASK;
        VPN = entry->VPN & ~mask;
        mask >>= 1;
        if(DBG)printf("tag = 0x%016lx\tVPN = 0x%016lx\n",tag,VPN);
        
        if((entry->G == ENTRY_GLOBAL || entry->ASID == asid) && (entry->R == R) && (VPN == tag) && (entry->Validity == ENTRY_VALID)){
            int n = (GVA >> __builtin_popcount(mask)) & 0x01;
            
            if (!(n ? entry->V1 : entry->V0))
                result = TLB_INV;
            else if ((context->getLDST() == WRITE) && !(n ? entry->D1 : entry->D0)) {
                result = TLB_MOD;
            }
            context->setProt((n ? entry->D1 : entry->D0));
            *GPA = entry->PFN[n] | (GVA & mask);
            break;
        }
    }
            
    if(idx == TLB_MAX_ENTERIES)
        result = TLBL + context->getLDST();
    
    if(result != TLB_MATCH){
        if(DBG)printf("TLB not found for 0x%016lx.Code = %d\n",GVA,result);
        *GPA = TLB_Exception_IPE::TLB_Excep(cp0, GVA, result, context);
    }
    if(DBG)printf("GPA = 0x%016lx\n",*GPA);
    return (result == TLB_MATCH);
}

void TLB_IPE::write_tlb_state(FILE *fp){
    int idx;
    for (idx = 0; idx <= TLB_MAX_ENTERIES; idx++){
        TLB_Entry_IPE entry = this->tlb_entry[idx];
        fprintf(fp,"tlbentry %d: VPN 0x%x ASID 0x%x mask 0x%x entryLo0 0x%x 0x%x 0x%x  entryLo1 0x%x 0x%x 0x%x\n",idx,entry.VPN,entry.ASID,entry.get_pagemask(),entry.PFN[0],entry.D0,entry.V0,entry.PFN[1],entry.D1,entry.V1);
    }
}

/*TLB_IPE Constructor*/
TLB_IPE::TLB_IPE(CP0_IPE* cp0)
{
	this->cp0 = cp0;
}

/*Get Random Register value*/
uint32_t TLB_IPE::get_random(){
    return (cp0->read(H2_CP0_RANDOM, SEL_ZERO) & RANDOM_MASK);
}

/*Set Random Register value*/
void TLB_IPE::set_random(uint64_t value){
    cp0->write(value & RANDOM_MASK,H2_CP0_RANDOM, SEL_ZERO);
}

/*Increment in random register*/
void TLB_IPE::inc_random(){
    uint32_t random = this->get_random();
    random++;
    if(random >= TLB_MAX_ENTERIES){
        random = this->get_wired();
    }
    cp0->write(random, H2_CP0_RANDOM, SEL_ZERO);
}

/*Get Wired Register value*/
uint32_t TLB_IPE::get_wired(){
    return (cp0->read(H2_CP0_WIRED, SEL_ZERO) & WIRED_MASK);
}

/*Set Index value*/
void TLB_IPE::set_index(uint32_t index_val){
    cp0->write(INDEX_MASK & index_val, H2_CP0_INDEX, SEL_ZERO);
}

/*Get Index value*/
uint64_t TLB_IPE::get_index(){
    return (INDEX_MASK & cp0->read(H2_CP0_INDEX, SEL_ZERO));
}

/*Set Probe flag*/
void TLB_IPE::set_probe(){
    cp0->write(cp0->read(H2_CP0_INDEX, SEL_ZERO) | INDEX_PROBE_MASK, H2_CP0_INDEX, SEL_ZERO);
}

/*Set page mask register*/
void TLB_IPE::set_pagemask(uint64_t mask){
    cp0->write(mask, H2_CP0_PAGEMASK, SEL_ZERO);
}

/*Set entry hi register*/
void TLB_IPE::set_entryHi(uint64_t entry){
    cp0->write(entry, H2_CP0_ENTRYHI, SEL_ZERO);
}

/*Get entry hi register*/
uint64_t TLB_IPE::get_entryHi(){
    return cp0->read(H2_CP0_ENTRYHI, SEL_ZERO);
}

/*Set entry Lo0 register*/
void TLB_IPE::set_entryLo0(uint64_t entry){
    cp0->write(entry, H2_CP0_ENTRYLO0, SEL_ZERO);
}

/*Set entry Lo1 register*/
void TLB_IPE::set_entryLo1(uint64_t entry){
    cp0->write(entry, H2_CP0_ENTRYLO1, SEL_ZERO);
}

/*Check address region of entry*/
uint8_t TLB_IPE::check_region(){
    return (REGION_MASK & (cp0->read(H2_CP0_ENTRYHI, SEL_ZERO) >> ENTRYHI_REGION));
}

/*Get ASID from entryHi register*/
uint8_t TLB_IPE::get_ASID(){
    return (cp0->read(H2_CP0_ENTRYHI, SEL_ZERO) & ENTRYHI_ASID_MASK);
}

/*Get TLB entry at index idx*/
void TLB_IPE::get_TLBEntry(uint32_t idx){
    this->set_pagemask(this->tlb_entry[idx].get_pagemask());
    this->set_entryHi(this->tlb_entry[idx].get_entryHi());
    this->set_entryLo0(this->tlb_entry[idx].get_entryLo0());
    this->set_entryLo1(this->tlb_entry[idx].get_entryLo1());
}

/*Reset TLB entry*/
void TLB_IPE::reset_TLBEntry(uint32_t idx){
    this->tlb_entry[idx].reset_Entry();
}

/*Clear all TLB entries*/
void TLB_IPE::clear_TLB(){
    for(int i = 0; i < TLB_MAX_ENTERIES; i++)
        this->tlb_entry[i].reset_Entry();
}

/*Print TLB entry at index idx*/
void TLB_IPE::print_TLBEntry(uint32_t idx){
    this->tlb_entry[idx].print_entry();
}

/*Print all TLB entries*/
void TLB_IPE::print_TLB(){
    int i;
    for(i=0; i<TLB_MAX_ENTERIES; i++){
        this->print_TLBEntry(i);
    }
}

/*tlbp*/
void TLB_IPE::tlb_probe(){
    int idx;
    TLB_Entry_IPE *entry;
    uint64_t mask;
    uint64_t VPN;
    uint64_t tag;
    uint8_t asid = this->get_ASID();
    uint8_t R = this->check_region();
    
    for(idx = 0; idx < TLB_MAX_ENTERIES; idx++){
        entry = &(this->tlb_entry[idx]);
        mask = entry->get_pagemask() | MIN_PAGE_MASK;
        tag = (this->get_entryHi() & ~mask) & SEG_MASK;
        VPN = entry->VPN & ~mask;
        
        if((entry->G == ENTRY_GLOBAL || entry->ASID == asid) /*&& (entry->R == R)*/ && (VPN == tag)){
            this->set_index(idx);
            break;
        }
    }
    
    if(idx == TLB_MAX_ENTERIES)
        this->set_probe(); 
    
    for(++idx ; idx < TLB_MAX_ENTERIES; idx++){
        entry = &(this->tlb_entry[idx]);
        mask = entry->get_pagemask() | MIN_PAGE_MASK;
        tag = (this->get_entryHi() & ~mask) & SEG_MASK;
        VPN = entry->VPN & ~mask;
        
        if((entry->G == ENTRY_GLOBAL || entry->ASID == asid) && (VPN == tag) &&  (entry->Validity == ENTRY_VALID)){
            entry->Validity = ENTRY_INVALID;
            printf("Invalidated duplicate entry\n");
            break;
        }
    }
    
}

/*tlbwr*/
void TLB_IPE::tlb_random(){
//    int idx = this->get_random();
    uint64_t idx = rand() % TLB_MAX_ENTERIES;
    uint64_t wired = this->get_wired();
    if(idx < wired)
        idx += wired;
    this->tlb_entry[idx].set_Entry(this->cp0);
    if(DBG)printf("TLBWR: Writing Entry at %d\n",idx);
    if(DBG)this->tlb_entry[idx].print_entry();
    this->set_random(idx);
//    this->inc_random();
}

/*tlbwi*/
void TLB_IPE::tlb_write_index(){
    int idx = this->get_index();
    this->tlb_entry[idx].set_Entry(this->cp0);
    if(DBG)printf("TLBWI: Writing Entry at %d\n",idx);
    if(DBG)this->tlb_entry[idx].print_entry();
}

/*tlbr*/
void TLB_IPE::tlb_read(){
    this->get_TLBEntry(this->get_index());    
}
/*TLB_IPE Destructor*/
TLB_IPE::~TLB_IPE()
{
        
}
