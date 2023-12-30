/*
 * appcb_PC.h
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */

#ifndef DFU_USB_BOOTLOADER_APP_CALLBACK_APPCB_PC_INC_APPCB_PC_H_
#define DFU_USB_BOOTLOADER_APP_CALLBACK_APPCB_PC_INC_APPCB_PC_H_

#define YES		0U
#define NO		1U


extern uint8_t	g_ui8PCTxChannelIdle;
extern uint8_t	g_ui8PCRxChannelIdle;
extern uint8_t  g_ui8PCRxMsgLen;

void APPCB_PC_MsgReceived (uint32_t ui32RxMsgLen);


void APPCB_PC_MsgSent ();

#endif /* DFU_USB_BOOTLOADER_APP_CALLBACK_APPCB_PC_INC_APPCB_PC_H_ */
