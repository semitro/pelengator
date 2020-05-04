#ifndef _CORR_H
#define _CORR_H

#include "stddef.h"
#include "stdint.h"
#include "audio_format.h"

#include "arm_math.h"

int mean_silence_ch1;
int mean_silence_ch2;

int energy_silence_ch1;
int mean_silence_ch1;

/* use to detect microphones zero-level values */
void calibrate_mean(Audio_Data *data);
q15_t* evl_shift(Audio_Data *data);
int max_index(q15_t* corr);
int calc_shift(Audio_Data *data, q15_t** gde_data);
int to_grad(int corr);
#endif // _CORR_H
