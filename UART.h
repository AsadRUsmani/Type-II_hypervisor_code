/* 
 * File:   UART.h
 * Author: usamanwar
 *
 * Created on October 16, 2014, 1:59 PM
 */

#ifndef UART_H
#define	UART_H


#include "CIU.h"
#include "IOManager.h"

#define BUFFER_SIZE 3
////////////Macros////////////

/* Register Offsets*/

#define RBR_OFFSET 0  /*Receive Buffer Register */
#define THR_OFFSET 0  /*Transmit Holding Register */
#define DLL_OFFSET 0  /*Divisor Latch Low Register */

#define IER_OFFSET 1  /*Interrupt-Enable Register */
#define DLH_OFFSET 1  /*Divisor Latch High Register */
 
#define FCR_OFFSET 2  /*FIFO Control Register */
#define IIR_OFFSET 2  /*Interrupt Identity Register */

#define LCR_OFFSET 3  /*Line Control Register */
#define MCR_OFFSET 4  /*Modem-Control Register */
#define LSR_OFFSET 5  /*Line Status Register */
#define MSR_OFFSET 6  /*Modem Status Register */
#define SCR_OFFSET 7  /*Scratchpad Register */
#define FAR_OFFSET 8  /*FIFO Access Register */
#define TFR_OFFSET 9  /*Transmit FIFO Read Register */
#define RFW_OFFSET 10 /*Receive FIFO Write Register */
#define USR_OFFSET 11 /*UART Status Register */
#define TFL_OFFSET 12 /*Transmit FIFO Level Register */
#define RFL_OFFSET 13 /*Transmit FIFO Level Register */
#define SRR_OFFSET 14 /*Software Reset Register */


/* FIFO Control Register */
#define UART_FCR_ENABLE      0x01    	/* Enable FIFO  */
#define UART_FCR_CLRX        0x02    	/* Clear Rx FIFO /* */
#define UART_FCR_CLTX        0x04    	/* Clear Tx FIFO /* */
#define UART_FCR_RXTRGONE    (0x00<<6) 	/* Rx FIFO ONE CHAR /* */
#define UART_FCR_RXTRGQUAR   (0x01<<6)       /* Rx FIFO QUARTER /* */
#define UART_FCR_RXTRGHALF   (0x10<<6)       /* Rx FIFO HALF /* */
#define UART_FCR_RXTRGFULL   (0x11<<6)       /* Rx FIFO FULL /* */

/* Interrupt Identification Register */
#define UART_IIR_NO_INT	0x01	/* No interrupts pending */
#define UART_IIR_ID	0x06	/* Mask for the interrupt ID */

#define UART_IIR_MSI	0x00	/* Modem status interrupt */
#define UART_IIR_THRI	0x02	/* Transmitter holding register empty */
#define UART_IIR_RDI	0x04	/* Receiver data interrupt */
#define UART_IIR_RLSI	0x06	/* Receiver line status interrupt */
#define UART_IIR_CTI    0x0C    /* Character Timeout Indication */

#define UART_IIR_FENF   0x80    /* Fifo enabled, but not functionning */
#define UART_IIR_FE     0xC0    /* Fifo enabled */

//////////////////////////////////////////////////////////////
#define UART_LCR_DLAB	0x80	/* Divisor latch access bit */
//////////////////////////////////////////////////////////////

/* Interrupt Enable Register */

#define UART_IER_MSI	0x08	/* Enable Modem status interrupt */
#define UART_IER_RLSI	0x04	/* Enable receiver line status interrupt */
#define UART_IER_THRI	0x02	/* Enable Transmitter holding register int. */
#define UART_IER_RDI	0x01	/* Enable receiver data interrupt *

/* Line Status Register */
#define UART_LSR_DR           0x01    /* Data ready  */
#define UART_LSR_OE           0x02    /* Overrun   */            
#define UART_LSR_PE           0x04    /* Parity error  */       
#define UART_LSR_FE           0x08    /* Framing error  */       
#define UART_LSR_BI           0x10    /* Break  */               
#define UART_LSR_THRE         0x20    /* Xmit hold reg empty  */ 
#define UART_LSR_TEMT         0x40    /* Xmitter empty  */       
#define UART_LSR_FERR         0x80    /* FIFO Error  */
#define UART_LSR_INT_ANY      0x1E    /* Any of the lsr-interrupt-triggering status bits */

/* Modem Status Register*/
#define UART_MSR_DCD	0x80	/* Data Carrier Detect */
#define UART_MSR_RI	0x40	/* Ring Indicator */
#define UART_MSR_DSR	0x20	/* Data Set Ready */
#define UART_MSR_CTS	0x10	/* Clear to Send */
#define UART_MSR_DDCD	0x08	/* Delta DCD */
#define UART_MSR_TERI	0x04	/* Trailing edge ring indicator */
#define UART_MSR_DDSR	0x02	/* Delta DSR */
#define UART_MSR_DCTS	0x01	/* Delta CTS */
#define UART_MSR_ANY_DELTA 0x0F	/* Any of the delta bits! */

/*Modem Control Register*/
#define UART_MCR_LOOP	0x10	/* Enable loopback test mode */
#define UART_MCR_OUT2	0x08	/* Out2 complement */
#define UART_MCR_OUT1	0x04	/* Out1 complement */
#define UART_MCR_RTS	0x02	/* RTS complement */
#define UART_MCR_DTR	0x01	/* DTR complement */

/*FIFO Access Register*/
#define UART_FAR_FE     0x1     /* FIFO Access Mode Enable*/

/*Transmit FIFO register*/
#define UART_TFR_DATA 	0xFF	/* Tx FIFO Data*/

/*Uart Status Register*/
#define UART_USR_TFNF	0x2	/* Tx FIFO not full*/	
#define UART_USR_TFE	0x3	/* Tx FIFO not empty*/

/*transmit FIFO Level Register*/
#define UART_TFL_TFL	0xEF	/* No of enteries in Tx FIFO*/

#define UART_FIFO_LENGTH    16      /* 16550A Fifo Length */

#define XMIT_FIFO           0
#define RECV_FIFO           1
#define MAX_XMIT_RETRY      4

class UART
{
private:
    uint64_t RBR;
    uint64_t THR;
    uint64_t DLL;
    uint64_t IER;
    uint64_t DLH;
    uint64_t IIR;
    uint64_t FCR;
    uint64_t LCR;
    uint64_t MCR;
    uint64_t LSR;
    uint64_t MSR;
    uint64_t SCR;
    uint64_t FAR;
    uint64_t TFL;
    uint64_t TFR;
    uint64_t USR;
    uint64_t SRR;
    uint64_t index;
    
    uint64_t UART_Buffer[BUFFER_SIZE];
    uint64_t *Reg_HVA;  //Base Address of UART registers
    DeviceMessageBox *MessageBox;
    CIU *CIUunit;
    int thr_ipending;
private:
    void inline uart_update_irq();
    void inline uart_io_read();
    void inline uart_io_write();
    void uart_reset();
    void uart_update_memory();
    static void *uart_receiver(void *uartptr);
    static char inline getch();
public:
    UART(CIU*);
    virtual ~UART();
    void start();
    static void EnableEcho();
    void sendsignal();
};


#endif	/* UART_H */

