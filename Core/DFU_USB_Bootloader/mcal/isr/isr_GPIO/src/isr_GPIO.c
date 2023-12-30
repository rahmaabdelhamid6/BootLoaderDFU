/*
 * isr_GPIO.c
 *
 *  Created on: Dec 22, 2023
 *      Author: Rahma
 */


#include "main.h"
#include "isr_GPIO.h"

void (*g_pfnButtonCallback) (void)= NULL;
uint16_t g_ui8GPIOPinNumber;

void MCAL_GPIO_DetecStateCallback (uint16_t GPIO_Pin , void (*pfnBtnCallback) (void))
{
	g_ui8GPIOPinNumber=GPIO_Pin;
	g_pfnButtonCallback = pfnBtnCallback;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if (GPIO_Pin == g_ui8GPIOPinNumber )
	{
		if (g_pfnButtonCallback != NULL)
		{
			g_pfnButtonCallback();
		}
		else
		{
			/*do nothing*/
		}
	}
}
