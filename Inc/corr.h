#ifndef _CORR_H
#define _CORR_H

#include "stddef.h"
#include "stdint.h"
#include "audio_format.h"

#include "arm_math.h"

q15_t* evl_shift(Audio_Data *data);
int calc_shift(Audio_Data *data);
int to_grad(int corr);
#endif // _CORR_H
