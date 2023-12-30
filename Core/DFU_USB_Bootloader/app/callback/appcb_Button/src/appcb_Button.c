/*
 * appcb_Button.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */


#include "main.h"
#include "appcb_Button.h"
#include "hal_Button.h"



void APPCB_Button(void)
{
	//	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET)
	//	{
	//		g_etui8BtnState=PRESSED;
	//	}
	//	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_SET)
	//	{
	//		g_etui8BtnState=RELEASED;
	//	}
	g_etui8BtnState++;
	if(g_etui8BtnState>PRESSED)
	{
		g_etui8BtnState=RELEASED;
	}
}
