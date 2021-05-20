#ifndef __USART_H
#define __USART_H

#include "main.h" 

extern unsigned char USART1_RecieveData;
extern unsigned char NewCMD;

void USART1_UART_Init(uint32_t bound);
void UART1_BulkOut(uint32_t len, uint8_t *p);

#endif
