/*
 * hal_Button.h
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */

#ifndef DFU_USB_BOOTLOADER_HAL_HAL_BUTTON_INC_HAL_BUTTON_H_
#define DFU_USB_BOOTLOADER_HAL_HAL_BUTTON_INC_HAL_BUTTON_H_

typedef enum{

	RELEASED=0,
	PRESSED=1
}ButtonState_et;

extern ButtonState_et g_etui8BtnState;

void HAL_Button_Init(uint16_t GPIO_Pin,void (*pfnBtnCallback)(void));

//ButtonState_et HAL_Button_GetState(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

//void HAL_Button_Write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);


#endif /* DFU_USB_BOOTLOADER_HAL_HAL_BUTTON_INC_HAL_BUTTON_H_ */
