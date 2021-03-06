# STM32

## Brief Introduction
- This program is a demo of how to use ArduCAM Mini camera on STM32 f103C8T6/ f103ZET6 .etc platform.
- This demo was made for ArduCAM_Mini_2MP. ArduCAM_Mini_5MP_Plus. It needs to be used in combination with PC software.
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

## Hardware connection
             Arducam   ->        STM32 
 
             SCL       ->        PB10
             
             SDA       ->        PB11
             
             VCC       ->        5V
             
             GND       ->        GND
             
             SCK       ->        PB13 
             
             MISO      ->        PB14
             
             MOSI      ->        PB15
             
             CS        ->        PB1
 ![Alt text](https://github.com/UCTRONICS/pic/blob/master/stm32/hardware2.jpg)

## Operation Demo
### Part1: Download the Project from https://github.com/ArduCAM/STM32.git
![EasyBehavior](https://github.com/UCTRONICS/pic/blob/master/stm32/stm32_lib_download.gif)
### Part2: Upload the firmware to the STM32 platform
![EasyBehavior](https://github.com/UCTRONICS/pic/blob/master/stm32/stm32_firmware.gif)
### Part3: Signal capture demo:
![EasyBehavior](https://github.com/UCTRONICS/pic/blob/master/stm32/stm32_signal_capture.gif)
### Part4: Video streaming demo:
![EasyBehavior](https://github.com/UCTRONICS/pic/blob/master/stm32/stm32_videoStreaming.gif)






