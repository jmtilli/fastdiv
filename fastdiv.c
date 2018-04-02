#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

struct fastdivctx {
  uint32_t mult;
  uint32_t mod;
  uint8_t shift1:1;
  uint8_t shift2:7;
};

uint8_t ilog(uint32_t i)
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

uint32_t fastmod(struct fastdivctx *ctx, uint32_t eax)
{
  uint64_t edxeax = ((uint64_t)eax) * ctx->mult;
  uint32_t edx = edxeax>>32;
  uint32_t eaxorig = eax;
  eax -= edx;
  eax >>= (ctx->shift1);
  eax += edx;
  eax >>= (ctx->shift2);
  edx = ctx->mod*eax;
  return eaxorig - edx;
}

int main(int argc, char **argv)
{
  struct fastdivctx ctx;
  uint32_t divisor;
  struct timeval tv1, tv2;
  int i, j;
  for (i = 0; i < 10000; i++)
  {
    divisor = rand();
    init_fastdivctx(&ctx, divisor);
    for (j = 0; j < 10000; j++)
    {
      uint32_t x;
      x = ((rand()&1)<<31) | rand();
      if (fastmod(&ctx, x) != (x % divisor))
      {
        printf("failure: %u %% %u\n", x, divisor);
      }
    }
  }
  printf("Verified successfully\n");
  init_fastdivctx(&ctx, 37);
  gettimeofday(&tv1, NULL);
  for (i = 0; i < 1000*1000*1000/4; i++)
  {
    volatile uint32_t x = i;
    if (fastmod(&ctx, x) == 37)
    {
      abort();
    }
    if (fastmod(&ctx, x) == 37)
    {
      abort();
    }
    if (fastmod(&ctx, x) == 37)
    {
      abort();
    }
    if (fastmod(&ctx, x) == 37)
    {
      abort();
    }
  }
  gettimeofday(&tv2, NULL);
  printf("fast: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);

  gettimeofday(&tv1, NULL);
  volatile uint32_t d = 37;
  for (i = 0; i < 1000*1000*1000/4; i++)
  {
    volatile uint32_t x = i;
    if (x % d == 37)
    {
      abort();
    }
    if (x % d == 37)
    {
      abort();
    }
    if (x % d == 37)
    {
      abort();
    }
    if (x % d == 37)
    {
      abort();
    }
  }
  gettimeofday(&tv2, NULL);
  printf("direct: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);
  gettimeofday(&tv1, NULL);
  for (i = 0; i < 1000*1000*1000/4; i++)
  {
    init_fastdivctx(&ctx, d);
    init_fastdivctx(&ctx, d);
    init_fastdivctx(&ctx, d);
    init_fastdivctx(&ctx, d);
  }
  gettimeofday(&tv2, NULL);
  if (fastmod(&ctx, 0x12345678) == 37)
  {
    abort();
  }
  printf("init: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);
  return 0;
}
