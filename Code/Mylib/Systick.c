/* 
June 13,2020
LV_Embedded
 */

/* Systick Configuration */
#include "Systick.h"

uint32_t SystickTimerCount;

void Systick_Configuration()
{
    /* Setup SysTick Timer for 1ms interrupt */
    if (SysTick_Config(SystemCoreClock/1000))
    {
        /* Capture error */
        NVIC_SystemReset();
    }
}

void SystickDelay_ms(uint32_t time)
{
    SystickTimerCount=time;
    while (SystickTimerCount!=0);
}

void TimingDelay_Decrement(void)
{
    if (SystickTimerCount!=0)
    {
        SystickTimerCount--;
    }
}