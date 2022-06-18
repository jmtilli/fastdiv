#ifndef _FASTLIBHDR_H_
#define _FASTLIBHDR_H_

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

#endif
