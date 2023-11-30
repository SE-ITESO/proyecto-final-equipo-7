#include "PIT.h"
#include "fsl_pit.h"

static void (*PIT0_callback)(uint32_t flags) = 0;
static void (*PIT1_callback)(uint32_t flags) = 0;
static void (*PIT2_callback)(uint32_t flags) = 0;


void PIT_callback_init(uint32_t channel, void (*handler)(uint32_t flags)) {
    switch (channel) {
        case kPIT_Chnl_0:
            PIT0_callback = handler;
            break;
        case kPIT_Chnl_1:
            PIT1_callback = handler;
            break;
        case kPIT_Chnl_2:
            PIT2_callback = handler;
            break;
    }
}

void PIT_init()
{
	pit_config_t pitConfig;
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT, &pitConfig);
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	//PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
	//PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT0_IRQn);
	//EnableIRQ(PIT1_IRQn);
	//EnableIRQ(PIT2_IRQn);
}

void PIT_timer_period(uint32_t channel, uint32_t usec) {
    switch (channel) {
        case kPIT_Chnl_0:
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(usec, PIT_SOURCE_CLOCK));
            break;
        case kPIT_Chnl_1:
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(usec, PIT_SOURCE_CLOCK));
            break;
        case kPIT_Chnl_2:
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, USEC_TO_COUNT(usec, PIT_SOURCE_CLOCK));
            break;
        default:
            break;
    }
}

void PIT_start(uint8_t channel) {
    switch (channel) {
        case kPIT_Chnl_0:
            PIT_StartTimer(PIT, kPIT_Chnl_0);
            break;
        case kPIT_Chnl_1:
            PIT_StartTimer(PIT, kPIT_Chnl_1);
            break;
        case kPIT_Chnl_2:
            PIT_StartTimer(PIT, kPIT_Chnl_2);
            break;
        default:
            break;
    }
}

void PIT_stop(uint32_t channel) {
    switch (channel) {
        case kPIT_Chnl_0:
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            break;
        case kPIT_Chnl_1:
            PIT_StopTimer(PIT, kPIT_Chnl_1);
            break;
        case kPIT_Chnl_2:
            PIT_StopTimer(PIT, kPIT_Chnl_2);
            break;
        default:
            break;
    }
}


void PIT0_IRQHandler()
{


	if (PIT0_callback)
	{
		PIT0_callback(1);
	}

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
}

void PIT2_IRQHandler()
{


	if (PIT2_callback)
	{
		PIT2_callback(1);
	}

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
}
