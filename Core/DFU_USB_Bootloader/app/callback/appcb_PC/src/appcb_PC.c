/*
 * appcb_PC.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */


#include "main.h"
#include "appcb_PC.h"

/* PC Msg Sent */
uint8_t	g_ui8PCTxChannelIdle = NO;
uint8_t	g_ui8PCRxChannelIdle = NO;
uint8_t g_ui8PCRxMsgLen;

void APPCB_PC_MsgReceived (uint32_t ui32RxMsgLen)
{
	 g_ui8PCRxMsgLen= (uint8_t)ui32RxMsgLen;
 	g_ui8PCRxChannelIdle = YES;
}


void APPCB_PC_MsgSent ()
{
	g_ui8PCTxChannelIdle = YES;
}
