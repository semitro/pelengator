#ifndef _DEBUG_HEPER_H
#define _DEBUG_HEPER_H

#include "usart.h"
#include "audio_format.h"

void print_debug(char* msg);
void print_debug_uint(uint16_t digit);
void print_debug_ch1(Audio_Data *data);
void print_debug_ch2(Audio_Data *data);
void print_debug_array(int16_t* arr, size_t len);

#endif // _DEBUG_HEPER_H
