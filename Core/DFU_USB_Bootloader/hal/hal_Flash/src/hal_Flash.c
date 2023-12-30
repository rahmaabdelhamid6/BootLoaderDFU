/*
 * hal_Flash.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Rahma
 */
#include "main.h"
#include "hal_Flash.h"


static uint32_t _GetPage(uint32_t Addr)
{
	uint32_t page = 0;

	if (Addr <= (FLASH_BASE + FLASH_BANK_SIZE))
	{
		/* Bank 1 */
		page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
	}
	else
	{
		/* Bank 2 */
//		page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
		page = (Addr - FLASH_BASE ) / FLASH_PAGE_SIZE;
	}

	return page;
}

static uint32_t _GetBank(uint32_t Addr)
{
	uint32_t bank = 0;

	if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
	{
		/* No Bank swap */
		if (Addr <= (FLASH_BASE + FLASH_BANK_SIZE))
		{
			bank = FLASH_BANK_1;
		}
		else
		{
			bank = FLASH_BANK_2;
		}
	}
	else
	{
		/* Bank swap */
		if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
		{
			bank = FLASH_BANK_2;
		}
		else
		{
			bank = FLASH_BANK_1;
		}
	}

	return bank;
}
uint32_t HAL_Flash_Write_Data (uint32_t ui32StartPageAddress, uint32_t *paui32Data, uint16_t ui16numberofwords)
{
	uint32_t	ui32Status = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t ui32StartPage=0;
	uint32_t ui32EndPageAdress=0;
	uint32_t ui32EndPage=0;
	uint32_t ui32BankNumber=0;
	uint32_t ui32PAGEError=0;
	uint32_t ui32sofar=0;

	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Erase the user Flash area*/

	ui32StartPage = _GetPage(ui32StartPageAddress);
	ui32EndPageAdress =(ui32StartPageAddress +(ui16numberofwords*4));
	ui32EndPage = _GetPage(ui32EndPageAdress);
	/* Get the bank */
	ui32BankNumber = _GetBank(ui32StartPageAddress);

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = ui32BankNumber;
	EraseInitStruct.Page = ui32StartPage;
	EraseInitStruct.NbPages = ((ui32EndPage - ui32StartPage)/FLASH_PAGE_SIZE) +1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &ui32PAGEError) != HAL_OK)
	{
		/*Error occurred while page erase.*/
		return HAL_FLASH_GetError ();
	}

	/* Program the user Flash area word by word*/

	while (ui32sofar<ui16numberofwords)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ui32StartPageAddress, (uint64_t)paui32Data[ui32sofar]) == HAL_OK)
		{
			ui32StartPageAddress += 8;  // use StartPageAddress += 2 for half word and 8 for double word
			ui32sofar++;
		}
		else
		{
			/* Error occurred while writing data in Flash memory*/
			return HAL_FLASH_GetError ();
		}
	}

	/* Lock the Flash to disable the flash control register access (recommended
	      to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();

	return ui32Status;
}

uint32_t HAL_Flash_Erase_EntireorPage_Flash(uint16_t ui16PageNo,uint16_t ui16NoOfPages )
{
	uint32_t	ui32Status = 0;
	static FLASH_EraseInitTypeDef EraseStruct;

	uint32_t ui32StartAdd=0;
	uint32_t ui32BankNumber=0;
	uint32_t ui32PAGEError=0;
	uint32_t ui32StartPage=0;

	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Erase the user Flash area*/
	/* Get the bank */
	ui32StartAdd = ((ui16PageNo*FLASH_PAGE_SIZE)+FLASH_BASE);
	ui32StartPage = _GetPage(ui32StartAdd);

	ui32BankNumber = _GetBank(ui32StartAdd);

	/* Fill EraseInit structure*/
	EraseStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseStruct.Banks = ui32BankNumber;
	EraseStruct.Page = ui32StartPage;
	EraseStruct.NbPages = ui16NoOfPages;

//	ui32Status=HAL_FLASHEx_Erase(&EraseStruct, &ui32PAGEError);

	if (HAL_FLASHEx_Erase(&EraseStruct, &ui32PAGEError) != HAL_OK)
	{
		/*Error occurred while page erase.*/
		return HAL_FLASH_GetError ();
	}
	/* Lock the Flash to disable the flash control register access (recommended
		      to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();

	return ui32Status;
}
uint32_t HAL_Flash_Read_Data (uint32_t ui32StartPageAddress, uint32_t *paui32RxBuf, uint16_t ui16numberofwords)
{
	uint32_t	ui32Status = 0;
	while (1)
	{

		*paui32RxBuf = *(__IO uint32_t *)ui32StartPageAddress;
		ui32StartPageAddress += 8;
		paui32RxBuf++;
		if (!(ui16numberofwords--)) break;
	}
	return ui32Status;
}



