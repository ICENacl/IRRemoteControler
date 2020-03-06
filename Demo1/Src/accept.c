#include "accept.h"


uint16_t Dval_Falling=0;
uint16_t Dval_Rising=0;
uint16_t Data[300];
uint16_t Data_cnt=0;

char start[]=" Start Accept !";
char stop[] = "Stop Accept!";

uint8_t start_flag=0;

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;

void UartPrint(UART_HandleTypeDef *huart,char *s,...)
{
    char UartPrintBuff[256];
    va_list ap;
    char *pStr=UartPrintBuff;
    
    va_start(ap,s);
    vsprintf((char *)UartPrintBuff,s,ap);
    va_end(ap);
    
    while(*pStr != 0)
    {
        HAL_UART_Transmit(huart,pStr++,1,10);
    }
    
    
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
	
}
//捕获中断回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==GPIO_PIN_RESET)
        {
                HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
                Dval_Falling=HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_4);
                TIM_RESET_CAPTUREPOLARITY(&htim1 , TIM_CHANNEL_4);
                TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
                //__HAL_TIM_SET_COUNTER(htim,0);  
                if(Dval_Falling>=Width &&Dval_Falling <= 5000)
                {
                        Data[Data_cnt++]=Dval_Falling;
                        __HAL_TIM_SET_COUNTER(&htim1,0); 
                }
        }
        else
        {
                HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
                Dval_Rising=HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_4);
                TIM_RESET_CAPTUREPOLARITY(&htim1 , TIM_CHANNEL_4);
                TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
                if( Dval_Rising>=Width && Dval_Rising <= 5000 )
                {
                        Data[Data_cnt++]= Dval_Rising;
                        __HAL_TIM_SET_COUNTER(&htim1,0); 
                }
        }
}

//开始捕获
//按下按键，灯亮，开始捕获
void  start_accept()
{
    
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);//点亮绿色LED，关闭黄色LED，开始输入捕获
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
    HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_4);
    
    start_flag=1;
    //HAL_UART_Transmit(&huart1,(uint8_t *)start,strlen(start),50);
    UartPrint(&huart1,"Start Accept!");
    
    //初始化变量
    Dval_Falling=0;
    Dval_Rising=0;
    Data_cnt=0;
    memset(Data,0,sizeof(Data));
}

void stop_accept()
{
    
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);//与开始捕获相反
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
    HAL_TIM_IC_Stop_IT(&htim1,TIM_CHANNEL_4);
    
    start_flag=0;
    //HAL_UART_Transmit(&huart1,(uint8_t *)stop,strlen(stop),50);
    UartPrint(&huart1,"Stop Accept!");
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)// 按键中断
{
    if(GPIO_Pin == GPIO_PIN_12)
    {
        start_accept();
    }
    else if(GPIO_Pin == GPIO_PIN_2)
    {
        stop_accept();
    }
}

