#include "debug_helper.h"

void print_debug(char* msg)
{
#ifdef DEBUG_MODE
	  HAL_UART_Transmit(&huart1, msg, 128, 100);
#endif // DEBUG_MODE
}

void print_debug_uint(uint16_t digit)
{
	  char string[100] = {};
	  size_t bytes_written = sprintf(string, "%d\n\r", digit);
	  HAL_UART_Transmit(&huart1, string, bytes_written, 100);
}


