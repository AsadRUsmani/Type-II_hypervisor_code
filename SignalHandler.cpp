#include "SignalHandler.h"
#include "GPAtoHVATranslator_IPE.h"
#include "UART.h"
#include <sys/syscall.h>

map<uint64_t,Processor_IPE*> signalHandler::ProcessorPtrs;
UART *signalHandler::uart;

VirtioBlk *signalHandler::disk;
void signalHandler::registerDisk(VirtioBlk* vdisk)
{
    disk = vdisk;
}

#ifdef ENABLE_VNET
VirtioNet *signalHandler::vnet;
void signalHandler::registerVnet(VirtioNet* net)
{
    vnet = net;
}
#endif
signalHandler::signalHandler()
{
    
}

signalHandler::~signalHandler()
{
    
}

void signalHandler::sigHandler(int signo, siginfo_t* si, void* uc)
{
    uint64_t ID = (uint64_t)syscall(SYS_gettid);
    if(DBG)printf("signo %d ID %d \n",signo,ID);
    Processor_IPE *core = retreiveProcessor(ID);
    ucontext_t *con = (ucontext_t *)uc;
    if(DBG)printf("Sig#=%d,Sig_err#=%d,Sig_code=%d\n",si->si_signo,si->si_errno,si->si_code);
      
        switch(signo)
        {
                case SIGFPE: 
                    if(DBG)printf("Received SIGFPE:\n");
                    if(DBG)printf("Exception raised by core %d\n",core->getCP0()->corenum());
                    con->uc_mcontext.pc = (greg_t)core->handleSigFPE();
                    break;                    
                case SIGBUS:
                    printf("Received SIGBUS:Trap %d\n",core->getTrapCount());
                    printf("Exception raised by core %d\n",core->getCP0()->corenum());
                    exit(1);
                    break;
//                case SIGTRAP: 
//                    printf("Received SIGTRAP:\n");
//                    printf("Exception raised by core %d\n",core->getCP0()->corenum());
//                    exit(1);
//                    break;
                case SIGSEGV:
                    printf("Received SIGSEGV: at 0x%016llx\n",con->uc_mcontext.pc);
                    printf("Exception raised by core %d\tPC = 0x%016lx\n",core->getCP0()->corenum(),core->getPC());
                    exit(1);
                    break;
//                case SIGALRM:
//                    printf("Received SIGSALRM: at 0x%016llx\n",con->uc_mcontext.pc);
//                    printf("Exception raised timer %d\n",(core->getCP0()->read(H2_CP0_EBASE_1,1) & 0x3FF));
//                    exit(1);
//                    break;    
//                case SIGILL: 
//                    printf("Received SIGILL:\n");
//                    printf("Exception raised by core %d\n",(core->getCP0()->read(H2_CP0_EBASE_1,1) & 0x3FF));
//                    printf("Block start addr = 0x%016llx\n",core->getPC());
//                    exit(1);
//                    break;
                default:  
		    printf("Received other Signal:\n");
                    printf("Exception raised by core %d\n",core->getCP0()->corenum());
		    exit(1);
                    break;
        }	
}

void signalHandler::Terminator(int signo, siginfo_t* si, void* uc)
{
    delete disk;
#ifdef ENABLE_VNET
    delete vnet;
#endif
    printf("Teminating hypervisor\n");
//    raise(SIGTERM);
    exit(1);
}

void signalHandler::Exit()
{
    Terminator(0,NULL,NULL);
}

void signalHandler::Interruptsignal(int signo, siginfo_t* si, void* uc)
{
    uart->sendsignal();
}

void signalHandler::registerSignals()
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof (sa));
    sa.sa_sigaction = &sigHandler;
    sigemptyset(&sa.sa_mask);
 //   sigaddset(&sa.sa_mask,SIGALRM);                   // uncomment when want to block sigarlm or any other signal in all threads 
 //   pthread_sigmask(SIG_BLOCK,&sa.sa_mask,NULL);
    
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    stack_t ss;
    ss.ss_sp = (void*) new char[SIGSTKSZ];
    if (ss.ss_sp == NULL) {
        printf("  Handle error2 \n");
    }
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) == -1) {
        printf("Handle error!signalstack()\n");
    }
//    if (sigaction(SIGILL, &sa, NULL) == -1)
//        if (DBG) printf( "SIGILL Handle error\n");
    if (sigaction(SIGBUS, &sa, NULL) == -1)
        if (DBG) printf("SIGBUS Handle error\n");
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        if (DBG) printf("SIGSEGV Handle error\n");
//    if (sigaction(SIGTRAP, &sa, NULL) == -1)
//        if (DBG) printf("SIGTRAP Handle error\n");
    if (sigaction(SIGFPE, &sa, NULL) == -1)
        if (DBG) printf("SIGTRAP Handle error\n");
    
    sa.sa_sigaction = &Interruptsignal;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        if (DBG) printf("SIGINT Handle error\n");
    sa.sa_sigaction = &Terminator;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        if (DBG) printf("SIGINT Handle error\n");
//    if (sigaction(SIGALRM, &sa, NULL) == -1)
//        if (DBG) printf("SIGTRAP Handle error\n");
}

void signalHandler::registerProcessor(Processor_IPE* core)
{
    uint64_t ID = (uint64_t)syscall(SYS_gettid);
    ProcessorPtrs[ID] = core;
}

void signalHandler::registerUART(UART* urt)
{
    uart  = urt;
}

Processor_IPE *signalHandler::retreiveProcessor(uint64_t ID)
{
    map<uint64_t,Processor_IPE*>::iterator i = ProcessorPtrs.find(ID);
    if(i != ProcessorPtrs.end())
        return i->second;
    else
    {
        printf("Signal in unregistered core. Thread Id = %d\n",ID);
        Exit();
    }
}
