/*
 * isr_USB.h
 *
 *  Created on: Dec 22, 2023
 *      Author: Rahma
 */

#ifndef DFU_USB_BOOTLOADER_MCAL_ISR_ISR_USB_INC_ISR_USB_H_
#define DFU_USB_BOOTLOADER_MCAL_ISR_ISR_USB_INC_ISR_USB_H_

// Pointer To callback functions
// USB Tx
extern  void (*g_pfnUSBTxCpltCallback) (void) ;

// USB Rx
extern  void (*g_pfnUSBRxCpltCallback) ( uint32_t);


#endif /* DFU_USB_BOOTLOADER_MCAL_ISR_ISR_USB_INC_ISR_USB_H_ */
