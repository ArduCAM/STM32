# STM32
## ArduCAM_STM32_V0.1 (C)2018 Arducam 
## Web: http://www.ArduCAM.com

This program is a demo of how to use ArduCAM Mini camera on STM32 f103C8T6/ f103zet6 .etc platform.
This demo was made for ArduCAM_Mini_2MP. ArduCAM_Mini_5MP_Plus.It needs to be used in combination with PC software.
It can take photo continuously as video streaming.


 
The demo sketch will do the following tasks:
1. Set the camera to JPEG output mode.
2. Read data from Serial port and deal with it
3. If receive 0x00-0x08,the resolution will be changed.
4. If receive 0x10,camera will capture a JPEG photo and buffer the image to FIFO.Then write datas to Serial port.
5. If receive 0x20,camera will capture JPEG photo and write datas continuously.Stop when receive 0x21.
6. If receive 0x30,camera will capture a BMP  photo and buffer the image to FIFO.Then write datas to Serial port.
7. If receive 0x11 ,set camera to JPEG output mode.
8. If receive 0x31 ,set camera to BMP  output mode.

