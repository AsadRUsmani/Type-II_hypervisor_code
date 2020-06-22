
#include "timer.h"


#define SIG SIGRTMIN
#define CLOCKID CLOCK_REALTIME


/** Gets time in nano seconds
  * @param none
  * 
  * @retval time in nano seconds
  */
uint64_t timer::get_clk(void)
{
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      return (uint64_t)(ts.tv_sec * 1000000000LL + ts.tv_nsec);
}

/** Constructor of timer. It registers timer with hardware and it's interrupt function
  * @param p: Pointer to processor to which this timer belongs
  * 
  * @retval time in nano seconds
  */
timer::timer(Processor_IPE *p){
   printf("Initializing Timer unit\n");
   
   this->startOfTime = get_clk();
   this->startOfCVMTime = this->startOfTime;
   this->core = p;
  
  int ID = (uint64_t)syscall(SYS_gettid);
  
  this->reset_val.it_interval.tv_sec = 0;
  this->reset_val.it_interval.tv_nsec = 0;
  this->reset_val.it_value.tv_sec = 0;
  this->reset_val.it_value.tv_nsec = 0;
    
  memset(&sa, '\0', sizeof (sa));
  sa.sa_sigaction = &TimerUnit_Handler;
    
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask,SIGALRM);
  pthread_sigmask(SIG_UNBLOCK,&sa.sa_mask,NULL);
  
  if (sigaction(SIGALRM, &sa, NULL) == -1)
      printf("SIGTRAP Handle error\n");
  
  sev.sigev_notify = SIGEV_THREAD_ID;
  sev.sigev_signo = SIGALRM;
  sev.sigev_value.sival_ptr = (void *)this;
  sev.sigev_notify_attributes = NULL;
  sev._sigev_un._tid = ID;
  //creating timer
  if (timer_create(CLOCKID, &sev, &timerid) == -1){
     printf("Error timer_create \n");
     exit(1);
  } 
}

/** Resets the timer
  * Take count compare value from processor registers and set new timer on hardware
  * @param none
  * 
  * @retval none
  */
void timer::resetTimer()
{
    CP0_IPE  *cp0 = this->core->getCP0();
    uint64_t count= cp0->read(H2_CP0_COUNT,SEL_ZERO) & 0xffffffff;
    uint64_t compare= (cp0->read(H2_CP0_COMPARE, SEL_ZERO)) & 0xffffffff;
    uint64_t time;
    int d = compare - count;
    
    if (timer_settime(this->timerid, 0, &(this->reset_val), NULL) == -1)
                printf("Error setting timer from restart_timer \n");
    if (d>0)
        time = d;
    else
    {
            time = (uint32_t)(0xffffffff + d);
    }
    
    time *= LATENCY;
    if(DBG)printf("resetTimer... count %lx compare %lx duration of timer  %d \n",count,compare,time);
    this->tout_val.it_interval.tv_sec = 0;
    this->tout_val.it_interval.tv_nsec = 0;
    if(DBG)printf("Sec = %d\tnsec = %d\n",time / 1000000000,time % 1000000000LL);
    if (time > 1000000000LL) 
    {
        this->tout_val.it_value.tv_sec = time / 1000000000LL;
        time %= 1000000000LL;
    } 
    else
        this->tout_val.it_value.tv_sec = 0;

    this->tout_val.it_value.tv_nsec = time; 
  
  //firing timer
    if (timer_settime(this->timerid, 0, &(this->tout_val), NULL) == -1)
        printf("Error setting timer from restart_timer \n");
}

/** Updates count register of guest
  * @param none
  * 
  * @retval none
  */
void timer::updateCounter()
{
    uint64_t time = this->get_clk();
    CP0_IPE  *cp0 = this->core->getCP0();
    uint64_t currentTime =  (time  - this->startOfTime) / TIME_FACTOR; //subtracting the current host time from when the guest has started 
    uint64_t CVMcurrentTime = (time  - this->startOfCVMTime) / TIME_FACTOR;
    if((cp0->read(H2_CP0_CAUSE,SEL_ZERO) & 0x10000000) == 0)
    {  //check DC is not set
        cp0->write((currentTime & 0xffffffff), H2_CP0_COUNT, SEL_ZERO);
        cp0->write(CVMcurrentTime , H2_CP0_CVMCOUNT_6, SEL_SIX);
    }
    else
        printf("DC bit is set \n");
    if(DBG)printf("Counter updated current value %lx \n",this->core->getCP0()->read(H2_CP0_COUNT,SEL_ZERO));
}

/** Updates timer when count register is re-written
  * @param none
  * 
  * @retval none
  */
void timer::updateTimer()
{
    if(DBG)printf("updateTimer --- value written on count %lx \n", this->core->getCP0()->read(H2_CP0_COUNT,SEL_ZERO));
    uint64_t count = this->core->getCP0()->read(H2_CP0_COUNT,SEL_ZERO);
    uint64_t currentTime = this->get_clk();
    this->startOfTime = currentTime - (count * TIME_FACTOR);
    this->resetTimer();
}