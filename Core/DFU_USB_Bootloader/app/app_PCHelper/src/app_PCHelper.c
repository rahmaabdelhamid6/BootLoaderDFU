/*
 * app_PCHelper.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */

#include "stdint.h"
#include "string.h"
#include "app_PCHelper.h"
#include "hal_Flash.h"


uint32_t g_ui32StartWriteAddress=0;

uint32_t g_ui32StartReadAddress;

uint16_t g_ui16NumberOfPages=0;

uint16_t g_ui16PageNumber=0;

uint32_t g_ui32ModifiedAddress;

uint8_t  g_aui8WriteData[PCHELPER_MAX_MSG_SIZE_IN_BYTES] ;

PCHelperStatus_et	_APP_PCHelper_CalculateCheckSum (void* 		        pPCMsg,
		uint8_t		    ui8PCMsgSize,
		uint8_t*	        pui8CheckSum)


{
	PCHelperStatus_et		etPCHelperStatus = PCHELPER_SUCCESS;
	uint8_t					*pui8Buffer = (uint8_t*)pPCMsg;
	uint32_t				ui32Size = (ui8PCMsgSize - (PCHELPER_PC_MSG_CHECKSUM_SIZE_IN_BYTES));
	uint8_t					ui8CheckSum = 0;
	uint32_t				ui32Index;


	for (ui32Index = 0 ; ui32Index < ui32Size ; ui32Index++)
	{
		ui8CheckSum += pui8Buffer[ui32Index];
	}


	*pui8CheckSum = ui8CheckSum;


	return etPCHelperStatus;

}


PCHelperStatus_et	APP_PCHelper_ParseMsg (void*     pPCRxMsg ,
		uint8_t	 ui8PCRxMsgSize,
		uint8_t   *pui8MsgType)
{
	PCHelperStatus_et		etPCHelperStatus = PCHELPER_SUCCESS;
	uint8_t				ui8CalculatedCheckSum = 0;
	uint8_t				ui8PCRxCheckSum = 0;
	uint32_t *pui32ReadData;

	uint32_t *pui32ModifyAdd;

	uint32_t *pui32WriteAdd;

	uint16_t *pui16PageNo;

	uint16_t *pui16NoOfPages;

	//	PCGetVersion_st *        pstPCGetVersion;
	//	PCGO_st *                pstPCGO;
	//	PCEraseEntireFlash_st *  pstPCEraseEntireFlash;
	PCErasePages_st *        pstPCErasePages;
	PCReadMemory_st *        pstPCReadMemory;
	PCWriteMemory_st *       pstPCWriteMemory;
	PCModifyAppAddress_st *  pstPCModifyAppAddress;
//	PCUpdateBootLoader_st  *  pstPCUpdateBL;


	// Calculate Check sum
	_APP_PCHelper_CalculateCheckSum(pPCRxMsg, ui8PCRxMsgSize, &ui8CalculatedCheckSum);

	ui8PCRxCheckSum = ((uint8_t*)pPCRxMsg)[ui8PCRxMsgSize - 1] ;

	// Verify Check Sum
	if (ui8PCRxCheckSum != ui8CalculatedCheckSum )
	{
		return PCHELPER_ERROR_CHECKSUM;
	}
	// Check Msg Type
	switch (((uint8_t *)pPCRxMsg)[PCHELPER_PC_MSG_MSGTYPE_INDEX])
	{
	case PCHELPER_REQUEST_GET_VERSION:

		// Assign PC Get version Msg Buffer
		//		pstPCGetVersion = ((PCGetVersion_st *) pPCRxMsg);

		// Assign Msg Type
		*pui8MsgType = PCHELPER_REQUEST_GET_VERSION;


		break;
	case PCHELPER_REQUEST_GO:

		// Assign Go Msg Buffer
		//		pstPCGO = ((PCGO_st*) pPCRxMsg);

		// Assign Msg Type
		*pui8MsgType = PCHELPER_REQUEST_GO;


		break;
	case PCHELPER_REQUEST_ERASE_ENTIRE_FLASH:
		// Assign Go Msg Buffer
		//		pstPCEraseEntireFlash = ((PCEraseEntireFlash_st*) pPCRxMsg);

		// Assign Msg Type
		*pui8MsgType = PCHELPER_REQUEST_ERASE_ENTIRE_FLASH;

		break;
	case PCHELPER_REQUEST_ERASE_PAGES:
		// Assign Go Msg Buffer
		pstPCErasePages = ((PCErasePages_st*) pPCRxMsg);

		// Assign Msg Type
		*pui8MsgType = PCHELPER_REQUEST_ERASE_PAGES;

//		g_ui16PageNumber=pstPCErasePages->ui16PageNo;

		pui16PageNo = (uint16_t *)pstPCErasePages->ui8PageNo;
		g_ui16PageNumber = *pui16PageNo;
		g_ui16PageNumber= (g_ui16PageNumber>>8)|(g_ui16PageNumber<<8);

//		g_ui16NumberOfPages=pstPCErasePages->ui16NoOfPages;

		pui16NoOfPages=(uint16_t *)pstPCErasePages->ui8NoOfPages;
		g_ui16NumberOfPages = *pui16NoOfPages;
		g_ui16NumberOfPages= (g_ui16NumberOfPages>>8)|(g_ui16NumberOfPages<<8);

		break;
	case PCHELPER_REQUEST_READ_MEMORY:
		// Assign Go Msg Buffer
		pstPCReadMemory = ((PCReadMemory_st*) pPCRxMsg);

		// Assign Msg Type
		*pui8MsgType = PCHELPER_REQUEST_READ_MEMORY;

		//		g_ui32StartReadAddress= pstPCReadMemory->ui32Data;

		pui32ReadData=(uint32_t *)pstPCReadMemory->ui8Data;
		g_ui32StartReadAddress = *pui32ReadData;
		g_ui32StartReadAddress= (g_ui32StartReadAddress>>8)|(g_ui32StartReadAddress<<24);

		break;
	case PCHELPER_REQUEST_WRITE_MEMORY:
		// Assign Go Msg Buffer
		pstPCWriteMemory = ((PCWriteMemory_st*) pPCRxMsg);

		// Assign Msg Type
		*pui8MsgType = PCHELPER_REQUEST_WRITE_MEMORY;

		//		g_ui32StartWriteAddress= pstPCWriteMemory->ui32DataStartAddress;

		pui32WriteAdd=(uint32_t *)pstPCWriteMemory->ui8DataStartAddress;
		g_ui32StartWriteAddress = *pui32WriteAdd;
		g_ui32StartWriteAddress= (g_ui32StartWriteAddress>>8)|(g_ui32StartWriteAddress<<24);

		memcpy(g_aui8WriteData , (void *)&pstPCWriteMemory->ui8Data , PCHELPER_MAX_MSG_SIZE_IN_BYTES);


		break;
	case PCHELPER_REQUEST_MODIFY_APP_ADDRESS:
		// Assign Go Msg Buffer
		pstPCModifyAppAddress = ((PCModifyAppAddress_st*) pPCRxMsg);

		// Assign Msg Type
		*pui8MsgType = PCHELPER_REQUEST_MODIFY_APP_ADDRESS;

		pui32ModifyAdd=(uint32_t *)pstPCModifyAppAddress->ui8Data;
		g_ui32ModifiedAddress = *pui32ModifyAdd;
		g_ui32ModifiedAddress= (g_ui32ModifiedAddress>>8)|(g_ui32ModifiedAddress<<24);

		break;
	case PCHELPER_REQUEST_UPDATE_BOOTLOADER:

			// Assign Msg Type
			*pui8MsgType = PCHELPER_REQUEST_UPDATE_BOOTLOADER;
			break;

	default:
		return PCHELPER_ERROR_INVALID_MSG_TYPE;
		break;
	}



	return etPCHelperStatus;
}


PCHelperStatus_et	APP_PCHelper_ConstructMsg (void* 			pPCMsg ,
		uint8_t*			pui8MsgSize,
		uint8_t			ui8Msgtype,
		uint32_t         ui32BTVersion,
		uint8_t          aui8ReadData[PCHELPER_MAX_MSG_SIZE_IN_BYTES])
{
	PCHelperStatus_et		etPCHelperStatus = PCHELPER_SUCCESS;
	uint8_t				    ui8CheckSum = 0;
	uint8_t					ui8MsgLen = 0;
	uint32_t *paui32Data;

	BootLoaderACKNACK_st * pstBootLoaderACKNACK;
	BootLoaderReplyGetVersion_st * pstBootLoaderReplyGetVersion;
	BootLoaderReplyReadMemory_st * pstBootLoaderReplyReadMemory;


	// Check Msg Type
	switch (ui8Msgtype)
	{
	case PCHELPER_BOOTLOADER_ACK_MSG_TYPE:
	case PCHELPER_BOOTLOADER_NACK_MSG_TYPE:

		// Assign bootloader ACK OR NACK Msg Buffer
		pstBootLoaderACKNACK = (BootLoaderACKNACK_st *)pPCMsg;

		// Assign Msg Len
		ui8MsgLen =  sizeof (BootLoaderACKNACK_st);

		// Reset bootloader ACK Msg Buffer
		memset (pstBootLoaderACKNACK , 0 , sizeof (BootLoaderACKNACK_st));

		// Fill bootloader ACK Msg Buffer with ACK
		pstBootLoaderACKNACK->ui8Synq = PCHELPER_BOOTLOADER_MSG_SYNQ_BYTE;
		pstBootLoaderACKNACK->ui8Len =  ( sizeof (BootLoaderACKNACK_st)-0x2);
		pstBootLoaderACKNACK->ui8MsgType = ui8Msgtype;

		// Calculate Check sum
		_APP_PCHelper_CalculateCheckSum(pPCMsg, ui8MsgLen, &ui8CheckSum);

		pstBootLoaderACKNACK->ui8CheckSum = ui8CheckSum;

		break;

	case PCHELPER_BOOTLOADER_REPLY_GET_VERSION_MSG_TYPE:

		pstBootLoaderReplyGetVersion = (BootLoaderReplyGetVersion_st *)pPCMsg;

		// Assign Msg Len
		ui8MsgLen =  sizeof (BootLoaderReplyGetVersion_st);

		// Reset bootloader reply get version Msg Buffer
		memset (pstBootLoaderReplyGetVersion , 0 , sizeof (BootLoaderReplyGetVersion_st));

		// Fill bootloader reply get version Buffer with message
		pstBootLoaderReplyGetVersion->ui8Synq = PCHELPER_BOOTLOADER_MSG_SYNQ_BYTE;
		pstBootLoaderReplyGetVersion->ui8Len =  (sizeof (BootLoaderReplyGetVersion_st)-0x2);
		pstBootLoaderReplyGetVersion->ui8MsgType = ui8Msgtype;

		paui32Data=(uint32_t *)pstBootLoaderReplyGetVersion->ui8Data;

		ui32BTVersion= (ui32BTVersion>>8)|(ui32BTVersion<<24);
		*paui32Data= ui32BTVersion;

		// Calculate Check sum
		_APP_PCHelper_CalculateCheckSum(pPCMsg, ui8MsgLen, &ui8CheckSum);

		pstBootLoaderReplyGetVersion->ui8CheckSum = ui8CheckSum;

		break;

	case PCHELPER_BOOTLOADER_REPLY_READ_MEMORY_MSG_TYPE:

		pstBootLoaderReplyReadMemory = (BootLoaderReplyReadMemory_st *)pPCMsg;

		// Assign Msg Len
		ui8MsgLen =  sizeof (BootLoaderReplyReadMemory_st);
		//		ui8MsgLen= sizeof(aui8ReadData[PCHELPER_MAX_MSG_SIZE_IN_BYTES]);

		// Reset bootloader reply read memory Msg Buffer
		memset (pstBootLoaderReplyReadMemory , 0 , sizeof (BootLoaderReplyReadMemory_st));

		// Fill bootloader reply read memory Buffer with message
		pstBootLoaderReplyReadMemory->ui8Synq = PCHELPER_BOOTLOADER_MSG_SYNQ_BYTE;
		pstBootLoaderReplyReadMemory->ui8Len =  (sizeof (BootLoaderReplyReadMemory_st)-0x2);
		pstBootLoaderReplyReadMemory->ui8MsgType = ui8Msgtype;
		//		pstBootLoaderReplyReadMemory->ui8Data = &aui8ReadData;
		memcpy((void *)&pstBootLoaderReplyReadMemory->ui8Data , aui8ReadData ,PCHELPER_MAX_MSG_SIZE_IN_BYTES);


		// Calculate Check sum
		_APP_PCHelper_CalculateCheckSum(pPCMsg, ui8MsgLen, &ui8CheckSum);

		pstBootLoaderReplyReadMemory->ui8CheckSum = ui8CheckSum;

		break;
	default:
		return PCHELPER_ERROR_INVALID_MSG_TYPE;
		break;

	}

	// Assign Msg Size
	*pui8MsgSize = ui8MsgLen;

	return etPCHelperStatus;
}
