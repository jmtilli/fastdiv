#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "fastdivhdr.h"

static inline uint8_t ilog(uint32_t i)
{
  uint8_t result = 0;
  while (i >>= 1)
  {
    result++;
  }
  return result;
}

void init_fastdivctx(struct fastdivctx *ctx, uint32_t divisor)
{
  uint8_t ilogd = ilog(divisor);
  int power_of_2 = (divisor & (divisor - 1)) == 0;
  if (divisor == 0 || divisor >= (1U<<31))
  {
    abort(); // Not supported
  }
  if (power_of_2)
  {
    ctx->shift1 = 0;
  }
  else
  {
    ctx->shift1 = 1;
  }
  ctx->shift2 = ilogd;
  ctx->mod = divisor;
  ctx->mult = (1ULL<<(32+ctx->shift1+ctx->shift2)) / divisor + 1;
}
