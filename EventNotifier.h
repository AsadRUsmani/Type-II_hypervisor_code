/* 
 * File:   EventNotifier.h
 * Author: usamanwar
 *
 * Created on 13 July 2015, 10:58
 */

#ifndef EVENTNOTIFIER_H
#define	EVENTNOTIFIER_H


#include <sys/epoll.h>
#include <sys/eventfd.h>

#define EVENTNOTIFIER_DBG    0

class EventNotifier
{
public:
    EventNotifier();
    ~EventNotifier();
    int getfd();
    void write(long int);
    int poll();
    int enablePolling();
private:
    int Eventfd;
    int pollfd;
};

#endif	/* EVENTNOTIFIER_H */

