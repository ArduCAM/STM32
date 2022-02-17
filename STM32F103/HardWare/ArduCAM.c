/**
  * @file    ArduCAM.c
  * @author  Arducam 
  * @version V0.1
  * @date    2018.06.18
  * @brief   Arducam mainly driver
  */

#include "arducam_conf.h"
#include "ArduCAM.h"
#include "spi.h"
#include "sccb_bus.h"
#include "usart.h"
#include "ov2640_regs.h"
#include "ov5642_regs.h"
#include "ov5640_regs.h"

byte sensor_model = 0;
byte sensor_addr = 0;
byte m_fmt = JPEG;
uint32_t length = 0;
uint8_t is_header= false ;

void ArduCAM_Init(byte model) 
{
	switch (model)
  {
    case OV2640:
    case OV9650:
    case OV9655:
		{		
		  wrSensorReg8_8(0xff, 0x01);
      wrSensorReg8_8(0x12, 0x80);
      if(m_fmt == JPEG)
      {
				wrSensorRegs8_8(OV2640_JPEG_INIT);
				wrSensorRegs8_8(OV2640_YUV422);
				wrSensorRegs8_8(OV2640_JPEG);
				wrSensorReg8_8(0xff, 0x01);
				wrSensorReg8_8(0x15, 0x00);
				wrSensorRegs8_8(OV2640_320x240_JPEG);
      }
      else
      {
        wrSensorRegs8_8(OV2640_QVGA);
      }
      break;
		}
		case OV5640:
		{
			if (m_fmt == JPEG)
			{
				wrSensorReg16_8(0x3103, 0x11);
				wrSensorReg16_8(0x3008, 0x82);
				wrSensorRegs16_8(OV5640YUV_Sensor_Dvp_Init);	
				wrSensorRegs16_8(OV5640_JPEG_QSXGA);
				wrSensorRegs16_8(OV5640_QSXGA2QVGA);
			  wrSensorReg16_8(0x4407, 0x04);

			}
			else
			{
				wrSensorReg16_8(0x3103, 0x11);
				wrSensorReg16_8(0x3008, 0x82);
				wrSensorRegs16_8(OV5640YUV_Sensor_Dvp_Init);
				wrSensorRegs16_8(OV5640_RGB_QVGA);
			} 
			write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
			 break;
		}
		case OV5642:
      {
        wrSensorReg16_8(0x3008, 0x80);
        wrSensorRegs16_8(OV5642_QVGA_Preview);
      
        if (m_fmt == JPEG)
        {
          wrSensorRegs16_8(OV5642_JPEG_Capture_QSXGA);
          wrSensorRegs16_8(ov5642_320x240);
          wrSensorReg16_8(0x3818, 0xa8);
          wrSensorReg16_8(0x3621, 0x10);
          wrSensorReg16_8(0x3801, 0xb0);
          wrSensorReg16_8(0x4407, 0x04);
        }
        else
        {
        	byte reg_val;
          wrSensorReg16_8(0x4740, 0x21);
          wrSensorReg16_8(0x501e, 0x2a);
          wrSensorReg16_8(0x5002, 0xf8);
          wrSensorReg16_8(0x501f, 0x01);
          wrSensorReg16_8(0x4300, 0x61);
          rdSensorReg16_8(0x3818, &reg_val);
          wrSensorReg16_8(0x3818, (reg_val | 0x60) & 0xff);
          rdSensorReg16_8(0x3621, &reg_val);
          wrSensorReg16_8(0x3621, reg_val & 0xdf);
        }
		write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH					
				
			  break;
      }
     default:
     break;
  }
}
//CS init
void ArduCAM_CS_init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Pin =  CS_PIN;		
GPIO_Init(CS_PORT, &GPIO_InitStructure);
CS_HIGH();	
}

//Ö¸Ê¾µÆ³õÊ¼»¯
void ArduCAM_LED_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  LED_PIN;		
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
//***********************************************
	//   debug pin 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;		
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_2);
//************************************************/
}

//Control the CS pin
void CS_HIGH(void)
{
 	GPIO_SetBits(CS_PORT,CS_PIN);					
}

void CS_LOW(void)
{
 	GPIO_ResetBits(CS_PORT,CS_PIN);					    
}

void set_format(byte fmt)
{
  if (fmt == BMP)
    m_fmt = BMP;
  else
    m_fmt = JPEG;
}

uint8_t bus_read(int address)
{
	uint8_t value;
   CS_LOW();
	 SPI1_ReadWriteByte(address);
	 value = SPI1_ReadWriteByte(0x00);
	 CS_HIGH();
	 return value;
}

uint8_t bus_write(int address,int value)
{	
	CS_LOW();delay_us(10);
	SPI1_ReadWriteByte(address);
	SPI1_ReadWriteByte(value);
	delay_us(10);
	CS_HIGH();
	return 1;
}

uint8_t read_reg(uint8_t addr)
{
	uint8_t data;
	data = bus_read(addr & 0x7F);
	return data;
}
void write_reg(uint8_t addr, uint8_t data)
{
	 bus_write(addr | 0x80, data); 
}

uint8_t read_fifo(void)
{
	uint8_t data;
	data = bus_read(SINGLE_FIFO_READ);
	return data;
}
void set_fifo_burst()
{
	SPI1_ReadWriteByte(BURST_FIFO_READ);
}


void flush_fifo(void)
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

void start_capture(void)
{
	write_reg(ARDUCHIP_FIFO, FIFO_START_MASK);
}

void clear_fifo_flag(void )
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

uint32_t read_fifo_length(void)
{
	uint32_t len1,len2,len3,len=0;
	len1 = read_reg(FIFO_SIZE1);
	len2 = read_reg(FIFO_SIZE2);
	len3 = read_reg(FIFO_SIZE3) & 0x7f;
	len = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	return len;	
}

//Set corresponding bit  
void set_bit(uint8_t addr, uint8_t bit)
{
	uint8_t temp;
	temp = read_reg(addr);
	write_reg(addr, temp | bit);
}
//Clear corresponding bit 
void clear_bit(uint8_t addr, uint8_t bit)
{
	uint8_t temp;
	temp = read_reg(addr);
	write_reg(addr, temp & (~bit));
}

//Get corresponding bit status
uint8_t get_bit(uint8_t addr, uint8_t bit)
{
  uint8_t temp;
  temp = read_reg(addr);
  temp = temp & bit;
  return temp;
}

//Set ArduCAM working mode
//MCU2LCD_MODE: MCU writes the LCD screen GRAM
//CAM2LCD_MODE: Camera takes control of the LCD screen
//LCD2MCU_MODE: MCU read the LCD screen GRAM
void set_mode(uint8_t mode)
{
  switch (mode)
  {
    case MCU2LCD_MODE:
      write_reg(ARDUCHIP_MODE, MCU2LCD_MODE);
      break;
    case CAM2LCD_MODE:
      write_reg(ARDUCHIP_MODE, CAM2LCD_MODE);
      break;
    case LCD2MCU_MODE:
      write_reg(ARDUCHIP_MODE, LCD2MCU_MODE);
      break;
    default:
      write_reg(ARDUCHIP_MODE, MCU2LCD_MODE);
      break;
  }
}


void OV2640_set_JPEG_size(uint8_t size)
{
	switch(size)
	{
		case OV2640_160x120:
			wrSensorRegs8_8(OV2640_160x120_JPEG);
			break;
		case OV2640_176x144:
			wrSensorRegs8_8(OV2640_176x144_JPEG);
			break;
		case OV2640_320x240:
			wrSensorRegs8_8(OV2640_320x240_JPEG);
			break;
		case OV2640_352x288:
	  	wrSensorRegs8_8(OV2640_352x288_JPEG);
			break;
		case OV2640_640x480:
			wrSensorRegs8_8(OV2640_640x480_JPEG);
			break;
		case OV2640_800x600:
			wrSensorRegs8_8(OV2640_800x600_JPEG);
			break;
		case OV2640_1024x768:
			wrSensorRegs8_8(OV2640_1024x768_JPEG);
			break;
		case OV2640_1280x1024:
			wrSensorRegs8_8(OV2640_1280x1024_JPEG);
			break;
		case OV2640_1600x1200:
			wrSensorRegs8_8(OV2640_1600x1200_JPEG);
			break;
		default:
			wrSensorRegs8_8(OV2640_320x240_JPEG);
			break;
	}
}

void OV5640_set_JPEG_size(uint8_t size)
{
  switch (size)
  {
    case OV5640_320x240:
      wrSensorRegs16_8(OV5640_QSXGA2QVGA);
      break;
    case OV5640_352x288:
      wrSensorRegs16_8(OV5640_QSXGA2CIF);
      break;
    case OV5640_640x480:
      wrSensorRegs16_8(OV5640_QSXGA2VGA);
      break;
    case OV5640_800x480:
      wrSensorRegs16_8(OV5640_QSXGA2WVGA);
      break;
    case OV5640_1024x768:
      wrSensorRegs16_8(OV5640_QSXGA2XGA);
      break;
    case OV5640_1280x960:
      wrSensorRegs16_8(OV5640_QSXGA2SXGA);
      break;
    case OV5640_1600x1200:
      wrSensorRegs16_8(OV5640_QSXGA2UXGA);
      break;
    case OV5640_2048x1536:
      wrSensorRegs16_8(OV5640_QSXGA2QXGA);
      break;
    case OV5640_2592x1944:
      wrSensorRegs16_8(OV5640_JPEG_QSXGA);
      break;
    default:
      //320x240
      wrSensorRegs16_8(OV5640_QSXGA2QVGA);
      break;
  }
}

void OV5642_set_JPEG_size(uint8_t size)
{ 
  switch (size)
  {
    case OV5642_320x240:
      wrSensorRegs16_8(ov5642_320x240);
      break;
    case OV5642_640x480:
      wrSensorRegs16_8(ov5642_640x480);
      break;
    case OV5642_1024x768:
      wrSensorRegs16_8(ov5642_1024x768);
      break;
    case OV5642_1280x960:
      wrSensorRegs16_8(ov5642_1280x960);
      break;
    case OV5642_1600x1200:
      wrSensorRegs16_8(ov5642_1600x1200);
      break;
    case OV5642_2048x1536:
      wrSensorRegs16_8(ov5642_2048x1536);
      break;
    case OV5642_2592x1944:
      wrSensorRegs16_8(ov5642_2592x1944);
      break;
    default:
      wrSensorRegs16_8(ov5642_320x240);
      break;
  }
}


byte wrSensorReg8_8(int regID, int regDat)
{
	delay_us(5);
	sccb_bus_start();                          
	if(sccb_bus_write_byte(sensor_addr) == 0)         
	{
		sccb_bus_stop();                        
		return 1;
	}
	delay_us(5);
	if(sccb_bus_write_byte(regID) == 0)
	{
		sccb_bus_stop();                              
		return 2;                                       
	}
	delay_us(5);
	if(sccb_bus_write_byte(regDat)==0)                    
	{
		sccb_bus_stop();                                 
		return 3;
	}
	sccb_bus_stop();                                    
	return 0;
}


byte rdSensorReg8_8(uint8_t regID, uint8_t* regDat)
{
	delay_us(10);
	
	sccb_bus_start();
	if(sccb_bus_write_byte(sensor_addr) == 0)                 
	{
		sccb_bus_stop();                                
		//goto start;
		return 1;                                        
	}
	delay_us(10);
	if(sccb_bus_write_byte(regID)==0)//ID
	{
		sccb_bus_stop();                                  
		//goto start;
		return 2;                                       
	}
	sccb_bus_stop();                                   
	delay_us(10);	
	sccb_bus_start();
	if(sccb_bus_write_byte(sensor_addr|0x01)==0)                    
	{
		sccb_bus_stop();                                   
		//goto start;
		return 3;                                          
	}
	delay_us(10);
	*regDat = sccb_bus_read_byte();                    
	sccb_bus_send_noack();                                
	sccb_bus_stop();                                      
	return 0;                
}

//I2C Array Write 8bit address, 8bit data
int wrSensorRegs8_8(const struct sensor_reg reglist[])
{
  int err = 0;
  uint16_t reg_addr = 0;
  uint16_t reg_val = 0;
  const struct sensor_reg *next = reglist;
  while ((reg_addr != 0xff) | (reg_val != 0xff))
  {
    reg_addr = next->reg;
    reg_val = next->val;
    err = wrSensorReg8_8(reg_addr, reg_val);
 //   delay_us(400);
    next++;
  }

  return err;
}

byte wrSensorReg16_8(int regID, int regDat)
{
	sccb_bus_start();
	if(0==sccb_bus_write_byte(sensor_addr))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(5);
  if(0==sccb_bus_write_byte(regID>>8))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(5);
  if(0==sccb_bus_write_byte(regID))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(5);
  if(0==sccb_bus_write_byte(regDat))
	{
		sccb_bus_stop();
		return(0);
	}
  sccb_bus_stop();
	
  return(1);
}

int wrSensorRegs16_8(const struct sensor_reg reglist[])
{
  int err = 0;

  unsigned int reg_addr;
  unsigned char reg_val;
  const struct sensor_reg *next = reglist;

  while ((reg_addr != 0xffff) | (reg_val != 0xff))
  {
    reg_addr =next->reg;
    reg_val = next->val;
    err = wrSensorReg16_8(reg_addr, reg_val);
    delay_us(600);
    next++;
  }
  return err;
}


byte rdSensorReg16_8(uint16_t regID, uint8_t* regDat)
{
	sccb_bus_start();                  
	if(0==sccb_bus_write_byte(0x78))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(20);
	delay_us(20);
  if(0==sccb_bus_write_byte(regID>>8))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(20);
  if(0==sccb_bus_write_byte(regID))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(20);
	sccb_bus_stop();
	
	delay_us(20);
	
	
	sccb_bus_start();                 
	if(0==sccb_bus_write_byte(0x79))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(20);
  *regDat=sccb_bus_read_byte();
  sccb_bus_send_noack();
  sccb_bus_stop();
  return(1);
}



