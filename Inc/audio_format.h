#ifndef _AUDIO_FORMAT_H
#define _AUDIO_FORMAT_H

#include <stddef.h>
#include <stdint.h>

#define AUDIO_DATA_LEN 256

typedef struct Audio_Data{
	union {
		uint32_t sample;
		struct {
			uint16_t ch1;
			uint16_t ch2;
		};
	} data[AUDIO_DATA_LEN];
	size_t len;
}Audio_Data;


//#define ADC_RESOLUTION_TIME 15
//#define ADC_PRESCALER_DIVISOR 2
//#define ADC_SAMPLING_TIME 5
//#define ADC_PCLK 1000000

#define ADC_APB1 64000
#define ADC_CLOCK_PRESCALER 4

#define OLD_ADC_FREQUENCY (ADC_PCLK / (ADC_RESOLUTION_TIME + ADC_SAMPLING_TIME) / ADC_PRESCALER_DIVISOR)

#define ADC_FREQUENCY (ADC_APB1 / ADC_CLOCK_PRESCALER)
// 16 kHz
#endif // _AUDIO_FORMAT_H
