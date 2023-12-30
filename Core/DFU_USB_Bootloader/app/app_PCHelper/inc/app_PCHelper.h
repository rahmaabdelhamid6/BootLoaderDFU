/*
 * app_PCHelper.h
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */

#ifndef DFU_USB_BOOTLOADER_APP_APP_PCHELPER_INC_APP_PCHELPER_H_
#define DFU_USB_BOOTLOADER_APP_APP_PCHELPER_INC_APP_PCHELPER_H_

#define			PCHELPER_MAX_MSG_SIZE_IN_BYTES								32U
extern uint32_t  g_ui32StartReadAddress;

extern uint32_t g_ui32StartWriteAddress;

extern uint16_t g_ui16NumberOfPages;

extern uint16_t g_ui16PageNumber;


extern uint32_t g_ui32ModifiedAddress;

extern uint8_t  g_aui8WriteData[PCHELPER_MAX_MSG_SIZE_IN_BYTES] ;


/******************************************************************************/
// PC Helper Commands Types
#define			PCHELPER_REQUEST_GET_VERSION				0x01U
#define			PCHELPER_REQUEST_GO					        0x02U
#define			PCHELPER_REQUEST_ERASE_ENTIRE_FLASH       	0x03U
#define			PCHELPER_REQUEST_ERASE_PAGES				0x04U
#define			PCHELPER_REQUEST_READ_MEMORY  				0x05U
#define			PCHELPER_REQUEST_WRITE_MEMORY				0x06U
#define			PCHELPER_REQUEST_MODIFY_APP_ADDRESS			0x07U
#define         PCHELPER_REQUEST_UPDATE_BOOTLOADER          0x08U

/*****************************************************************************/

//BootLoader Command Types

#define			PCHELPER_BOOTLOADER_ACK_MSG_TYPE						0xAAU
#define			PCHELPER_BOOTLOADER_NACK_MSG_TYPE						0xBBU
#define			PCHELPER_BOOTLOADER_REPLY_GET_VERSION_MSG_TYPE			0xA1U
#define			PCHELPER_BOOTLOADER_REPLY_READ_MEMORY_MSG_TYPE			0XA5U

/************************************************************************************/
#define			PCHELPER_PC_MSG_SYNQ_SIZE_IN_BYTES							1U
#define			PCHELPER_PC_MSG_CHECKSUM_SIZE_IN_BYTES						1U
#define			PCHELPER_PC_MSG_MSGTYPE_INDEX								2U
#define 		PCHELPER_PC_MSG_SYNQ_BYTE									0x55U


#define			PCHELPER_BOOTLOADER_MSG_SYNQ_SIZE_IN_BYTES							1U
#define			PCHELPER_BOOTLOADER_MSG_CHECKSUM_SIZE_IN_BYTES						1U
#define			PCHELPER_BOOTLOADER_MSG_MSGTYPE_INDEX								2U
#define 		PCHELPER_BOOTLOADER_MSG_SYNQ_BYTE									0x55U


/***********************************************************************************/
typedef enum
{
	PCHELPER_SUCCESS,
	PCHELPER_ERROR_CHECKSUM,
	PCHELPER_ERROR_INVALID_MSG_TYPE

}PCHelperStatus_et;

////PCConfig Msg Structure
//typedef	struct
//{
//	uint8_t		ui8GetVersion;
//	uint8_t		ui8GO;
//	uint8_t		ui8EraseEntireFlash;
//	uint8_t		ui8ErasePages;
//	uint8_t	    ui8ReadMemory;
//	uint8_t	    ui8WriteMemory;
//	uint8_t	    ui8ModifyAppAddress;
//
//}PCConfig_st;
/*********************************************************************************/
/* Commands Structrue*/

/*from PC to BootLoader*/

/* Get Version Command*/
typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t		ui8CheckSum;
}PCGetVersion_st;

/* GO Command*/
typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t		ui8CheckSum;
}PCGO_st;

/* Erase Entire Flash Command */
typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t		ui8CheckSum;
}PCEraseEntireFlash_st;


/* Erase Pages Command */
typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t	    ui8PageNo[2];   //Data[0,1]
	uint8_t  	ui8NoOfPages[2];   ////Data[2,3]
	uint8_t		ui8CheckSum;
}PCErasePages_st;

/*Read Memory Command*/
typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t	     ui8Data[4];    //StartAddress
//	uint16_t	ui16NoOfPages;
	uint8_t		ui8CheckSum;
}PCReadMemory_st;


/*Write Memory Command*/

typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t	    ui8DataStartAddress[4];    //StartAddress
	uint8_t     ui8Data[32];  //data
	uint8_t		ui8CheckSum;
}PCWriteMemory_st;


/*Modify App Address Command*/

typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t	     ui8Data[4];    //New Application Address
	uint8_t		ui8CheckSum;
}PCModifyAppAddress_st;

/* Get Version Command*/
typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t		ui8CheckSum;
}PCUpdateBootLoader_st;

/********************************************************************************/
/*from BootLoader to PC*/

/*ACK & NACK Command*/

typedef	struct
{
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t		ui8CheckSum;
}BootLoaderACKNACK_st;


/*Reply Get Version  command*/
typedef	struct
{

	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t    ui8Data[4];   //Current Bootloader Version
	uint8_t		ui8CheckSum;
}BootLoaderReplyGetVersion_st;


/*Reply Read Memory command*/
typedef	struct
{
//	uint8_t     ui8Data[32];
	uint8_t		ui8Synq;
	uint8_t		ui8Len;
	uint8_t		ui8MsgType;
	uint8_t     ui8Data[32];
	uint8_t		ui8CheckSum;
}BootLoaderReplyReadMemory_st;


// PC Helper Prototypes
PCHelperStatus_et	_APP_PCHelper_CalculateCheckSum (void* 		        pPCMsg,
	                             	                 uint8_t		    ui8PCMsgSize,
		                                             uint8_t*	        pui8CheckSum);

PCHelperStatus_et	APP_PCHelper_ParseMsg (void*     pPCRxMsg ,
		                                   uint8_t	 ui8PCRxMsgSize,
		                                   uint8_t   *pui8MsgType);


PCHelperStatus_et	APP_PCHelper_ConstructMsg (void* 			pPCMsg ,
		                                       uint8_t*			pui8MsgSize,
	                                           uint8_t			ui8Msgtype,
											   uint32_t         ui32BTVersion,
											   uint8_t          aui8ReadData[PCHELPER_MAX_MSG_SIZE_IN_BYTES]);





#endif /* DFU_USB_BOOTLOADER_APP_APP_PCHELPER_INC_APP_PCHELPER_H_ */
