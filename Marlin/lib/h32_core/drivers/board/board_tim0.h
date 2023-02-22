#include "hc32_ddl.h"
#include "hc32f46x_clk.h"
#include "hc32f46x_efm.h"
#include "hc32f46x_utility.h"
#include "hc32f46x_sram.h"
#include "hc32f46x_interrupts.h"
#include "hc32f46x_pwc.h"

#ifndef _BSP_TIM0_H_
#define _BSP_TIM0_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define OS_TICKS_PER_SEC 1000ul

    extern void setup_systick(void);
    extern void setup_time(void);
    extern void SysTickIrq_Handler(void);
    extern uint32_t millis(void);

#define os_get_timeslice(x, y) ((uint32_t)(x >= y ? (x - y) : 0))

#ifdef __cplusplus
}
#endif
#endif
