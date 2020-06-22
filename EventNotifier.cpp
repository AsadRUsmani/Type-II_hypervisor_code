#include "EventNotifier.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

/** Constructor of EventNotifier
  * Opens event fd
  * @param none
  * 
  * @retval none
  */
EventNotifier::EventNotifier()
{
    int fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (fd < 0)
        printf("Error: Can't create eventfd.\n");
    this->Eventfd = fd;
    this->pollfd = -1;
}

/** Destructor of EventNotifier
  * Closes event fd
  * @param none
  * 
  * @retval none
  */
EventNotifier::~EventNotifier()
{
    close(this->Eventfd);
    if(this->pollfd != -1)
        close(this->pollfd);
}

/** Gives fd of event fd file
  * @param none
  * 
  * @retval fd of event fd file
  */
int EventNotifier::getfd()
{
    return this->Eventfd;
}

/** Writes to event fd file
  * @param value: value to be written to event fd file
  * 
  * @retval none
  */
void EventNotifier::write(long int value)
{
    if(EVENTNOTIFIER_DBG) printf("%s\n",__func__);
    eventfd_write(this->Eventfd, value);
}

/** Enable polling of event fd file
  * @param none
  * 
  * @retval 0 on success, -1 on failure
  */
int EventNotifier::enablePolling()
{
//    int fd = epoll_create1(FD_CLOEXEC);
    int fd = epoll_create1(0);
    if(EVENTNOTIFIER_DBG) printf("%s\n",__func__);
    if(fd < 0)
    {
        printf("Error: %s Opening pollfd\n",__func__);
        return -1;
    }
    struct epoll_event evnt = {0};
    evnt.data.fd = this->Eventfd;
    evnt.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(fd, EPOLL_CTL_ADD, this->Eventfd, &evnt) == -1)
    {
        printf("Error: %s epoll_ctl\n",__func__);
        return -1;
    }
    this->pollfd = fd;
    return 0;
}

/** Starts polling of event fd file
  * @param none
  * 
  * @retval value read from event fd file
  */
int EventNotifier::poll()
{
    int ret;
    long int count;
    struct epoll_event event;
    if(EVENTNOTIFIER_DBG) printf("%s\n",__func__);
    
    ret = epoll_wait(this->pollfd, &event, 1, -1);
    if (ret > 0)
    {
        if(event.events & EPOLLIN)
        {
            if(this->Eventfd == event.data.fd)
            {
                ret = read(this->Eventfd, &count, sizeof(count));
                if (ret < 0)
                    printf("%s: Read fail\n",__func__);
            }
            else
              printf("%s: Event occur in some other descriptor\n",__func__);   
        }
        else
           printf("%s: Error in event occur\n",__func__); 
    }
    return ret;
}
