/* 
 * File:   GPAMap_IPE.h
 * Author: asadusmani
 *
 * Created on June 9, 2014, 1:57 PM
 */

#ifndef GPAMAP_IPE_H
#define	GPAMAP_IPE_H

#include "GPAtoHVATranslator_IPE.h"

#define ADDRESS_SECTIONS 51
#define MAX_SECTION_SIZE        256*1024*1024 // in bytes
#define MAX_IO_SECTION_SIZE     128*1024*1024 // in bytes

#define IO_ADDR_BASE_MASK           0xFFFFFFFFF8000000UL
#define IO_ADDR_OFFSET_MASK         0x0000000007FFFFFFUL
#define MEM_ADDR_BASE_MASK          0xFFFFFFFFF0000000UL
#define MEM_ADDR_OFFSET_MASK        0x000000000FFFFFFFUL

#define DR0_DRAM    0
#define BOOT_BUS    1
#define DR2_DRAM    2
#define DR1_DRAM    3
#define IO_BOOT_BUS 4

struct Address_Section_IPE{
    double starting_address;
    double ending_address;
};



Address_Section_IPE addr_sections_ipe[ADDRESS_SECTIONS];

/** Initializes address array of board
  * @param none
  * 
  * @retval none
  */
void GPAtoHVATranslator_IPE::init_map_array(void){
    
    addr_sections_ipe[0].starting_address = 0x0000000000000000; //DR0 DRAM
    addr_sections_ipe[0].ending_address   = 0x000000000FFFFFFF;
    
    addr_sections_ipe[1].starting_address = 0x0000000010000000; //Boot Bus
    addr_sections_ipe[1].ending_address   = 0x000000001FFFFFFF;
  
    addr_sections_ipe[2].starting_address = 0x0000000020000000; //DR2 DRAM
    addr_sections_ipe[2].ending_address   = 0x00000003FFFFFFFF;
    
    addr_sections_ipe[3].starting_address = 0x0000000410000000; //DR1 DRAM
    addr_sections_ipe[3].ending_address   = 0x000000041FFFFFFF;
    
    addr_sections_ipe[4].starting_address = 0x0001000000000000; //Boot Bus
    addr_sections_ipe[4].ending_address   = 0x00010000FFFFFFFF;
    
    /*addr_sections_ipe[5].starting_address = 0x0001070000000000; //CIU and GPIO NCB type CSRs
    addr_sections_ipe[5].ending_address = 0x00010700000008FF;*/
    
    addr_sections_ipe[5].starting_address = 0x0001070000000000; //CIU and GPIO NCB type CSRs
    addr_sections_ipe[5].ending_address   = 0x00010700001008FF;
    
    addr_sections_ipe[6].starting_address = 0x0001180070000000; //Used for Virtio Devices
    addr_sections_ipe[6].ending_address   = 0x0001180070001FFF;
    
    addr_sections_ipe[7].starting_address = 0x0001280000000000; // FPA Pool 0 Allocate/Free operations
    addr_sections_ipe[7].ending_address   = 0x0001280047FFFFFF;
    
    addr_sections_ipe[8].starting_address = 0x0001180000000000; //MIO BOOT, LED, FUS, TWSI, UART0, UART1, SMI RML type CSRs
    addr_sections_ipe[8].ending_address   = 0x0001180000001FFF;
    
    addr_sections_ipe[9].starting_address = 0x0001180008000000; //GMX0 RML type CSRs
    addr_sections_ipe[9].ending_address   = 0x0001180008001FFF;
    
    addr_sections_ipe[10].starting_address = 0x0001180028000000; //FPA RML type CSRs
    addr_sections_ipe[10].ending_address   = 0x00011800280001FF;
    
    addr_sections_ipe[11].starting_address = 0x0001180040000000; // RNM RML type CSRs
    addr_sections_ipe[11].ending_address   = 0x000118004000000F;

    addr_sections_ipe[12].starting_address = 0x0001180050000000; // PKO RML type CSRs
    addr_sections_ipe[12].ending_address   = 0x0001180050001FFF;
    
    addr_sections_ipe[13].starting_address = 0x0001180058000000; // TIM RML type CSRs
    addr_sections_ipe[13].ending_address   = 0x0001180058001FFF;

    addr_sections_ipe[14].starting_address = 0x0001180068000000; // USBC RML CSRs
    addr_sections_ipe[14].ending_address   = 0x0001180068001FFF;

    addr_sections_ipe[15].starting_address = 0x0001180080000000; // L2C RML type CSRs
    addr_sections_ipe[15].ending_address   = 0x00011800800007FF;

    addr_sections_ipe[16].starting_address = 0x0001180088000000; // LMC RML type CSRs
    addr_sections_ipe[16].ending_address   = 0x000118008800007F;

    addr_sections_ipe[17].starting_address = 0x00011800A0000000; // PIP RMLtype CSRs
    addr_sections_ipe[17].ending_address   = 0x00011800A0001FFF;

    addr_sections_ipe[18].starting_address = 0x00011800B0000000; // ASX0 RML type CSRs
    addr_sections_ipe[18].ending_address   = 0x00011800B00003FF;

    addr_sections_ipe[19].starting_address = 0x00011800F0000000; // IOB RML type CSRs
    addr_sections_ipe[19].ending_address   = 0x00011800F00007FF;
    
    addr_sections_ipe[20].starting_address = 0x0001190000000000; // PCI Bus Config/IACK/Special space
    addr_sections_ipe[20].ending_address   = 0x0001190FFFFFFFFF;
    
    addr_sections_ipe[21].starting_address = 0x00011B0000000000; // PCI Bus Memory Space(subdid3)
    addr_sections_ipe[21].ending_address   = 0x00011B0FFFFFFFFF;
    
    addr_sections_ipe[22].starting_address = 0x00011C0000000000; // PCI Bus Memory Space(subdid4)
    addr_sections_ipe[22].ending_address   = 0x00011C0FFFFFFFFF;
    
    addr_sections_ipe[23].starting_address = 0x00011D0000000000; // PCI Bus Memory Space(subdid5)
    addr_sections_ipe[23].ending_address   = 0x00011D0FFFFFFFFF;
    
    addr_sections_ipe[24].starting_address = 0x00011E0000000000; // PCI Bus Memory Space(subdid6)
    addr_sections_ipe[24].ending_address   = 0x00011E0FFFFFFFFF;
    
    addr_sections_ipe[25].starting_address = 0x00011A0000000000; // PCI Bus I/O space
    addr_sections_ipe[25].ending_address   = 0x00011A0FFFFFFFFF;
    
    addr_sections_ipe[26].starting_address = 0x0001290000000000; // FPA Pool 1 Allocate/Free operations
    addr_sections_ipe[26].ending_address   = 0x0001290FFFFFFFFF;
    
    addr_sections_ipe[27].starting_address = 0x00012A0000000000; // FPA Pool 2 Allocate/Free operations
    addr_sections_ipe[27].ending_address   = 0x00012A0FFFFFFFFF;
    
    addr_sections_ipe[28].starting_address = 0x00012B0000000000; // FPA Pool 3 Allocate/Free operations
    addr_sections_ipe[28].ending_address   = 0x00012B0FFFFFFFFF;
    
    addr_sections_ipe[29].starting_address = 0x00012C0000000000; // FPA Pool 4 Allocate/Free operations
    addr_sections_ipe[29].ending_address   = 0x00012C0FFFFFFFFF;
    
    addr_sections_ipe[30].starting_address = 0x00012D0000000000; // FPA Pool 5 Allocate/Free operations
    addr_sections_ipe[30].ending_address   = 0x00012D0FFFFFFFFF;
    
    addr_sections_ipe[31].starting_address = 0x00012E0000000000; // FPA Pool 6 Allocate/Free operations
    addr_sections_ipe[31].ending_address   = 0x00012E0FFFFFFFFF;
    
    addr_sections_ipe[32].starting_address = 0x00012F0000000000; // FPA Pool 7 Allocate/Free operations
    addr_sections_ipe[32].ending_address   = 0x00012F0FFFFFFFFF;
    
    addr_sections_ipe[33].starting_address = 0x0001400000000000; // RNG Load/IOBDMA operations
    addr_sections_ipe[33].ending_address   = 0x00014000000007FF;
    
    addr_sections_ipe[34].starting_address = 0x00014F0000000000; // IPD NCB type CSRs
    addr_sections_ipe[34].ending_address   = 0x00014F00000007FF;
    
    addr_sections_ipe[35].starting_address = 0x0001520000000000; // PKO doorbell store operations
    addr_sections_ipe[35].ending_address   = 0x000152000003FFFF;
    
    addr_sections_ipe[36].starting_address = 0x0001600000000000; // POW getwork load
    addr_sections_ipe[36].ending_address   = 0x0001600FFFFFFFFF;
    
    addr_sections_ipe[37].starting_address = 0x0001610000000000; // POW status load operations
    addr_sections_ipe[37].ending_address   = 0x0001610FFFFFFFFF;
    
    addr_sections_ipe[38].starting_address = 0x0001620000000000; // POW memory load operations
    addr_sections_ipe[38].ending_address   = 0x000162000000FFFF;
    
    addr_sections_ipe[39].starting_address = 0x0001630000000000; // POW index load/store operations
    addr_sections_ipe[39].ending_address   = 0x0001630FFFFFFFFF;
    
    addr_sections_ipe[40].starting_address = 0x0001670000000000; // POW NCB type CSRs
    addr_sections_ipe[40].ending_address   = 0x00016700000003FF;
    
    addr_sections_ipe[41].starting_address = 0x00016F0000000000; // USBN NCB type CSRs
    addr_sections_ipe[41].ending_address   = 0x00016F0000000FFF;
    
    addr_sections_ipe[42].starting_address = 0x0001F000000000000; // FAU operations
    addr_sections_ipe[42].ending_address   = 0x0001F00FFFFFFFFF;
    
    addr_sections_ipe[43].starting_address = 0x00011F0000000000; // NPI NCB type CSRs, doorbells
    addr_sections_ipe[43].ending_address   = 0x00011F0FFFFFFFFF;
    /*Misc IO Devices*/
    
    addr_sections_ipe[44].starting_address = 0x0001180020000000;
    addr_sections_ipe[44].ending_address   = 0x0001180020001FFF;
    
    addr_sections_ipe[45].starting_address = 0x00011800A8000000;
    addr_sections_ipe[45].ending_address   = 0x00011800A8001FFF;
    
    addr_sections_ipe[46].starting_address = 0x0001180038000000;
    addr_sections_ipe[46].ending_address   = 0x0001180038001FFF;
    
    addr_sections_ipe[47].starting_address = 0x00011800E0000000;
    addr_sections_ipe[47].ending_address   = 0x00011800E0001FFF;
    
    addr_sections_ipe[48].starting_address = 0x00011800B8000000;
    addr_sections_ipe[48].ending_address   = 0x00011800B8001FFF;
            
    addr_sections_ipe[49].starting_address = 0x00016f0010000000;
    addr_sections_ipe[49].ending_address   = 0x00016f0010001FFF;
    
    addr_sections_ipe[50].starting_address = 0x0001180010000000;
    addr_sections_ipe[50].ending_address   = 0x0001180010001FFF;
}


#endif	/* GPAMAP_IPE_H */

