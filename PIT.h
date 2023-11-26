#ifndef PIT_H_
#define PIT_H_

#include "stdint.h"
#include "stdbool.h"

#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

volatile static bool pitIsrFlag = false;

void PIT_init();
void PIT_timer_period(uint32_t channel, uint32_t usec);
void PIT_callback_init(uint32_t channel, void (*handler)(uint32_t flags));
void PIT_start(uint8_t channel);
void PIT_stop(uint32_t channel);

#endif /* PIT_H_ */
