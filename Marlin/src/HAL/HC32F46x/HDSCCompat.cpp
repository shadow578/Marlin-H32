/**
 * Compatibility Layer  for hdsc.a library
 */

#ifdef HC32F46x
#include "HAL.h"

void HAL_watchdog_refresh()
{
  MarlinHAL::watchdog_refresh();
}

#endif // HC32F46x
