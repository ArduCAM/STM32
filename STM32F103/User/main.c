/**
  * @file    main.c
  * @author  Arducam 
  * @version V0.1
  * @date    2018.06.22
  * @brief   Minly work
  */

#include "main.h"

int main(void)
{
	uint8_t vid, pid, temp ;
	uint8_t Camera_WorkMode = 0;
	uint8_t start_shoot = 0;
	uint8_t stop = 0;
	NVIC_Configuration();
	SystemInit();
	delay_init();
	USART1_UART_Init(921600);
	ArduCAM_LED_init();
	ArduCAM_CS_init();
	sccb_bus_init();
	SPI1_Init();
	
	while(1)
	{
		write_reg(ARDUCHIP_TEST1, 0x55);
		temp = read_reg(ARDUCHIP_TEST1);
		if (temp != 0x55)
		{
			printf("ACK CMD SPI interface Error!\n");
			delay_ms(1000);
			continue;
		}
		else
		{
			printf("ACK CMD SPI interface OK!\r\n");
			break;
		}
	}
	
  while(1)
	{
		sensor_addr = 0x60;
		wrSensorReg8_8(0xff, 0x01);
		rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
		rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
		if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
			printf("ACK CMD Can't find OV2640 module!\r\n");
		else
		{
		  sensor_model =  OV2640 ;
		  printf("ACK CMD OV2640 detected.\r\n");   
		  break;
		}
		sensor_addr = 0x78;
		rdSensorReg16_8(OV5640_CHIPID_HIGH, &vid);
		rdSensorReg16_8(OV5640_CHIPID_LOW, &pid);
		if ((vid != 0x56) || (pid != 0x40))
			printf("ACK CMD Can't find OV5640 module!\r\n");
		else
		{
			sensor_model =  OV5640 ;
			printf("ACK CMD OV5640 detected.\r\n");
		  break;
		}
		sensor_addr = 0x78;
		rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
		rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
		if ((vid != 0x56) || (pid != 0x42))
		{
			printf("ACK CMD Can't find OV5642 module!\r\n");
			continue;
		}
		else
		{
		 sensor_model =  OV5642 ;
		 printf("ACK CMD OV5642 detected.\r\n"); 
		 break;		 
		}
	}  
//Support OV2640/OV5640/OV5642 Init
  ArduCAM_Init(sensor_model);

	while(1)
	{
		if(NewCMD == 1)
		{
			NewCMD = 0;
			switch(USART1_RecieveData)
			{
				case 0:
					if (sensor_model == OV2640)
					{
						OV2640_set_JPEG_size(OV2640_160x120);
						printf("ACK CMD switch to OV2640_160x120\r\n");
					}
					else if(sensor_model == OV5640)
					{
						OV5640_set_JPEG_size(OV5640_320x240);
						printf("ACK CMD switch to OV5640_320x240\r\n");
					}
					else if(sensor_model == OV5642)
					{
						OV5642_set_JPEG_size(OV5642_320x240);
						printf("ACK CMD switch to OV5642_320x240\r\n");
					}
					break;
				case 1:
					if(sensor_model== OV2640)
					{
						OV2640_set_JPEG_size(OV2640_176x144);
						printf("ACK CMD switch to OV2640_176x144\r\n");
					}
					else if(sensor_model== OV5640)
					{ 
						OV5640_set_JPEG_size(OV5640_352x288);
						printf("ACK CMD switch to OV5640_352x288\r\n");
					}
					else if(sensor_model== OV5642)
					{ 
						OV5642_set_JPEG_size(OV5642_640x480);
						printf("ACK CMD switch to OV5642_640x480\r\n");
					}
					break;
				case 2:
					if(sensor_model== OV2640)
					{
						OV2640_set_JPEG_size(OV2640_320x240);
						printf("ACK CMD switch to OV2640_320x240\r\n");
					}
					else if(sensor_model== OV5640)
					{ 
						OV5640_set_JPEG_size(OV5640_640x480);
					printf("ACK CMD switch to OV5640_640x480\r\n");
					}
					else if(sensor_model== OV5642)
					{
						OV5642_set_JPEG_size(OV5642_1024x768);
						printf("ACK CMD switch to OV5642_1024x768\r\n");
					}
					break;
				case 3:
					if(sensor_model== OV2640)
					{
						OV2640_set_JPEG_size(OV2640_352x288);
						printf("ACK CMD switch to OV2640_352x288\r\n");
					}
					else if(sensor_model== OV5640)
					{ 
						OV5640_set_JPEG_size(OV5640_800x480);
						printf("ACK CMD switch to OV5640_800x480\r\n");
					}
					else if(sensor_model== OV5642)
					{
						OV5642_set_JPEG_size(OV5642_1280x960);
						printf("ACK CMD switch to OV5642_1280x960\r\n");
					}
					break;
				case 4:
					if(sensor_model== OV2640)
					{
						OV2640_set_JPEG_size(OV2640_640x480);
						printf("ACK CMD switch to OV2640_640x480\r\n");
					}
					else if(sensor_model== OV5640)
					{
						OV5640_set_JPEG_size(OV5640_1024x768);
						printf("ACK CMD switch to OV5640_1024x768\r\n");
					}
					else if(sensor_model== OV5642)
					{
						OV5642_set_JPEG_size(OV5642_1600x1200);	
						printf("ACK CMD switch to OV5642_1600x1200\r\n");
					}
					break;
				case 5:
					if(sensor_model== OV2640)
					{
						OV2640_set_JPEG_size(OV2640_800x600);
						printf("ACK CMD switch to OV2640_800x600\r\n");
					}
					else if(sensor_model== OV5640) 
					{
						OV5640_set_JPEG_size(OV5640_1280x960);
						printf("ACK CMD switch to OV5640_1280x960\r\n");
					}
					else if(sensor_model== OV5642)
					{
						OV5642_set_JPEG_size(OV5642_2048x1536);
						printf("ACK CMD switch to OV5642_2048x1536\r\n");
					}
					break;
				case 6:
					if(sensor_model== OV2640)
					{
						OV2640_set_JPEG_size(OV2640_1024x768);
						printf("ACK CMD switch to OV2640_1024x768\r\n");
					}
					else if(sensor_model== OV5640)
					{
						OV5640_set_JPEG_size(OV5640_1600x1200);
						printf("ACK CMD switch to OV5640_1600x1200\r\n");
					}
					else if(sensor_model== OV5642)
					{
						OV5642_set_JPEG_size(OV5642_2592x1944);	
						printf("ACK CMD switch to OV5642_2592x1944\r\n");
					}
					break;
				case 7:
					if(sensor_model== OV2640)
					{
						OV2640_set_JPEG_size(OV2640_1280x1024);
						printf("ACK CMD switch to OV2640_1280x1024\r\n");
					}
					else if(sensor_model== OV5640) 
					{
						OV5640_set_JPEG_size(OV5640_2048x1536);
						printf("ACK CMD switch to OV5640_2048x1536\r\n");
					}
					break;
				case 8:
					if(sensor_model== OV2640)
					{
						OV2640_set_JPEG_size(OV2640_1600x1200);
						printf("ACK CMD switch to OV2640_1600x1200\r\n");
					}
					else if(sensor_model== OV5640) 
					{
						OV5640_set_JPEG_size(OV5640_2592x1944);
						printf("ACK CMD switch to OV5640_2592x1944\r\n");
					}
					break;
				case 0x10:
					Camera_WorkMode = 1;
					start_shoot = 1;
					printf("ACK CMD CAM start single shoot.\r\n");
					break;
				case 0x11:
					set_format(JPEG);
					ArduCAM_Init(sensor_model);
					#if !defined(OV2640)
						set_bit(ARDUCHIP_TIM,VSYNC_MASK);
					#endif
					break;
				case 0x20:
					Camera_WorkMode = 2;
					start_shoot = 2;
					printf("ACK CMD CAM start video streaming.\r\n");
					break;
				case 0x21:
					stop = 1;
					printf("ACK CMD CAM stop video streaming.\r\n");
					break;
				case 0x30:
					Camera_WorkMode = 3;
					start_shoot = 3;
					printf("ACK CMD CAM start single shoot.\r\n");
					break;
				case 0x31:
					set_format(BMP);
					ArduCAM_Init(sensor_model);
					if(sensor_model != OV2640)
					{
						clear_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
					}
					wrSensorReg16_8(0x3818,0x81);
					wrSensorReg16_8(0x3621,0xa7);		
					printf("ACK CMD SetToBMP \r\n");	
					break;
				default:
					break;
			}
		}
		if(Camera_WorkMode == 1)
		{
			if(start_shoot == 1)
			{
				start_shoot = 0;
				SingleCapTransfer();
			}
			if(receive_OK) 
			{
				receive_OK= 0;
				SendbyUSART1(); 
			}
		}	
		else if(Camera_WorkMode == 2)
		{
			if(start_shoot == 2)
			{
				if(send_OK)
				{
					if(stop)
					{
						printf("ACK CMD CAM stop video streaming.\r\n");
						stop = 0;
						Camera_WorkMode = 0;
						start_shoot = 0;
					}
					send_OK=false;    
					SingleCapTransfer();		
				}	
				if(receive_OK) 
				{
					receive_OK= 0;
					SendbyUSART1();   
				}
			}
		}
		else if(Camera_WorkMode == 3)
		{	
			if(start_shoot == 3)
			{
				start_shoot = 0;
				Camera_WorkMode = 0;
				StartBMPcapture();
			}
		}
	}
}
