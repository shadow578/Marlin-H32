#include "endstop_interrupts.h"
#include "hc32f46x_interrupts.h"
#include "../module/endstops.h"

// Z_MIN_PROBE does not follow the same naming as all other pins...
#if HAS_Z_MIN_PROBE_PIN && !defined(HAS_Z_MIN_PROBE)
#define HAS_Z_MIN_PROBE
#endif

//
// Util
//
inline en_exti_lvl_t mapToTriggerMode(ExtIntTriggerMode mode)
{
    switch (mode)
    {
    case RISING:
        return ExIntRisingEdge;
    case FALLING:
        return ExIntFallingEdge;
    case CHANGE:
        return ExIntBothEdge;
    }

    DDL_ASSERT(0);
    return ExIntFallingEdge;
}

inline en_exti_ch_t mapToExternalInterruptChannel(uint8_t pin)
{
    // check range
    if (pin > BOARD_NR_GPIO_PINS)
    {
        return ExtiCh00;
    }

    // map to exti channel
    uint8_t ch = PIN_MAP[pin].gpio_bit;
    if (ch < 0 || ch > 15)
    {
        // invalid channel, only 00 - 15 are valid
        DDL_ASSERT(0);
        return ExtiCh00;
    }

    // exti channel maps 1-to-1
    return (en_exti_ch_t)ch;
}

inline en_int_src_t mapToInterruptSource(uint8_t pin)
{
    // check range
    if (pin > BOARD_NR_GPIO_PINS)
    {
        return INT_PORT_EIRQ0;
    }

    // map to interrupt source
    uint8_t ch = PIN_MAP[pin].gpio_bit;
    if (ch < 0 || ch > 15)
    {
        // invalid source, only 00 - 15 are valid
        DDL_ASSERT(0);
        return INT_PORT_EIRQ0;
    }

    // interrupt source maps 1-to-1
    return (en_int_src_t)ch;
}

inline IRQn_Type mapToIQRVector(uint8_t n)
{
    // map 0-10 to IRQ 10-20
    uint8_t vec = Int010_IRQn + n;
    DDL_ASSERT(vec >= 10 && vec <= 20);
    return (IRQn_Type)vec;
}

void attachInterrupt(uint8_t pin, voidFuncPtr handler, uint8_t irqNum, ExtIntTriggerMode mode)
{
    // check inputs
    if (pin >= BOARD_NR_GPIO_PINS || !handler)
    {
        return;
    }

    // initialize external interrupt channel
    stc_exint_config_t extiConf = {
        .enExitCh = mapToExternalInterruptChannel(pin),
        .enFilterEn = Enable,
        .enFltClk = Pclk3Div8,
        .enExtiLvl = mapToTriggerMode(mode)};
    EXINT_Init(&extiConf);

    // configure port for external interrupt
    stc_port_init_t portConf;
    MEM_ZERO_STRUCT(portConf);
    portConf.enExInt = Enable;
    PORT_InitGPIO(pin, &portConf);

    // register IRQ
    stc_irq_regi_conf_t irqReg = {
        .enIntSrc = mapToInterruptSource(pin),
        .enIRQn = mapToIQRVector(irqNum),
        .pfnCallback = handler};
    enIrqRegistration(&irqReg);

    // clear pending, set priority and enable
    NVIC_ClearPendingIRQ(irqReg.enIRQn);
    NVIC_SetPriority(irqReg.enIRQn, DDL_IRQ_PRIORITY_06);
    NVIC_EnableIRQ(irqReg.enIRQn);
}

inline bool checkAndClearExtIRQFlag(uint8_t pin)
{
    en_exti_ch_t ch = mapToExternalInterruptChannel(pin);
    if (EXINT_IrqFlgGet(ch) == Set)
    {
        EXINT_IrqFlgClr(ch);
        return true;
    }

    return false;
}

//
// IRQ handler
//
void endstopIRQHandler()
{
    bool flag = false;

// check all irq flags
#define CHECK(name) TERN_(HAS_##name, flag |= checkAndClearExtIRQFlag(name##_PIN))

    CHECK(X_MAX);
    CHECK(X_MIN);

    CHECK(Y_MAX);
    CHECK(Y_MIN);

    CHECK(Z_MAX);
    CHECK(Z_MIN);

    CHECK(Z2_MAX);
    CHECK(Z2_MIN);

    CHECK(Z3_MAX);
    CHECK(Z3_MIN);

    CHECK(Z_MIN_PROBE);

    // update endstops
    if (flag)
    {
        endstops.update();
    }
}

//
// HAL functions
//
void setup_endstop_interrupts()
{
    // setup all interrupts
    uint8_t irqn = 0;
#define SETUP(name) TERN_(HAS_##name, attachInterrupt(name##_PIN, endstopIRQHandler, irqn++, CHANGE))

    SETUP(X_MAX);
    SETUP(X_MIN);

    SETUP(Y_MAX);
    SETUP(Y_MIN);

    SETUP(Z_MAX);
    SETUP(Z_MIN);

    SETUP(Z2_MAX);
    SETUP(Z2_MIN);

    SETUP(Z3_MAX);
    SETUP(Z3_MIN);

    SETUP(Z_MIN_PROBE);

    // only 10 IRQs are available
    DDL_ASSERT(irqn <= 11);
}

// ensure max. 10 irqs are registered
// if you encounter this error, you'll have to disable some endstops
#if HAS_X_MAX && HAS_X_MIN && HAS_Y_MAX && HAS_Y_MIN && HAS_Z_MAX && HAS_Z_MIN && HAS_Z2_MAX && HAS_Z2_MIN && HAS_Z3_MAX && HAS_Z3_MIN && HAS_MIN_Z_PROBE
#error "too many endstop interrupts! HC32F46x only supports 10 endstop interrupts."
#endif
