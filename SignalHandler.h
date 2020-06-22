/* 
 * File:   SignalHandler.h
 * Author: asadusmani
 *
 * Created on June 20, 2014, 11:48 AM
 */

#ifndef SIGNALHANDLER_H
#define	SIGNALHANDLER_H

#include "Processor_IPE.h"
#include "UART.h"
#include "VirtioBlk.h"
#ifdef ENABLE_VNET
#include "Virtionet.h"
#endif

class signalHandler
{
public:
    signalHandler();
    virtual ~signalHandler();
    static void sigHandler(int signo, siginfo_t *si, void *uc);
    static void Terminator(int signo, siginfo_t *si, void *uc);
    static void Interruptsignal(int signo, siginfo_t *si, void *uc);
    static void registerSignals();
    static void registerProcessor(Processor_IPE *core);
    static void registerUART(UART *uart);
    static Processor_IPE *retreiveProcessor(uint64_t ID);
    static void registerDisk(VirtioBlk*);
#ifdef ENABLE_VNET
    static void registerVnet(VirtioNet*);
#endif
    static void Exit();

private:
    static map<uint64_t,Processor_IPE*> ProcessorPtrs;
    static UART * uart;
    static VirtioBlk *disk;
#ifdef ENABLE_VNET
    static VirtioNet *vnet;
#endif
};

#endif	/* SIGNALHANDLER_H */

