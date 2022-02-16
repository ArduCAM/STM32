# STM32

## Brief Introduction
- This program is a demo of how to use ArduCAM Mini camera on STM32 f103C8T6/ f103ZET6 / L152RBT6.etc platform.
- This demo was made for ArduCAM_Mini_2MP. It needs to be used in combination with PC software.
- It support signal capture 
- It can take photo continuously as video streaming.

## Mainly tasks
- Set the camera to JPEG output mode.
- Read data from Serial port and deal with it
- If receive 0x00-0x08,the resolution will be changed.
- If receive 0x10,camera will capture a JPEG photo and buffer the image to FIFO.Then write datas to Serial port.
- If receive 0x20,camera will capture JPEG photo and write datas continuously.Stop when receive 0x21.
- If receive 0x30,camera will capture a BMP  photo and buffer the image to FIFO.Then write datas to Serial port.
- If receive 0x11 ,set camera to JPEG output mode.
- If receive 0x31 ,set camera to BMP  output mode.

## Hardware connection (STM32 f103C8T6/ f103ZET6)
             Arducam   ->        STM32 
 
             SCL       ->        PB10
             
             SDA       ->        PB11
             
             VCC       ->        3.3V
             
             GND       ->        GND
             
             SCK       ->        PA5 
             
             MISO      ->        PA6
             
             MOSI      ->        PA7
             
             CS        ->        PA4
			 
## Hardware connection (STM32L152RBT6)
             Arducam   ->        STM32 
 
             SCL       ->        PB10
             
             SDA       ->        PB11
             
             VCC       ->        3.3V
             
             GND       ->        GND
             
             SCK       ->        PB13 
             
             MISO      ->        PB14
             
             MOSI      ->        PB15
             
             CS        ->        PB12
 ![Alt text](https://github.com/UCTRONICS/pic/blob/master/stm32/hardware2.jpg)

## Operation Demo
### Part1: Download the Project from https://github.com/ArduCAM/STM32.git
![EasyBehavior](https://github.com/UCTRONICS/pic/blob/master/stm32/Downloaddriver.gif)
### Part2: Upload the firmware to the STM32 platform
![EasyBehavior](https://github.com/UCTRONICS/pic/blob/master/stm32/Compiledriver.gif)
### Part3: Signal capture demo:
![EasyBehavior](https://github.com/UCTRONICS/pic/blob/master/stm32/stm32_signal_capture.gif)
### Part4: Video streaming demo:
![EasyBehavior](https://github.com/UCTRONICS/pic/blob/master/stm32/stm32_videoStreaming.gif)






