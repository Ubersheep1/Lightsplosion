# Lightsplosion
An Arduino project which controls WS2811 LEDs. Can take control data from ethernet, serial port or SD card.

To compile and run this project you will need:
* The latest version of the Arduino development enviroment (it will probably build on older versions, but I test on the latest release).
* The [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) library installed
* At least a basic knowledge of how arduino [works](https://www.arduino.cc/en/Reference/HomePage)
* An Arduino compaitble platform with 3K of RAM + 3 bytes per LED you connect (eg: 200 LEDs would require 3,600 bytes of RAM)

You configure this project from the "definitions.h" file, where you tell the system how many LEDs are connected to it, and on which pin. Then you simply build and upload it to your platform (I use an [Arduino Mega 2560](https://www.arduino.cc/en/Main/ArduinoBoardMega2560)), then connect it to your LEDs and your home network. The program will run through a demo sequence which showcases some of the macros. If you listen on the serial port at 9600 baud, it will tell you the IP address it got from your network. You can open a connection to it on socket 160 (I suggest using [Putty](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html) in raw mode to do this). Type "help" and press enter for a fully interactive help on the command line interface.
