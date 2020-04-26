#include "debug_helper.h"

void print_debug(char* msg)
{
//#ifdef DEBUG_MODE
	  HAL_UART_Transmit(&huart1, msg, 128, 100);
//#endif // DEBUG_MODE
}

void print_debug_uint(uint16_t digit)
{
	  char string[100] = {};
	  size_t bytes_written = sprintf(string, "%d\n\r", digit);
	  HAL_UART_Transmit(&huart1, string, bytes_written, 100);
}

void print_debug_int(int digit)
{
	  char string[100] = {};
	  size_t bytes_written = sprintf(string, "%d\n\r", digit);
	  HAL_UART_Transmit(&huart1, string, bytes_written, 100);
}


void print_debug_ch1(Audio_Data *data) {
	char string[100] = {};
	size_t written = sprintf(string, "\n\r[");
	HAL_UART_Transmit(&huart1, string, written, 100);
	string[0] = 0; string[1] = 0; string[2] = 0;
	for(int i = 0; i < data->len; i++){
		size_t bytes_written = sprintf(string, "%d,", data->data[i].ch1);
		HAL_UART_Transmit(&huart1, string, bytes_written, 100);
	}
	written = sprintf(string, "]\n\r");
	HAL_UART_Transmit(&huart1, string, written, 100);

}

void print_debug_ch2(Audio_Data *data) {
	char string[100] = {};
	size_t written = sprintf(string, "\n\r[");
	HAL_UART_Transmit(&huart1, string, written, 100);
	string[0] = 0; string[1] = 0; string[2] = 0;
	for(int i = 0; i < data->len; i++){
		size_t bytes_written = sprintf(string, "%d,", data->data[i].ch2);
		HAL_UART_Transmit(&huart1, string, bytes_written, 100);
	}
	written = sprintf(string, "]\n\r");
	HAL_UART_Transmit(&huart1, string, written, 100);

}

void print_debug_array(int16_t* arr, size_t len){
	char string[100] = {};
	size_t written = sprintf(string, "\n\r[");
	HAL_UART_Transmit(&huart1, string, written, 100);
	string[0] = 0; string[1] = 0; string[2] = 0;
	for(int i = 0; i < len; i++){
		size_t bytes_written = sprintf(string, "%d,", arr[i]);
		HAL_UART_Transmit(&huart1, string, bytes_written, 100);
	}
	written = sprintf(string, "]\n\r");
	HAL_UART_Transmit(&huart1, string, written, 100);

}
