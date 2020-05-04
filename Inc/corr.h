#ifndef _CORR_H
#define _CORR_H

#include "stddef.h"
#include "stdint.h"
#include "audio_format.h"

#include "arm_math.h"

q15_t mean_silence_ch1;
q15_t mean_silence_ch2;

/* use to detect microphones zero-level values */
void calibrate_mean(Audio_Data *data);
q15_t* evl_shift(Audio_Data *data);
int max_index(q31_t* corr);
int calc_shift(Audio_Data *data, q15_t** gde_data);
int to_grad(int corr);
#endif // _CORR_H
