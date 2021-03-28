#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <xc.h> // include processor files - each processor file is guarded. 

unsigned long get_millis(void);
void timer_init();

#endif