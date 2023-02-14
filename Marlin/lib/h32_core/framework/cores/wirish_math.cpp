#include "wirish_math.h"

void randomSeed(uint32_t seed)
{
  if (seed != 0) {
    srand(seed);
  }
  return;
}

long random(long howbig)
{
  uint32_t uVar1;
  long lVar2;
  
  if (howbig == 0) {
    lVar2 = 0;
  }
  else {
    uVar1 = rand();
    lVar2 = uVar1 - howbig * (uVar1 / howbig);
  }
  return lVar2;
}

long random(long howsmall,long howbig)
{
  long lVar1;
  long diff;
  
  if (howsmall < howbig) {
    lVar1 = random(howbig - howsmall);
    howsmall = howsmall + lVar1;
  }
  return howsmall;
}

//uint16_t makeWord(uint16_t w)
//{
//  return w;
//}
//
//uint16_t makeWord(uint8_t h,uint8_t l)
//{
//  uint16_t local_a;
//  
//  local_a = CONCAT11(h,l);
//  return local_a;
//}
