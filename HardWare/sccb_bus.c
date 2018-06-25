/**
  * @file    sccb_bus.c 
  * @author  Arducam
  * @version V0.1
  * @date    2018.06.22
  * @brief   sccb bus
  */
#include "sccb_bus.h"
#include "delay.h"
 uint32_t ntime;

/**
  * @brief  初始化总线
  * @param  None
  * @retval None
  */
void sccb_bus_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_11);						 // 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //输输出
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_10);						 // 输出高
	SCCB_DATA_OUT;

}




void sccb_bus_start(void)
{
    SCCB_SID_H();             //数据线高电平
    delay_us(I2C_TIM);
    SCCB_SIC_H();	           //在时钟线高的时候数据线由高至低
    delay_us(I2C_TIM);
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_L();	           //数据线恢复低电平，单操作函数必要
    delay_us(I2C_TIM);
}


void sccb_bus_stop(void)
{
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_H();	
    delay_us(I2C_TIM);  
    SCCB_SID_H();	
    delay_us(I2C_TIM);  
}


void sccb_bus_send_noack(void)
{	
	SCCB_SID_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SID_L();	
	delay_us(I2C_TIM);
}

void sccb_bus_send_ack(void)
{	
	SCCB_SID_L();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SIC_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SID_L();	
	delay_us(I2C_TIM);
}

uint8_t sccb_bus_write_byte(uint8_t data)
{
	uint32_t i;
	uint8_t tem;

	for(i = 0; i < 8; i++) //循环8次发送数据
	{
		if((data<<i) & 0x80)
		{
			SCCB_SID_H();
		}
		else 
		{
			SCCB_SID_L();
		}
		delay_us(I2C_TIM);
		SCCB_SIC_H();	
		delay_us(I2C_TIM);
		SCCB_SIC_L();	
		//delay_us(I2C_TIM);
	}
	//delay_us(10);
	SCCB_DATA_IN;
	delay_us(I2C_TIM);
	SCCB_SIC_H();	
	delay_us(I2C_TIM);
	if(SCCB_SID_STATE)
	{
		tem = 0;                //SDA=1发送失败
	}
	else 
	{
		tem = 1;                //SDA=0发送成功，返回1
	}

	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_DATA_OUT;
	return tem;  
}

uint8_t sccb_bus_read_byte(void)
{	
	uint32_t i;
	uint8_t read = 0;
	
	SCCB_DATA_IN;
	delay_us(I2C_TIM);
	for(i = 8; i > 0; i--) //循环8次接收数据
	{		     
		delay_us(I2C_TIM);
		SCCB_SIC_H();
		delay_us(I2C_TIM);
		read = read << 1;
		if(SCCB_SID_STATE)
		{
			read += 1; 
		}
		SCCB_SIC_L();
		delay_us(I2C_TIM);
	}	
    SCCB_DATA_OUT;
	return read;
}

