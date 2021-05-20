#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "main.h" 

	  	
extern uint8_t USART1_RecieveData;
extern uint8_t NewCMD;
		
void UART1_BulkOut(uint32_t, uint8_t*);
void uart_init(uint32_t bound);

#endif


