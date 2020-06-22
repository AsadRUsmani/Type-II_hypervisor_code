/* 
 * File:   GPAtoHVATranslator_IPE.cpp
 * Author: asadusmani
 * 
 * Created on June 9, 2014, 11:47 AM
 */

#include "GPAtoHVATranslator_IPE.h"
#include "GPAMap_IPE.h"


map<uint64_t,GPAtoHVAmap_Val_IPE> GPAtoHVATranslator_IPE::GPAtoHVAMap_IPE;

/** Constructor of GPAtoHVATranslator
  * @param none
  * 
  * @retval none
  */
GPAtoHVATranslator_IPE::GPAtoHVATranslator_IPE()
{
    
}

/** Constructor of GPAtoHVATranslator
  * @param addr: Address needed to be translated
  * 
  * @retval translated address
  */
uint64_t GPAtoHVATranslator_IPE::GPA_to_HVA(uint64_t addr)
{
    uint64_t key = 0, base = 0, offset = 0;
    GPAtoHVAmap_Val_IPE val;
    get_base_and_offset(addr, &key, &offset);
    val = translate_GPA_to_HVA(key);
    base = val.HVA;
    if(base == 0)
    {
        if(DBG)
        {
            printf("##########################################\n");
            printf("First encountered unallocated region\n");
            printf("Addr = 0x%016lx\t Key = 0x%016lx\t Offset = 0x%016lx\n",addr, key, offset);
            printf("##########################################\n");
        }
        base = add_IO_Region(key);
    }
    if(DBG)printf("Address base range = 0x%016llx\n",base);
    
    return (base + offset);
}

/** Destructor of GPAtoHVATranslator
  * @param none
  * 
  * @retval none
  */
GPAtoHVATranslator_IPE::~GPAtoHVATranslator_IPE(){
    
}

/** Translate address key to GPAtoHVAmap_Val_IPE structure
  * @param key: Key for hashmap
  * 
  * @retval GPAtoHVAmap_Val_IPE structure
  */
GPAtoHVAmap_Val_IPE GPAtoHVATranslator_IPE::translate_GPA_to_HVA(uint64_t key){
    map<uint64_t,GPAtoHVAmap_Val_IPE>::iterator i = GPAtoHVAMap_IPE.find( key);
    if(i == GPAtoHVAMap_IPE.end())
    {
        GPAtoHVAmap_Val_IPE val;
        val.HVA = 0;
        val.size = 0;
        return val;//not found
    }
    else
        return i->second;
}

/** Translate address into base and offset
  * @param GPA: Provided address
  * @param base: Pointer location for base
  * @param offset: Pointer location for offset
  * 
  * @retval none
  */
void GPAtoHVATranslator_IPE::get_base_and_offset(uint64_t GPA, uint64_t *base, uint64_t *offset)
{
      
    if((GPA >> 48) != 1){ //Memory  
         *base   = GPA & MEM_ADDR_BASE_MASK;
         *offset = GPA & MEM_ADDR_OFFSET_MASK;
    }
    else{                       //IO
        *base   = GPA & IO_ADDR_BASE_MASK;
        *offset = GPA & IO_ADDR_OFFSET_MASK;
    }
}

/** Initializes memory sub-sections
  * @param ramSize: Required size of ram
  * 
  * @retval none
  */
void GPAtoHVATranslator_IPE::initialize_GPA_subsections(uint64_t ramSize){
    uint64_t i = 0, size = 0, mem_addr = 0;
    init_map_array();
    alloc_DRAM(ramSize);
    GPAtoHVAmap_Val_IPE val;
    for (i = IO_BOOT_BUS; i < ADDRESS_SECTIONS; i++){
        size = addr_sections_ipe[i].ending_address - addr_sections_ipe[i].starting_address + 1;
        if(size <= MAX_SECTION_SIZE)
        {
            add_Mem_Region(size, addr_sections_ipe[i].starting_address);
        }
    }
}

/** Allocates a memory region
  * @param size: Required size of region
  * 
  * @retval pointer to allocated memory
  */
 void * GPAtoHVATranslator_IPE::allocMemRegion(uint64_t size)
 {
	void *dest;	
	dest = mmap(NULL,size, PROT_READ|PROT_WRITE|PROT_EXEC,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
	if (dest == MAP_FAILED)
        {
                perror("dest_mmap not found\n");
                exit(1);
        }
        return dest;
}

 /** Allocates RAM for system
  * @param size: Required size of RAM
  * 
  * @retval none
  */
void GPAtoHVATranslator_IPE::alloc_DRAM(uint64_t size){
    uint64_t ramsize = size, secSize;
    
    /*Allocating Boot Bus*/
    secSize = addr_sections_ipe[BOOT_BUS].ending_address - addr_sections_ipe[BOOT_BUS].starting_address + 1;
    add_Mem_Region(secSize, addr_sections_ipe[BOOT_BUS].starting_address);
    
    /*Allocating DR0 DRAM*/
    secSize = addr_sections_ipe[DR0_DRAM].ending_address - addr_sections_ipe[DR0_DRAM].starting_address + 1;
    if (secSize > ramsize)
        secSize = ramsize;
    ramsize -= secSize;
    add_Mem_Region(secSize, addr_sections_ipe[DR0_DRAM].starting_address);
    
    /*Allocating DR1 DRAM*/
    secSize = addr_sections_ipe[DR1_DRAM].ending_address - addr_sections_ipe[DR1_DRAM].starting_address + 1;
    if (secSize > ramsize)
        secSize = ramsize;
    ramsize -= secSize;
    add_Mem_Region(secSize, addr_sections_ipe[DR1_DRAM].starting_address);
    
    /*Allocating DR2 DRAM*/
    if(ramsize > 0){
        uint64_t allocOffset = 0;
        while(ramsize > 0) //Allocate memory in chunks of 256 MB so that we can have consistent base and offset flags
        {
            secSize = (ramsize > MAX_SECTION_SIZE) ? MAX_SECTION_SIZE : ramsize;
            ramsize -= secSize;
            add_Mem_Region(secSize, addr_sections_ipe[DR2_DRAM].starting_address + allocOffset);
            allocOffset += secSize;
        }
     };
}

/** Allocates a new memory region and adds it to our memory map
  * @param size: Required size of region
  * @param key: Map key of region
  * 
  * @retval none
  */
void GPAtoHVATranslator_IPE::add_Mem_Region(uint64_t size, uint64_t key)
{
    GPAtoHVAmap_Val_IPE val;
    val.HVA = (uint64_t) allocMemRegion(size);
    val.size = size;
    val.map_idx = DR2_DRAM;
    GPAtoHVAMap_IPE[key] = val;
}

/** Allocates a new memory region and adds it to our IO memory map. If region size is greater than MAX_IO_SECTION_SIZE,
  * it only allocated memory equal to MAX_IO_SECTION_SIZE thus makes on demand memory allocation
  * @param key: Map key of region
  * 
  * @retval Address of allocated region
  */
uint64_t GPAtoHVATranslator_IPE::add_IO_Region(uint64_t key)
{
    GPAtoHVAmap_Val_IPE val;
    uint64_t size;
    uint64_t idx = find_index(key, &size);
    if(size > MAX_IO_SECTION_SIZE)
        size = MAX_IO_SECTION_SIZE;
    val.HVA = (uint64_t) allocMemRegion(size);
    val.size = size;
    val.map_idx = idx;
    GPAtoHVAMap_IPE[key] = val;
    return val.HVA;
}

/** Finds the index of given region in array
  * @param addr: Provided address
  * @param size: Pointer to place total size of region
  * 
  * @retval Index of requested region
  */
uint64_t GPAtoHVATranslator_IPE::find_index(uint64_t addr, uint64_t *size)
{
    int i =0;
     for (i = IO_BOOT_BUS; i < ADDRESS_SECTIONS; i++)
        if(addr >= addr_sections_ipe[i].starting_address && addr <= addr_sections_ipe[i].ending_address)
        {
            *size = addr_sections_ipe[i].ending_address - addr_sections_ipe[i].starting_address + 1;
            return i;
        }
}