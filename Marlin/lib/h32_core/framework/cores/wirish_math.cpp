#include "wirish_math.h"

void randomSeed(uint32_t dwSeed)
{
  if (dwSeed != 0) {
    srand(dwSeed) ;
  }
}

long random(long howbig)
{
  if (howbig == 0) {
    return 0 ;
  }

  return rand() % howbig;
}

long random(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }

  long diff = howbig - howsmall;

  return random(diff) + howsmall;
}

uint16_t makeWord(uint16_t w)
{
  return w ;
}

uint16_t makeWord(uint8_t h, uint8_t l)
{
  return (h << 8) | l ;
}