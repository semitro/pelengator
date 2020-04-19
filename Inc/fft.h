#ifndef _FFT_H
#define _FFT_H

#include "stddef.h"
#include "stdint.h"
#include "audio_format.h"
//#define __STATIC_INLINE static inline
//#define __INLINE inline
//#define __ASM __asm
#define __SOFTFP__

#include "arm_math.h"
#include "arm_const_structs.h"

#define FFT_LEN 256


void fft(q15_t* src, q15_t* dst);

q15_t* fft_ch1(Audio_Data *data);


#endif // _FFT_H
