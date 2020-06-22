#include "UART.h"
#include <termios.h>

UART::UART(CIU* CIUunit)
{
    IOManager* manager = IOManager::getInstance();
    printf("Initializing UART........\n");
    this->CIUunit = CIUunit;
    this->Reg_HVA = (uint64_t*) manager->getDeviceHVA(UART_ID);
    this->uart_reset();
    this->MessageBox = manager->getMessageBox(UART_ID);
}

UART::~UART(){}

void UART::uart_reset()
{
    this->RBR = 0;
    this->IER = 0;
    this->IIR = UART_IIR_NO_INT;
    this->LCR = 0;
    this->LSR = UART_LSR_TEMT | UART_LSR_THRE;
    this->MSR = UART_MSR_DCD | UART_MSR_DSR | UART_MSR_CTS;
    this->MCR = UART_MCR_OUT2;
    this->SCR = 0;
    
    this->uart_update_memory();  //Update all values in HVA space
    this->thr_ipending = 0;
}

void UART::uart_update_memory()
{
    this->Reg_HVA[RBR_OFFSET] = this->RBR;
    this->Reg_HVA[IER_OFFSET] = this->IER;
    this->Reg_HVA[IIR_OFFSET] = this->IIR;
    this->Reg_HVA[LCR_OFFSET] = this->LCR;
    this->Reg_HVA[MCR_OFFSET] = this->MCR;
    this->Reg_HVA[LSR_OFFSET] = this->LSR;
    this->Reg_HVA[MSR_OFFSET] = this->MSR;
    this->Reg_HVA[SCR_OFFSET] = this->SCR;
}

void UART::start()
{
    printf("Initialized UART\n");
    pthread_t uartreceiver;
    if (pthread_create(&uartreceiver, NULL, &UART::uart_receiver, (void*) this)) 
    {
        printf("Error creating UART receiver thread.\n");
        pthread_exit(NULL);
    }
        
    while(true)
    {
        pthread_cond_wait(&(this->MessageBox->cond_device), &(this->MessageBox->lock_device)); //Waiting for notification from core
        if(this->MessageBox->RW == WRITE) //Writing to UART register
            this->uart_io_write();
        else    //Reading from UART register
            this->uart_io_read();
        this->MessageBox->isServiced = true;
    }
    
    pthread_join(uartreceiver,NULL);
}

void UART::uart_io_write()
{
    char addr = (this->MessageBox->addr & 0xFF) / 8;
    uint64_t data = this->MessageBox->data;
    char letter = (char) data;
    switch(addr)
    {
        default:
        case THR_OFFSET:
            if(this->LCR & UART_LCR_DLAB)
                this->DLL = data;
            else
            {
                this->THR = data;
                this->LSR &= ~UART_LSR_THRE;
//                putchar((char)data);
                write(STDOUT_FILENO, (void*)&letter, 1);
                this->LSR |= (UART_LSR_THRE | UART_LSR_TEMT);
                this->thr_ipending = 1;
                this->uart_update_irq();
            }
            break;
        case IER_OFFSET:
            if(this->LCR & UART_LCR_DLAB)
                this->DLH = data;
            else
            {
                this->IER = data & 0x0F;
                if(this->LSR & UART_LSR_THRE)
                    this->uart_update_irq();
            }
            break;
        case FCR_OFFSET:
            this->FCR = data;
            break;
        case LCR_OFFSET:
            this->LCR = data;
            break;
        case MCR_OFFSET:
            this->MCR = data;
            break;
        case LSR_OFFSET:
        case MSR_OFFSET:
            break;
        case SCR_OFFSET:
            this->SCR = data;
            break;
    };
}

void UART::uart_io_read()
{
    char addr = (this->MessageBox->addr & 0xFF) / 8;
     switch(addr)
    {
        default:
        case RBR_OFFSET:
            if(this->LCR & UART_LCR_DLAB)
                this->Reg_HVA[DLL_OFFSET] = this->DLL;
            else
            {
                    this->Reg_HVA[RBR_OFFSET] = this->UART_Buffer[3-index];
                    
                    this->index--;
                    if((this->index == 0) || (this->UART_Buffer[0] != 0x1b))
                    {
                        this->LSR &= ~UART_LSR_DR;
                        this->Reg_HVA[LSR_OFFSET] = this->LSR;
                    }
                    this->uart_update_irq();
            }
            break;
        case IER_OFFSET:
            if(this->LCR & UART_LCR_DLAB)
                this->Reg_HVA[DLH_OFFSET] = this->DLH;
            else
                this->Reg_HVA[IER_OFFSET] = this->IER;
            break;
        case IIR_OFFSET:
            this->Reg_HVA[IIR_OFFSET] = this->IIR;
            if((this->IIR & UART_IIR_ID) == UART_IIR_THRI){
                this->thr_ipending = 0;
                this->uart_update_irq();
            }
            break;
        case LCR_OFFSET:
            this->Reg_HVA[LCR_OFFSET] = this->LCR;
            break;
        case MCR_OFFSET:
            this->Reg_HVA[MCR_OFFSET] = this->MCR;
            break;
        case LSR_OFFSET:
            this->Reg_HVA[LSR_OFFSET] = this->LSR;
            if(this->LSR & (UART_LSR_BI | UART_LSR_OE))
            {
                this->LSR &= ~(UART_LSR_BI | UART_LSR_OE);
                this->uart_update_irq();
            }
            break;
        case MSR_OFFSET:
            this->Reg_HVA[MSR_OFFSET] = this->MSR;
            if(this->MSR & UART_MSR_ANY_DELTA)
            {
                this->MSR &= 0xF0;
//                this->uart_update_irq();
            }
            break;
        case SCR_OFFSET:
            this->Reg_HVA[SCR_OFFSET] = this->SCR;
            break;
        case FAR_OFFSET:
            this->FAR |= UART_FAR_FE; //FIFO enabled
            this->Reg_HVA[FAR_OFFSET] = this->FAR;
            break; 
    
    };
}

void UART::uart_update_irq()
{
    uint64_t tempiir = UART_IIR_NO_INT;
    
    if ((this->IER & UART_IER_RLSI) && (this->LSR & UART_LSR_INT_ANY))
        tempiir = UART_IIR_RLSI;
    else if((this->IER & UART_IER_RDI) && (this->LSR & UART_LSR_DR))
        tempiir = UART_IIR_CTI;
    else if((this->IER & UART_IER_THRI) && this->thr_ipending)
        tempiir = UART_IIR_THRI;
    
    this->IIR = tempiir | (this->IIR & 0xF0);
    
    if(tempiir != UART_IIR_NO_INT)
    {
        this->thr_ipending = 0;
//        printf("Generating UART interrupt\n");
        this->Reg_HVA[IIR_OFFSET] = this->IIR;
        this->CIUunit->generateInterrupt(CIU_SUM_UART1);
    }
    else
        this->CIUunit->clearInterrupt(CIU_SUM_UART1);
}

char UART::getch()
{
    struct termios oldattr, newattr;
    char ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void *UART::uart_receiver(void *uartptr)
{
    UART *uart = (UART*)uartptr;
    while(true)
    {
        uart->index = 3;
        int i = 0;
        for(i = 0; i < BUFFER_SIZE; i++)
        {
            uart->UART_Buffer[i] = (uint64_t) getch();
            if(uart->UART_Buffer[0] != 0x1b)
                break;
        }
        uart->LSR |= UART_LSR_DR;
        uart->RBR = uart->UART_Buffer[i];
        uart->Reg_HVA[RBR_OFFSET] = uart->UART_Buffer[i];
        uart->Reg_HVA[LSR_OFFSET] = uart->LSR;
            if(uart->UART_Buffer[0] == 0x40)
                uart->CIUunit->printPC();
            else if(uart->UART_Buffer[0] == 0x01)
                raise(SIGUSR1);
            else
                uart->uart_update_irq();
        }
        
 }

void UART::sendsignal()
{
    this->index = 3;
    this->UART_Buffer[0] = this->RBR = (uint64_t) 0x03;
    this->LSR |= UART_LSR_DR;
    this->Reg_HVA[RBR_OFFSET] = this->RBR;
    this->Reg_HVA[LSR_OFFSET] = this->LSR;
    this->uart_update_irq();
}

void UART::EnableEcho()
{
    struct termios attr;
    tcgetattr( STDIN_FILENO, &attr );
    attr.c_lflag |= ( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &attr );
}
