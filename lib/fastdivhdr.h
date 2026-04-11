#ifndef _FASTDIVHDR_H_
#define _FASTDIVHDR_H_

#include <stdint.h>

struct fastdivctx {
  uint32_t mult;
  uint32_t mod;
  uint8_t shift1:1;
  uint8_t shift2:7;
};

static inline uint32_t fastmod(struct fastdivctx *ctx, uint32_t eax)
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

static inline uint32_t fastdiv(struct fastdivctx *ctx, uint32_t eax)
{
  uint64_t edxeax = ((uint64_t)eax) * ctx->mult;
  uint32_t edx = edxeax>>32;
  eax -= edx;
  eax >>= (ctx->shift1);
  eax += edx;
  eax >>= (ctx->shift2);
  return eax;
}

static inline void fastdivmod(struct fastdivctx *ctx, uint32_t eax,
                              uint32_t *div, uint32_t *mod)
{
  uint64_t edxeax = ((uint64_t)eax) * ctx->mult;
  uint32_t edx = edxeax>>32;
  uint32_t eaxorig = eax;
  eax -= edx;
  eax >>= (ctx->shift1);
  eax += edx;
  eax >>= (ctx->shift2);
  *div = eax;
  edx = ctx->mod*eax;
  *mod = eaxorig - edx;
}

void init_fastdivctx(struct fastdivctx *ctx, uint32_t divisor);

// LLVM 2.6 is first with clang, it supports __has_builtin
// GCC 3.4 is the first GCC with __has_clz
#undef FASTDIV_HAS_CLZ

#ifdef __has_builtin
  #if __has_builtin (__builtin_clz)
    #define FASTDIV_HAS_CLZ
  #endif
#else
  #ifdef __GNUC__
    #if __GNUC__ == 3 && __GNUC_MINOR__ >= 4
      #define FASTDIV_HAS_CLZ
    #else
      #if __GNUC__ > 3
        #define FASTDIV_HAS_CLZ
      #endif
    #endif
  #endif
#endif

#ifdef FASTDIV_HAS_CLZ
static inline uint8_t fastdiv_ilog(uint32_t x)
{
  if (x == 0)
  {
    return 0;
  }
  return 31-__builtin_clz(x);
}
#else
static inline uint8_t fastdiv_ilog(uint32_t i)
{
  uint8_t result = 0;
  while (i >>= 1)
  {
    result++;
  }
  return result;
}
#endif

static inline void init_fastdivctx_inline(struct fastdivctx *ctx, uint32_t divisor)
{
  uint8_t ilogd = fastdiv_ilog(divisor);
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

#endif
