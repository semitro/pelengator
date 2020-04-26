#include "corr.h"

q15_t out[AUDIO_DATA_LEN*2];
q15_t ch1[AUDIO_DATA_LEN];
q15_t ch2[AUDIO_DATA_LEN];

q15_t* eval_shift(Audio_Data *data){
	for(int i = 0; i < data->len; i++) {
			ch1[i] = data->data[i].ch1 - 1920;
			ch2[i] = data->data[i].ch2 - 1920;
		}
	arm_correlate_fast_q15(ch1, AUDIO_DATA_LEN, ch2, AUDIO_DATA_LEN, out);
	return out;
}


int calc_shift(Audio_Data *data){
	for(int i = 0; i < data->len; i++) {
			ch1[i] = data->data[i].ch1 - 1920;
			ch2[i] = data->data[i].ch2 - 1920;
		}
	arm_correlate_fast_q15(ch1, AUDIO_DATA_LEN, ch2, AUDIO_DATA_LEN, out);
	q15_t maximum = 0;
	q15_t maximum_idx = 0;
	arm_max_q15(out, AUDIO_DATA_LEN*2, &maximum, &maximum_idx);

	return (int)maximum_idx - AUDIO_DATA_LEN;
}


int left_grads[] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 90};
int right_grads[] = {0, -16, -32, -48, -64, -80, -90};

int to_grad(int corr) {
	if(corr > 0) {
		if(corr > 12) {
			corr = 12;
		}
		return left_grads[corr];
	} else {
		corr = -corr;
		if(corr < 6) {
			corr = 6;
			return right_grads[corr];
		}
	}


 return 0;
}
