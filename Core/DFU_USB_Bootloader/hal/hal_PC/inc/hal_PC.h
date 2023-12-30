/*
 * hal_PC.h
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */

#ifndef DFU_USB_BOOTLOADER_HAL_HAL_PC_INC_HAL_PC_H_
#define DFU_USB_BOOTLOADER_HAL_HAL_PC_INC_HAL_PC_H_

#define HAL_PC_CONNECTED		1U
#define HAL_PC_DISCONNECTED		0U

uint32_t HAL_PC_init (void (*pfnPCRxMsgCpltCallback)(uint32_t ui32RxLen) , void (*pfnPCTxMsgCpltCallback) (void));
uint32_t HAL_PC_read (uint8_t * pui8Buffer , uint16_t ui16DataSize);
uint32_t HAL_PC_write (uint8_t * pui8Buffer , uint16_t ui16DataSize);
//uint8_t HAL_PC_isConnected();

#endif /* DFU_USB_BOOTLOADER_HAL_HAL_PC_INC_HAL_PC_H_ */
