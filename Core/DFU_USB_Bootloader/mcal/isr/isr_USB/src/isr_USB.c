/*
 * isr_USB.c
 *
 *  Created on: Dec 22, 2023
 *      Author: Rahma
 */


#include "main.h"
#include "isr_USB.h"



// Pointer To callback functions
// USB Tx
void (*g_pfnUSBRxCpltCallback) ( uint32_t) = NULL;

// USB Rx
void (*g_pfnUSBTxCpltCallback) (void) = NULL;
