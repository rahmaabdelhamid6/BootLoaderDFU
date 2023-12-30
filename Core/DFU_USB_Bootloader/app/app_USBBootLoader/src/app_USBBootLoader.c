/*
 * app_USBBootLoader.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */




#include "main.h"
#include "app_USBBootLoader.h"
#include "app_PCHelper.h"
#include "usb_device.h"
//#include "usbd_cdc_if.h"
#include "usbd_dfu_if.h"
#include "hal_PC.h"
#include "hal_Button.h"
#include "hal_Flash.h"
#include "appcb_PC.h"
#include "appcb_Button.h"

static uint32_t g_ui32PreviousAppAddress= APP_START_ADDRESS;
static uint32_t g_ui32CurrentAppAddress= APP_START_ADDRESS;
static uint32_t g_ui32BTVersion=BOOTLOADER_VERSION;

uint8_t g_aui8PCRxBuff[PCHELPER_MAX_MSG_SIZE_IN_BYTES];
uint8_t g_aui8PCTxBuff[PCHELPER_MAX_MSG_SIZE_IN_BYTES];

uint32_t g_ui3StartAddData=ADDR_FLASH_PAGE_12;
uint8_t	 g_pui8CheckSum;
uint8_t	 g_pui8MsgSize;
uint8_t  g_aui8ReadData[PCHELPER_MAX_MSG_SIZE_IN_BYTES];
uint8_t  g_aui8ReadBLData[BOOTLOADER_MAX_MSG_SIZE];

uint8_t 	ui8MsgType = 0;
uint8_t ui8PCRxCheckSum=0;

//uint8_t *ui8CurrentStartAdd1= g_aui8ReadData;

uint16_t ui16PageNum=0;
uint16_t ui16NoOfPages=0;

volatile BootLoaderMood_et etui8BTMood=BOOTLOADER_MOOD ;

void APP_ModifyAppAddress(void)
{
	uint16_t ui16PageNum=0;
	uint16_t ui16NoOfPages=0;

	g_ui32PreviousAppAddress=0;
	g_ui32CurrentAppAddress=0;

	//read the app address from flash memory at last page
	HAL_Flash_Read_Data(ADDR_FLASH_PAGE_255,&g_ui32PreviousAppAddress, PCHELPER_MAX_MSG_SIZE_IN_BYTES);

	ui16PageNum=0xFF;
	ui16NoOfPages=0X1;

	//erase the last page in flash
	if(HAL_Flash_Erase_EntireorPage_Flash(ui16PageNum,ui16NoOfPages)==HAL_OK)
	{
		//write the new address in flash memory
		HAL_Flash_Write_Data(ADDR_FLASH_PAGE_255, &g_ui32ModifiedAddress, PCHELPER_MAX_MSG_SIZE_IN_BYTES);
	}

	//read the app address from flash memory at last page after modifying
	HAL_Flash_Read_Data(ADDR_FLASH_PAGE_255,&g_ui32CurrentAppAddress, PCHELPER_MAX_MSG_SIZE_IN_BYTES);
}
void APP_GoToApp(void)
{
	g_ui32CurrentAppAddress=0;
	//read the app address from flash memory at last page after modifying
	HAL_Flash_Read_Data(ADDR_FLASH_PAGE_255,&g_ui32CurrentAppAddress, PCHELPER_MAX_MSG_SIZE_IN_BYTES);

	void (*app_reset_handler) (void)= (void*) (*(volatile uint32_t *)g_ui32CurrentAppAddress +4);

	__set_MSP((*(volatile uint32_t *)g_ui32CurrentAppAddress));

	HAL_GPIO_WritePin(LED_BT_GPIO_Port, LED_BT_Pin,GPIO_PIN_SET);

	app_reset_handler();

	HAL_Delay(2000);
}

uint32_t	APP_USBBOOTLOADER_Init (void)
{

	uint32_t ui32Status = 0;

	PCHelperStatus_et etPCHelperStatus = PCHELPER_SUCCESS;
	uint16_t ui16StartAppPge=0;

	uint8_t ui8WriteState=0;
	uint16_t ui16NumOfPagesinFlash=0;
	uint32_t ui32TempAddress=0;
	uint32_t ui32CurrentStartAdd=0;

	switch (g_etui8BtnState)
	{
	case PRESSED:
		etui8BTMood=USER_APP_MODE;
		//Run USER APP
		APP_GoToApp();
		break;
	case RELEASED :

		etui8BTMood=BOOTLOADER_MOOD;
		//erase the last page in flasH

		// loop till PC Rx Channel Idle is Yes
		while(g_ui8PCRxChannelIdle == NO);

		// Set PC Rx Channel Idle to No
		g_ui8PCRxChannelIdle = NO;

		//Read command from PC
		HAL_PC_read (g_aui8PCRxBuff , g_ui8PCRxMsgLen);

		// Calculate Check sum to check if the command is valid or not
		_APP_PCHelper_CalculateCheckSum(g_aui8PCRxBuff,g_ui8PCRxMsgLen,&g_pui8CheckSum);

		ui8PCRxCheckSum = ((uint8_t*)g_aui8PCRxBuff)[g_ui8PCRxMsgLen - 1] ;

		// Verify Check Sum
		if (ui8PCRxCheckSum == g_pui8CheckSum)
		{
			// Construct PC Msg
			APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_ACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

			//send ACk
			HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);


			// loop till PC Tx Channel Idle is Yes
			while(g_ui8PCTxChannelIdle == NO);

			// Set PC Tx Channel Idle to No
			g_ui8PCTxChannelIdle = NO;

			// Parse Msg
			etPCHelperStatus = APP_PCHelper_ParseMsg (g_aui8PCRxBuff ,g_ui8PCRxMsgLen,&ui8MsgType);

			// Check If Msg Is valid
			if (etPCHelperStatus == PCHELPER_SUCCESS)
			{
				// Check Msg Type
				switch (ui8MsgType)
				{

				case PCHELPER_REQUEST_GET_VERSION:

					APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_REPLY_GET_VERSION_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

					//send ACk
					HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

					// loop till PC Tx Channel Idle is Yes
					while(g_ui8PCTxChannelIdle == NO);

					// Set PC Rx Channel Idle to No
					g_ui8PCTxChannelIdle = NO;

					break;
				case PCHELPER_REQUEST_GO:

					//Go to user application
					APP_GoToApp();

					break;

				case PCHELPER_REQUEST_ERASE_ENTIRE_FLASH:

					//Calculate start app page
					ui16StartAppPge=(g_ui32CurrentAppAddress-FLASH_BASE)/FLASH_PAGE_SIZE;

					ui16NumOfPagesinFlash=NUM_OF_FLASH_APP_PAGES_IN_BANK-ui16StartAppPge;

					//check if data is erased successfully
					while(ui16NumOfPagesinFlash<128)
					{
						if(HAL_Flash_Erase_EntireorPage_Flash(ui16StartAppPge,ui16NumOfPagesinFlash)==HAL_OK)
						{
							// Construct PC Msg
							APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_ACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

							//send ACk
							HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

							// loop till PC Tx Channel Idle is Yes
							while(g_ui8PCTxChannelIdle == NO);

							// Set PC Rx Channel Idle to No
							g_ui8PCTxChannelIdle = NO;
						}
						else
						{
							// Construct PC Msg
							APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_NACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

							//send NACk
							HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

							// loop till PC Tx Channel Idle is Yes
							while(g_ui8PCTxChannelIdle == NO);

							// Set PC Rx Channel Idle to No
							g_ui8PCTxChannelIdle = NO;
						}
						ui32TempAddress=(((NUM_OF_FLASH_APP_PAGES_IN_BANK+1)*FLASH_PAGE_SIZE)+FLASH_BASE);

						ui16StartAppPge =(ui32TempAddress-FLASH_BASE)/FLASH_PAGE_SIZE;

						ui16NumOfPagesinFlash = NUM_OF_FLASH_APP_PAGES_IN_BANK-2;
					}
					break;

				case PCHELPER_REQUEST_ERASE_PAGES:

					//check if data is erased successfully
					if(HAL_Flash_Erase_EntireorPage_Flash(g_ui16PageNumber,g_ui16NumberOfPages)==HAL_OK)
					{
						// Construct PC Msg
						APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_ACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

						//send ACk
						HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

						// loop till PC Tx Channel Idle is Yes
						while(g_ui8PCTxChannelIdle == NO);

						// Set PC Rx Channel Idle to No
						g_ui8PCTxChannelIdle = NO;
					}
					else
					{
						// Construct PC Msg
						APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_NACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

						//send NACk
						HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

						// loop till PC Tx Channel Idle is Yes
						while(g_ui8PCTxChannelIdle == NO);

						// Set PC Rx Channel Idle to No
						g_ui8PCTxChannelIdle = NO;
					}
					break;

				case PCHELPER_REQUEST_READ_MEMORY:

					ui32CurrentStartAdd=g_ui32StartReadAddress;

					while(ui32CurrentStartAdd <(g_ui32StartReadAddress+(FLASH_PAGE_SIZE-1)))
					{
						memset(g_aui8ReadData,0,PCHELPER_MAX_MSG_SIZE_IN_BYTES);
						// read 32 byte from flash memory at specfied address till the end of page
						HAL_Flash_Read_Data (g_ui32StartReadAddress, (uint32_t *)g_aui8ReadData, PCHELPER_MAX_MSG_SIZE_IN_BYTES);

						memset(g_aui8PCTxBuff,0,PCHELPER_MAX_MSG_SIZE_IN_BYTES);
						// Construct PC Msg
						APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_REPLY_READ_MEMORY_MSG_TYPE,g_ui32BTVersion,(uint8_t *)g_aui8ReadData);

						//send 32 byte
						HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

						// loop till PC Tx Channel Idle is Yes
						while(g_ui8PCTxChannelIdle == NO);

						// Set PC Rx Channel Idle to No
						g_ui8PCTxChannelIdle = NO;

						//add 32 byte on the address to read
						ui32CurrentStartAdd+=0x20;
					}
					break;

				case PCHELPER_REQUEST_WRITE_MEMORY:

					//write 32 byte in flash memory at specified address
					HAL_Flash_Write_Data (g_ui32StartWriteAddress,(uint32_t *) g_aui8WriteData, PCHELPER_MAX_MSG_SIZE_IN_BYTES);

					memset(g_aui8ReadData,'\0',PCHELPER_MAX_MSG_SIZE_IN_BYTES);

					HAL_Flash_Read_Data(g_ui32StartWriteAddress,(uint32_t *) g_aui8ReadData,PCHELPER_MAX_MSG_SIZE_IN_BYTES);

					//check if data is written successfully
					for(uint8_t i=0;i<PCHELPER_MAX_MSG_SIZE_IN_BYTES; i++)
					{
						if(g_aui8WriteData[i]==g_aui8ReadData[i])
						{
							ui8WriteState=HAL_OK;
						}
						else
						{
							ui8WriteState=HAL_ERROR;
						}
					}

					if(ui8WriteState==HAL_OK)
					{
						memset(g_aui8PCTxBuff,'\0',PCHELPER_MAX_MSG_SIZE_IN_BYTES);

						// Construct PC Msg
						APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_ACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

						//send ACk
						HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

						// loop till PC Tx Channel Idle is Yes
						while(g_ui8PCTxChannelIdle == NO);

						// Set PC Rx Channel Idle to No
						g_ui8PCTxChannelIdle = NO;
					}
					else if(ui8WriteState==HAL_ERROR)
					{
						// Construct PC Msg
						APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_NACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

						//send NACk
						HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

						// loop till PC Tx Channel Idle is Yes
						while(g_ui8PCTxChannelIdle == NO);

						// Set PC Rx Channel Idle to No
						g_ui8PCTxChannelIdle = NO;
					}
					break;
				case PCHELPER_REQUEST_MODIFY_APP_ADDRESS:

					ui16PageNum=0xFF;
					ui16NoOfPages=0X1;

					//erase the last page in flash
					if(HAL_Flash_Erase_EntireorPage_Flash(ui16PageNum,ui16NoOfPages)==HAL_OK)
					{
						//write the new address in flash memory
						HAL_Flash_Write_Data(ADDR_FLASH_PAGE_255, &g_ui32ModifiedAddress, PCHELPER_MAX_MSG_SIZE_IN_BYTES);
					}

					//read the app address from flash memory at last page after modifying
					HAL_Flash_Read_Data(ADDR_FLASH_PAGE_255,&g_ui32CurrentAppAddress, PCHELPER_MAX_MSG_SIZE_IN_BYTES);


					//check if app address modified
					if(g_ui32CurrentAppAddress != g_ui32PreviousAppAddress )
					{
						// Construct PC Msg
						APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_ACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

						//send ACk
						HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

						// loop till PC Tx Channel Idle is Yes
						while(g_ui8PCTxChannelIdle == NO);

						// Set PC Rx Channel Idle to No
						g_ui8PCTxChannelIdle = NO;
					}
					else
					{
						// Construct PC Msg
						APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_NACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

						//send NACk
						HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

						// loop till PC Tx Channel Idle is Yes
						while(g_ui8PCTxChannelIdle == NO);

						// Set PC Rx Channel Idle to No
						g_ui8PCTxChannelIdle = NO;
					}
					break;

				case PCHELPER_REQUEST_UPDATE_BOOTLOADER:
					//READ BOOTLOADER Code in array saved in SRAM

					HAL_Flash_Read_Data(BOOTLOADER_START_ADDRESS,(uint32_t *) g_aui8ReadBLData,BOOTLOADER_MAX_MSG_SIZE);

					void (*app_reset_handler) (void)= (void*) (*(volatile uint32_t *)g_aui8ReadBLData +4);

						__set_MSP((*(volatile uint32_t *)g_aui8ReadBLData));

						app_reset_handler();

					if(HAL_Flash_Erase_EntireorPage_Flash(ADDR_FLASH_PAGE_0,BOOTLOADER_NUMBER_OF_PAGES)==HAL_OK)
					{

					}
					break;

				default :
					break;
				}
			}
		}
		else
		{
			// Construct PC Msg
			APP_PCHelper_ConstructMsg (g_aui8PCTxBuff,&g_pui8MsgSize,PCHELPER_BOOTLOADER_NACK_MSG_TYPE,g_ui32BTVersion,g_aui8ReadData);

			//send NACk
			HAL_PC_write (g_aui8PCTxBuff, g_pui8MsgSize);

			// loop till PC Tx Channel Idle is Yes
			while(g_ui8PCTxChannelIdle == NO);

			// Set PC Rx Channel Idle to No
			g_ui8PCTxChannelIdle = NO;
		}


	default:
		break;
	} //end of button state

	return ui32Status;
}
