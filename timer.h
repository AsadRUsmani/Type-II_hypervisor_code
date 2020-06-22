/* 
 * File:   timer.h
 * Author: usamanwar
 *
 * Created on August 12, 2014, 11:10 AM
 */

#ifndef TIMER_H
#define	TIMER_H
#include "CIU.h"
#include "Common.h"
#include <signal.h>

#define INTERVAL_10m 10000000LL

#define TIMER_REALTIME 0
#define TIMER_VIRTUAL  1
#define TIME_FACTOR    1.5      // this factor makes the host clock(i.e 1.2GHz) equals to guest's clock (i.e. 800MHz)
#define LATENCY        10      // this factor multiples with the timer duration to make it long than desired by guest.


class timer
{
private:
   struct sigevent sev;
   timer_t timerid;
   struct itimerspec tout_val;
   struct itimerspec reset_val;
   struct sigaction sa;
   uint64_t startOfTime;
   uint64_t startOfCVMTime;
   uint64_t get_clk();
   void start_timer_tick();
public:
   Processor_IPE *core;
   timer( Processor_IPE *p);
   void resetTimer();
   void updateCounter();
   void updateTimer();
  
};


static void TimerUnit_Handler(int signo, siginfo_t* si, void* uc){
    timer *t =(timer *)si->si_ptr; 
   
   uint64_t compare = t->core->getCP0()->read(H2_CP0_COMPARE,SEL_ZERO);
   if(DBG)printf(" HANDLER --- compare= %llx count= %llx \n",compare, t->core->getCP0()->read( H2_CP0_COUNT, SEL_ZERO));
   if(compare !=0){
        t->core->setTimerTick();
   }
 
}

#endif	/* TIMER_H */

