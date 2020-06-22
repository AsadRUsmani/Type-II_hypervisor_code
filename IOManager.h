/* 
 * File:   IOManager.h
 * Author: usamanwar
 *
 * Created on October 21, 2014, 11:38 AM
 */

#ifndef IOMANAGER_H
#define	IOMANAGER_H

#include "AddrTranslationCache.h"
#include "Context_IPE.h"
#include <pthread.h>

#define UART_ID             0
#define CIU_ID              1
#define VBLK_ID             2
#define VNET_ID             3
#ifndef ENABLE_VNET
#define AVAILABLE_DEVICES   3
#else
#define AVAILABLE_DEVICES   4
#endif

#define IO_MASK             0x0001000000000000
#define UART_OFFSET_MASK    0x07FF
#define CIU_OFFSET_MASK     0x7FFF
#define VNET_OFFSET_MASK    0x1FF

#define UART0_BASE_ADDR     0x8001180000000800
#define UART1_BASE_ADDR     0x8001180000000C00
#define UART0_END_ADDR      0x8001180000000890
#define UART1_END_ADDR      0x8001180000000C90
#define CIU_BASE_ADDR       0x8001070000000000
#define CIU_END_ADDR        0x8001070000006D40
#define VNET_BASE_ADDR      0x9001180070000000
#define VNET_END_ADDR       0x9001180070000120
#define VBLK_BASE_ADDR      0x9001180070000200
#define VBLK_END_ADDR       0x9001180070000320

typedef struct
{
    uint64_t addr;
    uint64_t data;
    uint64_t RW;
    volatile bool isServiced;
    pthread_mutex_t lock_device;
    pthread_cond_t cond_device;
}DeviceMessageBox;

class IOManager
{
private:
    static IOManager *iomanager;
    AddrTranslationCache ioaddrCache;
    DeviceMessageBox MessageBox[AVAILABLE_DEVICES];
    uint64_t DeviceHVA[AVAILABLE_DEVICES];
    
private:
    IOManager();
    void inline updateMessageBox(int,uint64_t,Context_IPE*);
    
public:
    static IOManager* getInstance();
    DeviceMessageBox* getMessageBox(int);
    uint64_t getDeviceHVA(int);
    void GVAtoHVA(uint64_t,uint64_t*,Context_IPE*);
    virtual ~IOManager();
    
public: //In-line public functions
    
    /** Check whether an address is of an IO device or Memory
    * @param addr: Address to be checked
    * 
    * @retval ture if IO address otherwise false
    */
  bool isIOaddr(uint64_t addr)
  {
      bool result = false;
      if(unlikely((addr >> 62) == 0x02)) //Region is x-phys
          if(likely(addr & IO_MASK)) //49th bit is 1
              result = true;
      return result;
  }
};

#endif	/* IOMANAGER_H */

