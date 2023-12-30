/*
 * app_USBBootLoader.h
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */

#ifndef DFU_USB_BOOTLOADER_APP_APP_USBBOOTLOADER_INC_APP_USBBOOTLOADER_H_
#define DFU_USB_BOOTLOADER_APP_APP_USBBOOTLOADER_INC_APP_USBBOOTLOADER_H_


#define BOOTLOADER_VERSION             1U

#define APP_START_ADDRESS      ADDR_FLASH_PAGE_12

//#define SAVED_ADD_PAGE_NO      16U
//
//#define NO_OF_PAGES_ERASED     1U

#define BOOTLOADER_START_ADDRESS      ADDR_FLASH_PAGE_0
#define BOOTLOADER_MAX_MSG_SIZE        0x3000
#define  BOOTLOADER_NUMBER_OF_PAGES    12U


typedef enum{

	USER_APP_MODE,
	BOOTLOADER_MOOD

}BootLoaderMood_et;


void APP_GoToApp(void);
void APP_ModifyAppAddress();
uint32_t APP_USBBOOTLOADER_Init (void);


#endif /* DFU_USB_BOOTLOADER_APP_APP_USBBOOTLOADER_INC_APP_USBBOOTLOADER_H_ */
