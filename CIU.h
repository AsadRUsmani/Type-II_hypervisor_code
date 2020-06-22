/* 
 * File:   CIU.h
 * Author: usamanwar
 *
 * Created on 21 May 2015, 17:34
 */

#ifndef CIU_H
#define	CIU_H

#include "Processor_IPE.h"

#define CIU_INT0_SUM0_OFFSET_NEW                0
#define CIU_INT0_EN0_OFFSET_NEW                 64
#define CIU_INT23_EN1_OFFSET_NEW                111

#define CIU_MBOX_SET0_OFFSET_NEW                192
#define CIU_MBOX_SET11_OFFSET_NEW               203
#define CIU_MBOX_CLR0_OFFSET_NEW                208
#define CIU_MBOX_CLR11_OFFSET_NEW               219

#define CIU_PP_RST_OFFSET_NEW                   224
#define CIU_NMI_OFFSET_NEW                      227
#define CIU_FUSE_OFFSET_NEW                     229
#define CIU_SOFT_RST_OFFSET                     232

#define CIU_INT0_SUM4_OFFSET_NEW                384
#define CIU_INT0_EN4_0_OFFSET_NEW               400
#define CIU_INT11_EN4_1_OFFSET_NEW              423

#define CIU_INT0_EN0_W1C_OFFSET_NEW             1088
#define CIU_INT23_EN1_W1C_OFFSET_NEW            1135

#define CIU_INT0_EN4_0_W1C_OFFSET_NEW           1424
#define CIU_INT11_EN4_1_W1C_OFFSET_NEW          1447

#define CIU_INT0_EN0_W1S_OFFSET_NEW             3136
#define CIU_INT23_EN1_W1S_OFFSET_NEW            3183

#define CIU_INT0_EN4_0_W1S_OFFSET_NEW           3472
#define CIU_INT11_EN4_1_W1S_OFFSET_NEW          3495

#define PP_RST_INIT_VALUE_NEW           0x0FFE
#define NMI_MASK_NEW                    0x0FFF

#define CIU_SUM_GPIO0                       16
#define CIU_SUM_GPIO1                       17
#define CIU_SUM_GPIO2                       18
#define CIU_SUM_GPIO3                       19
#define CIU_SUM_MBOX0                       32
#define CIU_SUM_MBOX1                       33
#define CIU_SUM_UART0                       34
#define CIU_SUM_UART1                       35

typedef union {
    uint64_t value;

    struct {
        uint64_t BOOTDMA : 1;
        uint64_t MII : 1;
        uint64_t IPDPPTHR : 1;
        uint64_t POWIQ : 1;
        uint64_t TWSI2 : 1;
        uint64_t Reserved : 2;
        uint64_t USB : 1;
        uint64_t Timer : 4;
        uint64_t KEY_ZERO : 1;
        uint64_t IPD_DRP : 1;
        uint64_t GMX_DRP : 2;
        uint64_t TRACE : 1;
        uint64_t RML : 1;
        uint64_t TWSI : 1;
        uint64_t WDOG_SUM : 1;
        uint64_t PCI_MSI : 4;
        uint64_t PCI_INT : 4;
        uint64_t UART1 : 1;
        uint64_t UART0 : 1;
        uint64_t MBOX1 : 1;
        uint64_t MBOX0 : 1;
        uint64_t GPIO : 14;
        uint64_t GPIO1 : 1;
        uint64_t GPIO0 : 1;
        uint64_t WorkQ : 16;
    } Data;
    
} CIU_INT_SUM0_EN0;

typedef union {
    uint64_t value;

    struct {
        uint64_t Reserved : 52;
        uint64_t WDOG : 12;
    } Data;
    
} CIU_INT_SUM1_EN1;

typedef union {
    uint64_t value;

    struct {
        uint64_t Reserved : 32;
        uint64_t UpperHalf : 16;
        uint64_t LowerHalf : 16;
    } Data;
    
} CIU_INT_MBOX_SET;

class CIU{
public:
    CIU(Processor_IPE *processor, int numcores);
    void generateInterrupt(uint64_t flag);
    void clearInterrupt(uint64_t flag);
    void printPC();
    void start();
    
private:
    inline void setMappings();
    inline void updateFlags();
    inline int handleMboxSet(int offset, uint64_t data);
    inline int handleMboxClr(int offset, uint64_t data);
    inline int handleEnable0(int offset, uint64_t data);
    inline int handleEnable4(int offset, uint64_t data);
    inline int handleEnable0_W1C(int offset, uint64_t data);
    inline int handleEnable4_W1C(int offset, uint64_t data);
    inline int handleEnable0_W1S(int offset, uint64_t data);
    inline int handleEnable4_W1S(int offset, uint64_t data);
    inline void sendNMI(uint64_t data);
    void updateInterrupt();
    void updateInterrupt(int core);
    
private:
    CIU_INT_SUM0_EN0 *INT_SUM0[24];
    CIU_INT_SUM0_EN0 *INT_SUM4[12];
    CIU_INT_SUM0_EN0 *INT_EN0[24];
    CIU_INT_SUM0_EN0 *INT_EN4_0[12];
    CIU_INT_MBOX_SET *INT_MBOX_SET[12];
    
    uint64_t *INT_MBOX_CLR[12];
    
    uint64_t *INT_EN0_W1C[24];
    uint64_t *INT_EN0_W1S[24];
    uint64_t *INT_EN4_0_W1C[12];
    uint64_t *INT_EN4_0_W1S[12];
    uint64_t *NMI;
    uint64_t *PP_RST;
    uint64_t *Fuse;
    
    uint64_t baseAddr;
    Processor_IPE *processors;
    int numcores;
    
    DeviceMessageBox *MessageBox;
};

#endif	/* CIU_H */

