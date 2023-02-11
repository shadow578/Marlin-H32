/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 * Copyright (c) 2017 Victor Perez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * HAL for stm32duino.com based on Libmaple and compatible (HC32F46x based on STM32F1)
 */

#ifdef HC32F46x

#include "../../inc/MarlinConfig.h"
#include "HAL.h"

#if (defined(SERIAL_USB) && !defined(USE_USB_COMPOSITE))
// USBSerial SerialUSB;
#error "SERIAL_USB not supported on HC32F46x"
#endif

uint16_t HAL_adc_result;

// ------------------------
// Private Variables
// ------------------------
enum TEMP_PINS
{
  TEMP_BED = 14,
  TEMP_0 = 15,
};

// ------------------------
// Public functions
// ------------------------

//
// Leave PA11/PA12 intact if USBSerial is not used
//
#if SERIAL_USB
namespace wirish
{
  namespace priv
  {
    void board_setup_usb() {}
  }
}
#endif

void HAL_init()
{
  NVIC_SetPriorityGrouping(0x3);

  // increase WDT timeout, see https://github.com/alexqzd/Marlin-H32/commit/d8483fdff9d582e6773b002a1730052718787c3a
  stc_wdt_init_t wdt_config;
  wdt_config.enCountCycle = WdtCountCycle65536;      ///< Count cycle
  wdt_config.enClkDiv = WdtPclk3Div8192;             ///< Count clock division
  wdt_config.enRefreshRange = WdtRefresh100Pct;      ///< Allow refresh percent range
  wdt_config.enSleepModeCountEn = Disable;           ///< Enable/disable count in the sleep mode
  wdt_config.enRequsetType = WdtTriggerResetRequest; ///< Refresh error or count underflow trigger event type
  WDT_Init(&wdt_config);
}

// HAL idle task
void HAL_idletask()
{
}

void HAL_clear_reset_source() {}

/**
 * TODO: Check this and change or remove.
 * currently returns 1 that's equal to poweron reset.
 */

#define RCC_FLAG_HSIRDY ((uint8_t)0x21)
#define RCC_FLAG_HSERDY ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY ((uint8_t)0x39)
#define RCC_FLAG_LSERDY ((uint8_t)0x41)
#define RCC_FLAG_LSIRDY ((uint8_t)0x61)
#define RCC_FLAG_PINRST ((uint8_t)0x7A)
#define RCC_FLAG_PORRST ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST ((uint8_t)0x7E)
#define RCC_FLAG_LPWRRST ((uint8_t)0x7F)

typedef enum
{
  RESET = 0,
  SET = !RESET
} FlagStatus,
    ITStatus;

FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
  FlagStatus bitstatus = RESET;
  return bitstatus;
}

uint8_t HAL_get_reset_source()
{
  return 1;
}

void _delay_ms(const int delay_ms) { delay(delay_ms); }

extern "C"
{
  extern unsigned int _ebss; // end of bss section
}

// ------------------------
// ADC
// ------------------------
// Init the AD in continuous capture mode
void HAL_adc_init() {}

void HAL_adc_start_conversion(const uint8_t adc_pin)
{
  TEMP_PINS pin_index;
  switch (adc_pin)
  {
  case TEMP_0_PIN:
    pin_index = TEMP_0;
    break;
  case TEMP_BED_PIN:
    pin_index = TEMP_BED;
    break;
  }
  HAL_adc_result = adc_read(ADC1, (uint8_t)pin_index);
}
uint16_t HAL_adc_get_result() { return HAL_adc_result; }

uint16_t HAL_analogRead(pin_t pin)
{
  const bool is_analog = _GET_MODE(pin) == INPUT_ANALOG;
  return is_analog ? analogRead(uint8_t(pin)) : 0;
}

// Wrapper to maple unprotected analogWrite
void HAL_analogWrite(pin_t pin, int pwm_val8)
{
  if (PWM_PIN(pin))
    analogWrite(uint8_t(pin), pwm_val8);
}

void flashFirmware(int16_t value) { NVIC_SystemReset(); }

#endif // HC32F46x
