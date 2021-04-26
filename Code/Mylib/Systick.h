/* 
June 13,2020
LV_Embedded
 */

#ifndef SYSTICK_H
#define SYSTICK_H

#include "../Src/stm32f4xx.h"

void Systick_Configuration(void);
void SystickDelay_ms(uint32_t time);
void TimingDelay_Decrement(void);


#endif