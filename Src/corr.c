#include "corr.h"

q15_t out[AUDIO_DATA_LEN*2];
q15_t ch1[AUDIO_DATA_LEN];
q15_t ch2[AUDIO_DATA_LEN];

void calibrate_mean(Audio_Data *data){
	uint32_t acc1 = 0;
	uint32_t acc2 = 0;
	for(int i = 0; i < data->len; i++) {
		acc1 += data->data[i].ch1;
		acc2 += data->data[i].ch2;
	}
	mean_silence_ch1 = acc1/data->len;
	mean_silence_ch2 = acc1/data->len;

}

int is_silence(Audio_Data* data){
	for(int i = 0; i < data->len; i++) {
		if(data->data[i].ch1 - mean_silence_ch1 > 20){
			return 0;
		}
	}
	return 1;
}

q15_t* eval_shift(Audio_Data *data){
	for(int i = 0; i < data->len; i++) {
			ch1[i] = data->data[i].ch1 - mean_silence_ch1;
			ch2[i] = data->data[i].ch2 - mean_silence_ch2;
		}
		arm_correlate_q7(ch1, AUDIO_DATA_LEN/4, ch2, AUDIO_DATA_LEN/4, out);
//		arm_conv_partial_fast_q15(ch1, AUDIO_DATA_LEN/4, ch2, AUDIO_DATA_LEN/4, out, 0, MAX_SHIFT*2);
	return out;
}

int max_index(q15_t* corr) {
	int32_t max = 0;
	int max_idx = 31;
	for(int i = 31 - 15; i < 64; i++){
		if(corr[i] > max){
			max = corr[i];
			max_idx = i;
		}
	}
	return max_idx - 31;

}


int calc_shift(Audio_Data *data, q15_t** gde_data){
	for(int i = 0; i < data->len; i++) {
			ch1[i] = data->data[i].ch1 - mean_silence_ch1;
			ch2[i] = data->data[i].ch2 - mean_silence_ch2;
		}
#define MAX_SHIFT 15
//	arm_correlate_fast_q15(ch1, AUDIO_DATA_LEN, ch2, AUDIO_DATA_LEN, out);
	arm_correlate_q7(ch1, AUDIO_DATA_LEN/4, ch2, AUDIO_DATA_LEN/4, out);
	q15_t maximum = 0;
	uint32_t maximum_idx = 0;
	arm_max_q7(out, 64, &maximum, &maximum_idx);
	gde_data = out;
	return (int)maximum_idx - 32;
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
