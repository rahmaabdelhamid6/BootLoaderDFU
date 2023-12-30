/*
 * hal_PC.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */


#include "main.h"
#include "hal_PC.h"
#include "isr_USB.h"
//#include "usbd_cdc_if.h"

// Usb Instance
extern USBD_HandleTypeDef hUsbDeviceFS;

// Rx Buffer
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

// Usb Rx Buffer Read/Write Index variables
volatile uint32_t g_ui32PCRxBufferWriteIndex = 0;

volatile uint32_t g_ui32PCRxBufferReadIndex = 0;


// USB TX/RX PFN application Callback

void (*g_pfnAPPPCMsgRxCpltCallback)(uint32_t ui32RxLen) = NULL;
void (*g_pfnAPPPCMsgTxCpltCallback)(void) = NULL;



static void _HAL_PC_RxCpltCallback (uint32_t ui32RxLen)
{
	g_ui32PCRxBufferWriteIndex = (g_ui32PCRxBufferWriteIndex + 64) % (APP_RX_DATA_SIZE); /* Increment Buffer Index by USB PAcket Size */
	USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &UserRxBufferFS[g_ui32PCRxBufferWriteIndex]);
	USBD_CDC_ReceivePacket(&hUsbDeviceFS);

	if (g_pfnAPPPCMsgRxCpltCallback!= NULL)
	{
		g_pfnAPPPCMsgRxCpltCallback(ui32RxLen);
	}

}

static void _HAL_PC_TxCpltCallback (void)
{

	if (g_pfnAPPPCMsgTxCpltCallback!= NULL)
	{
		g_pfnAPPPCMsgTxCpltCallback();
	}

}


uint32_t HAL_PC_init (void (*pfnPCRxMsgCpltCallback)(uint32_t ui32RxLen) , void (*pfnPCTxMsgCpltCallback) (void))
{

	uint32_t ui32Status = 0;

	// TODO: Set PC USB HAL Rx Callback (Pfn) to _HAL_PC_RxCpltCallback function
	g_pfnUSBRxCpltCallback = _HAL_PC_RxCpltCallback;

	// TODO: Set PC USB HAL Tx Callback (Pfn) to _HAL_PC_TxCpltCallback function
	g_pfnUSBTxCpltCallback = _HAL_PC_TxCpltCallback;


	// TODO: Set PC USB APP Tx Callback (Pfn) to pfnPCTxMsgCpltCallback function
	g_pfnAPPPCMsgTxCpltCallback = pfnPCTxMsgCpltCallback;

	// TODO: Set PC USB APP Rx Callback (Pfn) to pfnPCRxMsgCpltCallback function
	g_pfnAPPPCMsgRxCpltCallback = pfnPCRxMsgCpltCallback;

	// TODO : Reset PC Rx Buffer Read/Write Index
	g_ui32PCRxBufferWriteIndex = 0;

	g_ui32PCRxBufferReadIndex = 0;


	return ui32Status;

}



uint32_t HAL_PC_read (uint8_t * pui8Buffer , uint16_t ui16DataSize)   //recieve
{
	uint32_t ui32Status = 0;
	memset(pui8Buffer, '\0',ui16DataSize);
	// TODO: Copy data to the buffer
	memcpy((void *) pui8Buffer , (void *) &UserRxBufferFS[g_ui32PCRxBufferReadIndex] , ui16DataSize);

	memset(UserRxBufferFS, '\0',ui16DataSize);

	// TODO: Increment Read Index
	g_ui32PCRxBufferReadIndex = (g_ui32PCRxBufferReadIndex + 64) % (APP_RX_DATA_SIZE);


	return ui32Status;
}


uint32_t HAL_PC_write (uint8_t * pui8Buffer , uint16_t ui16DataSize)  //transmit
{
	uint32_t ui32Status = 0;


	//	// check if PC is connected
	//	if (HAL_PC_isConnected() == HAL_PC_CONNECTED)
	//	{
	//	memset (pui8Buffer ,'\0', ui16DataSize);
	// TODO: Send PC Message
	//	}
	CDC_Transmit_FS((uint8_t*)pui8Buffer, ui16DataSize);


	return ui32Status;
}

//uint8_t HAL_PC_isConnected()
//{
//	uint8_t ui8ConnectionStatus ;
//
//	if (HAL_GPIO_ReadPin(GPIOx,Pin) == GPIO_PIN_RESET)
//	{
//		ui8ConnectionStatus = HAL_PC_DISCONNECTED;
//	}
//	else
//	{
//		ui8ConnectionStatus = HAL_PC_CONNECTED;
//	}
//
//
//	return ui8ConnectionStatus;
//}
