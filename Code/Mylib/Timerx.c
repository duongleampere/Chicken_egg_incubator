#include "Timerx.h"

void TIM_Config()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period= 2000;
    TIM_TimeBaseStructure.TIM_Prescaler=41999;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
    TIM_Cmd(TIM4,ENABLE);
}
/* Interrupt Configuration */
void Interrupt_Config()
{
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
