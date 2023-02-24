#include "systick.h"
#include "../../hdsc/common/hc32_ddl.h"

volatile uint32_t uptime = 0;

extern "C" void SysTickIrq_Handler(void)
{
    uptime++;
}

void systick_init()
{
    stc_clk_freq_t stcClkFreq;
    CLK_GetClockFreq(&stcClkFreq);
    SysTick_Config(stcClkFreq.sysclkFreq / TICKS_PER_SECOND);
}

uint32_t millis()
{
    return uptime;
}
