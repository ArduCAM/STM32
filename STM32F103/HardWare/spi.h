#ifndef __SPI_H
#define __SPI_H
#include "main.h"
#define  BUFFER_MAX_SIZE 4096
extern uint32_t sendlen ; 
extern uint32_t haveRev ;
extern uint32_t noRev;
extern uint8_t *picbuf	;
extern bool receive_OK;	
extern bool send_OK ; 
extern uint8_t	Buf1[BUFFER_MAX_SIZE];
extern uint8_t	Buf2[BUFFER_MAX_SIZE];
extern uint8_t  EP2_SendFinish;

		
 #define  PB_SPI_RX_DMA_Channel    DMA1_Channel2
 #define  PB_SPI_DMA_IRQ		   DMA1_Channel2_IRQn
 #define  PB_SPI_DMA_IRQHandler	   DMA1_Channel2_IRQHandler

u8 DMA1_Init(void);
void SPI1_Init(void);			 
u8 SPI1_ReadWriteByte(u8 TxData);
void DMA1_RX(uint8_t *p , uint32_t len);
void DMA1_SendtoUsart(uint8_t *p , uint32_t len);
void SendbyUSART1( void);
void SingleCapTransfer(void);
void StartBMPcapture(void);
#endif
