#include "fft.h"

int is_there_whistle(Audio_Data* data){
	int16_t* freq_ch1 = fft_ch1(data);
	for(int i = 55; i < 115; ++i){
		if(freq_ch1[i] > 5 || freq_ch1[i] < -5) {
			return 1;
		}
	}
	return 0;
}


void fft(q15_t* src, q15_t* dst) {
	static 	arm_status status;
	static arm_rfft_instance_q15 fft_instance;
//	arm_rfft_q15(&arm_cfft_sR_q15_len256, src, dst);
	status = arm_rfft_init_q15(&fft_instance, 256/*bin count*/, 0/*forward FFT*/, 1/*output bit order is normal*/);
	arm_rfft_q15(&fft_instance, src, dst);
	arm_abs_q15(dst, dst, FFT_LEN);
}

static q15_t ch1_src[AUDIO_DATA_LEN];
static q15_t ch1_freq[FFT_LEN*2];


q15_t* fft_ch1(Audio_Data *data) {
	for(int i = 0; i < data->len; i++) {
		ch1_src[i] = data->data[i].ch1 - 1900;

	}
	fft(ch1_src, ch1_freq);
	return ch1_freq;
}
