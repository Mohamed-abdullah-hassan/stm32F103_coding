#ifndef __TIMEBASE_H_
#define __TIMEBASE_H_
#include <stdint.h>

uint32_t get_tick(void);
void tick_increment(void);
void timebase_init(void);
void delay(uint32_t delay);



#endif /* TIMEBASE_H_ */
