/* 
 * File:   common.h
 * Author: asadusmani
 * Created on February 12, 2014, 11:54 AM
 */

#ifndef COMMON_H
#define	COMMON_H

#include <cstdlib>
#include <stdint.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <map>
#include <syscall.h>
#include "P.h"

#define DBG                     0        
#define DBG_LOG                 1    
#define LOG_GEN                 0

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#endif	/* COMMON_H */

