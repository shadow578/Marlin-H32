#define _BSP_TIM0_C_
#include "startup.h"

volatile uint32_t systick_uptime_millis = 0;

extern void SysTickIrq_Handler(void)
{
	++systick_uptime_millis;
}

extern void setup_systick(void)
{
	stc_clk_freq_t stcClkFreq;
	MEM_ZERO_STRUCT(stcClkFreq);
	CLK_GetClockFreq(&stcClkFreq);
	SysTick_Config(stcClkFreq.sysclkFreq / OS_TICKS_PER_SEC);
}


extern uint32_t millis(void)
{
	return (systick_uptime_millis);
}

#undef _BSP_TIM0_C_
