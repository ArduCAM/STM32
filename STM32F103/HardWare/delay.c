#include "delay.h"

static uint8_t  fac_us=0;
static uint16_t fac_ms=0;

void delay_init()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SystemCoreClock/8000000;
	fac_ms = (uint16_t)fac_us*1000;
}								    
		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD = nus*fac_us;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
}

void delay_ms(u16 nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fac_ms;
	SysTick->VAL =0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
} 
