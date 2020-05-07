#ifndef __NANO_SLEEP_H__
#define __NANO_SLEEP_H__

#include <Windows.h>

#ifndef TRUE
  #define FALSE 0
  #define TRUE !FALSE
#endif

BOOLEAN nanosleep(LONGLONG ns);

#endif __NANO_SLEEP_H__
