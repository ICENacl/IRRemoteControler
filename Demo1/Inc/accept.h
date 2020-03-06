#include "main.h"
#include "math.h"
#include "stdlib.h"
#include <stdarg.h>
#include <string.h>
#include "stm32f1xx.h"

#define Width 400
/*
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;
*/
extern uint16_t Data[300];
extern  uint16_t Data_cnt;
extern uint8_t start_flag;
#define  using_button  1

void start_accept();
void stop_accept();
void UartPrint(UART_HandleTypeDef *huart,char *s,...);

