#include "spi.h"

#define BMPIMAGEOFFSET 66
#define pgm_read_byte(x)        (*((char *)x))
const char bmp_header[BMPIMAGEOFFSET] =
{
  0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00,
  0x00, 0x00
};

uint8_t	*picbuf = 0;
bool receive_OK = false;    
bool USB_sendOK = true;    
bool send_OK = true;
uint32_t sendlen = 0;
uint32_t haveRev = 0;
uint32_t noRev = 0;
uint8_t  EP2_SendFinish = 1;
uint8_t	Buf1[BUFFER_MAX_SIZE]={0}, Buf2[BUFFER_MAX_SIZE]={0};
extern uint16_t NumPackage;

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
	
	GPIO_InitStructure.GPIO_Pin = CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;    ;
	GPIO_Init( CS_PORT, &GPIO_InitStructure );
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
//	SPI2_ReadWriteByte(0xff);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(PB_SPI_RX_DMA_Channel, &DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = PB_SPI_DMA_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	
	
	
	DMA_ITConfig(PB_SPI_RX_DMA_Channel, DMA_IT_TC, ENABLE);

}   


////Control the CS pin
void CS_HIGH(void)
{
 	GPIO_SetBits(CS_PORT,CS_PIN);					
}

void CS_LOW(void)
{
 	GPIO_ResetBits(CS_PORT,CS_PIN);					    
}

 
void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;
	SPI_Cmd(SPI2,ENABLE); 
} 

uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData);
	retry=0;

	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2);				    
}

void DMA1_RX(uint8_t *p , uint32_t len)
{		
	CS_LOW();
	set_fifo_burst();
	PB_SPI_RX_DMA_Channel->CMAR = (uint32_t)p;
	PB_SPI_RX_DMA_Channel->CNDTR = len;
	DMA1_Channel5->CMAR = (uint32_t)p;
	DMA1_Channel5->CNDTR = len;
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);		
	DMA_Cmd(PB_SPI_RX_DMA_Channel, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);	
}

void DMA1_SendtoUsart(uint8_t *p , uint32_t len)
{		
	DMA1_Channel7->CMAR = (uint32_t)p;
	DMA1_Channel7->CNDTR = len;
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);		
	DMA_Cmd(DMA1_Channel7, ENABLE);
}

void SendbyUSART1(void)
{	
  uint8_t	*sdbuf;
	haveRev += sendlen;
  if(haveRev < length)
	{	
		if(picbuf == Buf1)
		{		
			sdbuf = Buf1;	  picbuf = Buf2;	
		}
		else
		{
			sdbuf = Buf2;	  picbuf = Buf1;
		}
		UART1_BulkOut(sendlen,sdbuf);
		noRev	= length - haveRev;		
		sendlen	= (noRev>=BUFFER_MAX_SIZE) ? BUFFER_MAX_SIZE : noRev;	
		DMA1_RX(picbuf, sendlen);	
	}
	else
	{
		UART1_BulkOut(sendlen, picbuf);
		send_OK = 1;
	}			 	 					 	 	
}


void SingleCapTransfer(void)
{
	flush_fifo();
	clear_fifo_flag();
	start_capture(); 
	while(!get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){;}
	length= read_fifo_length();
	sendlen = (length>=BUFFER_MAX_SIZE) ? BUFFER_MAX_SIZE : length;
	picbuf = Buf1;
	haveRev = 0;
	DMA1_RX(picbuf, sendlen);
}

void StartBMPcapture(void)
{
	flush_fifo();
	clear_fifo_flag();
	start_capture(); 
	while(!get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){;}
	//printf("ACK CMD capture done\r\n");
	length= read_fifo_length();
//	printf("ACK CMD the length is %d\r\n",length);		
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, 0xff);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, 0xaa);
	for(int temp = 0; temp < BMPIMAGEOFFSET; temp++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, pgm_read_byte(&bmp_header[temp]));

	}
	CS_LOW();
	set_fifo_burst();
	char VH, VL;
	int i = 0, j = 0;
	for (i = 0; i < 240; i++)
	{
		for (j = 0; j < 320; j++)
		{
			VH = SPI2_ReadWriteByte(0x00);			
			VL = SPI2_ReadWriteByte(0x00);		
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, VL);
			delay_us(15);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, VH);
			delay_us(15);
		}
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, 0xbb);
	delay_us(12);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, 0xcc);
	CS_HIGH();
}

void PB_SPI_DMA_IRQHandler(void)
{ 	
	if(DMA_GetITStatus(DMA1_IT_TC4))
	{
		DMA_ClearITPendingBit(DMA1_IT_GL4 | DMA1_IT_TC4 | DMA1_IT_GL5 | DMA1_IT_TC5);
		DMA_Cmd(PB_SPI_RX_DMA_Channel, DISABLE);
		DMA_Cmd(DMA1_Channel5, DISABLE);
		SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, DISABLE);  
		CS_HIGH();
		receive_OK =1;
	}
}

void DMA1_Channel7_IRQHandler(void)
{ 	
	if(DMA_GetITStatus(DMA1_IT_TC7))
	{
		DMA_ClearITPendingBit(DMA1_IT_GL7 | DMA1_IT_TC7);
		DMA_Cmd(DMA1_Channel7, DISABLE);
		USART_DMACmd(USART2, USART_DMAReq_Tx , DISABLE);
	}
}

