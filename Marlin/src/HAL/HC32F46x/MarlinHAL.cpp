#include "HAL.h"
#include "../cores/iwdg.h"

MarlinHAL::MarlinHAL() {}

void MarlinHAL::watchdog_init()
{

    //  increase WDT timeout, see https://github.com/alexqzd/Marlin-H32/commit/d8483fdff9d582e6773b002a1730052718787c3a
    stc_wdt_init_t wdt_config;
    wdt_config.enCountCycle = WdtCountCycle65536;      ///< Count cycle
    wdt_config.enClkDiv = WdtPclk3Div8192;             ///< Count clock division
    wdt_config.enRefreshRange = WdtRefresh100Pct;      ///< Allow refresh percent range
    wdt_config.enSleepModeCountEn = Disable;           ///< Enable/disable count in the sleep mode
    wdt_config.enRequsetType = WdtTriggerResetRequest; ///< Refresh error or count underflow trigger event type
    WDT_Init(&wdt_config);
}

void MarlinHAL::watchdog_refresh()
{
    iwdg_feed();
    // watchdog_refresh();
}

void MarlinHAL::init()
{
    HAL_init();
}

void MarlinHAL::init_board() {}

void MarlinHAL::reboot()
{
    NVIC_SystemReset();
}

bool MarlinHAL::isr_state()
{
    return ISRS_ENABLED();
}

void MarlinHAL::isr_on()
{
    ENABLE_ISRS();
}

void MarlinHAL::isr_off()
{
    DISABLE_ISRS();
}

void MarlinHAL::delay_ms(const int ms)
{
    _delay_ms(ms);
}

void MarlinHAL::idletask()
{
    HAL_idletask();
}

uint8_t MarlinHAL::get_reset_source()
{
    return HAL_get_reset_source();
}

void MarlinHAL::clear_reset_source()
{
    HAL_clear_reset_source();
}

int MarlinHAL::freeMemory()
{
    volatile char top;
    return top;
}

void MarlinHAL::adc_init()
{
    HAL_adc_init();
}

void MarlinHAL::adc_enable(const pin_t pin)
{
    HAL_ANALOG_SELECT(pin);
}

void MarlinHAL::adc_start(const pin_t pin)
{
    HAL_adc_start_conversion(pin);
}

bool MarlinHAL::adc_ready()
{
    return HAL_ADC_READY();
}

uint16_t MarlinHAL::adc_value()
{
    return HAL_adc_get_result();
}

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t a, const bool b)
{
    // TODO stub
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired)
{
    // TODO stub
}
