#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "fastdivhdr.h"

int main(int argc, char **argv)
{
  struct fastdivctx ctx;
  uint32_t divisor;
  struct timeval tv1, tv2;
  int i, j;
  uint32_t special_divisors[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,  (1U<<31) - 1};
  uint32_t special_numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                (1U<<31) - 3, (1U<<31) - 2, (1U<<31) - 1,
                                (1U<<31), (1U<<31) + 1, (1U<<31) + 2,
                                (1ULL<<32) - 3, (1ULL<<32) - 2, (1ULL<<32) - 1};


  for (i = 0; i < (int)(sizeof(special_divisors)/sizeof(*special_divisors)); i++)
  {
    divisor = special_divisors[i];
    init_fastdivctx(&ctx, divisor);
    for (j = 0; j < 10000; j++)
    {
      uint32_t x;
      x = ((((unsigned)rand())&1)<<31) | ((unsigned)rand());
      if (fastmod(&ctx, x) != (x % divisor))
      {
        printf("failure: %u %% %u\n", x, divisor);
      }
      if (fastdiv(&ctx, x) != (x / divisor))
      {
        printf("failure: %u %% %u\n", x, divisor);
      }
    }
    for (j = 0; j < (int)(sizeof(special_numbers)/sizeof(*special_numbers)); j++)
    {
      uint32_t x = special_numbers[j];
      if (fastmod(&ctx, x) != (x % divisor))
      {
        printf("failure: %u %% %u\n", x, divisor);
      }
      if (fastdiv(&ctx, x) != (x / divisor))
      {
        printf("failure: %u %% %u\n", x, divisor);
      }
    }
  }
  for (i = 0; i < 10000; i++)
  {
    divisor = (unsigned)rand();
    init_fastdivctx(&ctx, divisor);
    for (j = 0; j < 10000; j++)
    {
      uint32_t x;
      x = ((((unsigned)rand())&1)<<31) | ((unsigned)rand());
      if (fastmod(&ctx, x) != (x % divisor))
      {
        printf("failure: %u %% %u\n", x, divisor);
      }
      if (fastdiv(&ctx, x) != (x / divisor))
      {
        printf("failure: %u %% %u\n", x, divisor);
      }
    }
    for (j = 0; j < (int)(sizeof(special_numbers)/sizeof(*special_numbers)); j++)
    {
      uint32_t x = special_numbers[j];
      if (fastmod(&ctx, x) != (x % divisor))
      {
        printf("failure: %u %% %u\n", x, divisor);
      }
      if (fastdiv(&ctx, x) != (x / divisor))
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
    volatile uint32_t x = (uint32_t)i;
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
  printf("fast mod: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);

  gettimeofday(&tv1, NULL);
  volatile uint32_t d = 37;
  for (i = 0; i < 1000*1000*1000/4; i++)
  {
    volatile uint32_t x = (uint32_t)i;
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
  printf("direct mod: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);
  gettimeofday(&tv1, NULL);
  for (i = 0; i < 1000*1000*1000/4; i++)
  {
    volatile uint32_t x = (uint32_t)i + 1;
    if (fastdiv(&ctx, x) == x)
    {
      abort();
    }
    if (fastdiv(&ctx, x) == x)
    {
      printf("2\n");
      abort();
    }
    if (fastdiv(&ctx, x) == x)
    {
      printf("3\n");
      abort();
    }
    if (fastdiv(&ctx, x) == x)
    {
      printf("4\n");
      abort();
    }
  }
  gettimeofday(&tv2, NULL);
  printf("fast div: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);

  gettimeofday(&tv1, NULL);
  for (i = 0; i < 1000*1000*1000/4; i++)
  {
    volatile uint32_t x = (uint32_t)i + 1;
    if (x / d == x)
    {
      abort();
    }
    if (x / d == x)
    {
      abort();
    }
    if (x / d == x)
    {
      abort();
    }
    if (x / d == x)
    {
      abort();
    }
  }
  gettimeofday(&tv2, NULL);
  printf("direct div: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);

  gettimeofday(&tv1, NULL);
  for (i = 0; i < 1000*1000*1000/4; i++)
  {
    volatile uint32_t x = (uint32_t)i + 1;
    uint32_t div, mod;
    fastdivmod(&ctx, x, &div, &mod);
    if (mod == d || div == x)
    {
      printf("1\n");
      abort();
    }
    fastdivmod(&ctx, x, &div, &mod);
    if (mod == d || div == x)
    {
      printf("2\n");
      abort();
    }
    fastdivmod(&ctx, x, &div, &mod);
    if (mod == d || div == x)
    {
      printf("3\n");
      abort();
    }
    fastdivmod(&ctx, x, &div, &mod);
    if (mod == d || div == x)
    {
      printf("4\n");
      abort();
    }
  }
  gettimeofday(&tv2, NULL);
  printf("fast divmod: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);

  gettimeofday(&tv1, NULL);
  for (i = 0; i < 1000*1000*1000/4; i++)
  {
    volatile uint32_t x = (uint32_t)i + 1;
    if (x / d == x || x % d == d)
    {
      abort();
    }
    if (x / d == x || x % d == d)
    {
      abort();
    }
    if (x / d == x || x % d == d)
    {
      abort();
    }
    if (x / d == x || x % d == d)
    {
      abort();
    }
  }
  gettimeofday(&tv2, NULL);
  printf("direct divmod: %g nsec\n", tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec-tv1.tv_usec)/1e6);

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
