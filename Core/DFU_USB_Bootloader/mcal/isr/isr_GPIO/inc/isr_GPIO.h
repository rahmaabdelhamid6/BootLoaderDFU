/*
 * isr_GPIO.h
 *
 *  Created on: Dec 22, 2023
 *      Author: Rahma
 */

#ifndef DFU_USB_BOOTLOADER_MCAL_ISR_ISR_GPIO_INC_ISR_GPIO_H_
#define DFU_USB_BOOTLOADER_MCAL_ISR_ISR_GPIO_INC_ISR_GPIO_H_

extern void (*g_pfnButtonCallback) (void);


void MCAL_GPIO_DetecStateCallback (uint16_t GPIO_Pin , void (*pfnBtnCallback) (void));

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* DFU_USB_BOOTLOADER_MCAL_ISR_ISR_GPIO_INC_ISR_GPIO_H_ */
