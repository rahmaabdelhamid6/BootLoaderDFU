/*
 * hal_Button.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */


#include "main.h"
#include "hal_Button.h"
#include "isr_GPIO.h"


 ButtonState_et g_etui8BtnState=RELEASED;

void HAL_Button_Init(uint16_t GPIO_Pin,void (*pfnBtnCallback)(void))
{
	MCAL_GPIO_DetecStateCallback (GPIO_Pin ,pfnBtnCallback);
}

//ButtonState_et HAL_Button_GetState(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
//{
//	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==GPIO_PIN_SET)
//	{
//		g_etui8BtnState=PRESSED;
//	}
//	else if((HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==GPIO_PIN_RESET))
//	{
//		g_etui8BtnState=RELEASED;
//	}
//
//	return g_etui8BtnState;
//}
//
//void HAL_Button_Write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
//{
//	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
//}
