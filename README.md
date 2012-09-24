## Overview ##

First of all, what is a project about? Doomsday Button actually is a wireless keyboard with only one button.
Pressing this button hits a special hotkey, in my case it is SHIFT-F10, I use it to run "Make project" command in Idea IDE and XCode.

Here is a photo of the resulting device:

![Image Alt](https://raw.github.com/Nepherhotep/doomsday-button/master/img/result.jpg)

Whatch on Youtube how it works:
http://www.youtube.com/watch?v=JafRsrstjKw

Why I decided to build it?
Once I found it quite inconvenient to press Fn+SHIFT+F10 every time I want to build project.
So I had an idea to assemble a big red button and use it as a hotkey.

## Parts ##

After a short research, I decided that my button would be connected via bluetooth as a HID device.
For that purposes I ordered Bluesmirf HID Modem https://www.sparkfun.com/products/10938 

Actually it is a main part of the button since it already can work as a bluetooth HID device.
The modem is controlled with UART port. We also need ATTiny2313 controller as a "brain" of scheme.

Here is a whole listing of necessary parts:
* Bluesmirf HID modem
* ATTiny2313 controller 
* Board 11х16 holes
* ISP AVR programmer. Any will do, as for me I used this one https://www.sparkfun.com/products/9825 
* FTDI TTL cable. Also any will be ok. I used https://www.sparkfun.com/products/9717 . This cable will be very handful to setup modem and to debug ATTiny2313.
* Resistors - 10 kOm (x2 items), 200 Om (x1 item)
* Capacitor - 0.1 µF (x1 item)
* One led
* Break away male headers, right angle and break away female headers. 
* ISP socket

## Bluesmirf HID modem ##

Before we start using it the modem in scheme we need to configure it.


### Wiring ###

First we need to connect modem to computer using FTDI cable and setup some parameters. 
If you don't have FTDI cable, you can use serial port (or usb-to-serial) with special TTL level converter.

Connect modem in a way below:

```
Modem Pins       Cable Pins
            
VCC ------------ VCC

GND ------------ GND

TX-0 ----------- RX

RX-1------------ TX

RTS-0 ----+
          |
CTS-1 ----+
```

### Connecting ###

Launch CoolTerm app, or your favorite terminal application and connect to modem.
Terminal application options - no flow control, baudrate 115200, local echo (optional), CR on "Enter".

### Tweaking ###

Then type in ```$$$``` and modem should response ```CMD``` - it means your are in command mode.

Now you can change your modem's parameters. All the commands has to be followed by CR symbol.

- ```SN,Doomsday button``` - change name to "Doomsday button".
- ```SU,96``` - change baudrate to 9600 instead of 115200. Don't forget to change this option also in the terminal application later.
- ```S~,6``` - change profile to HID instead of default SPP (since 6.10 firmware version). After this command the modem will be visible as a keyboard.
- ```SM,6``` - with this option the modem will reconnect automatically after power off/on.
- ```R,1``` - reboot modem with new parameters.

Type in ```---``` command to quit command mode.

### Testing ###

When you switched to HID profile, try to find a new bluetooth device. After you find it and pair, the modem will connect automatically.
When modem is connected as a keyboard, any command to UART will be transformed into appropriate keypress.

You can test key combination in real using special script I wrote in Python.
https://github.com/Nepherhotep/doomsday-button/blob/master/proto/modem_test_util/key_presses.py
It makes a short delay before hitting a keypress into UART.
You only need to change TTY to your one. Line 23 -> ```ser.port = "/dev/tty.usbserial-AH019ML6"```.

## Control Module ##

Control module is based on ATTiny2313 controller. It controls HID modem with UART port. Scheme is very easy.
Below its picture scrached on paper. Sorry for quallity - it was created in time of assembling :)
![Image Alt](https://raw.github.com/Nepherhotep/doomsday-button/master/img/scheme.jpg)

Attention - the scheme is mirrored.

Pin names look in datasheet http://www.atmel.com/Images/doc2543.pdf .

Scheme description:
*  VCC, GND, RX, TX connect to UART port
*  MOSI, MISO, GND, VCC, SCK, RST pins connect to appropriate ISP pins. RST connect through 10 kOm resistor to VCC
*  Connect led to pin 12 (PB0), another led pin connect through 200 Om resistor to VCC.
*  Connect switcher to pin 6 (INT0) and to GND. The capacitor is required to remove micro "tinkling".
*  Connect 10 kOm resistor to pin 6 and VCC.

### Ledblink testing ###

After ISP and led are connected, the scheme can be tested with led blinking firmware. Look it here doomsday-button/proto/ledblink.

You need to install programmer drivers and avrdude. For Mac OS X just install CrossPack http://www.obdev.at/products/crosspack/index.html

Run ```make flash``` to build firmware and to upload it to controller. Probably, you'll need to change line ```PROGRAMMER = -c usbtiny -P usb``` to your programmer version.


### UART testing ###

UART schematic:
```
VCC -- VCC
GND -- GND
RX -- TX
TX -- RX
```

* Connect FTDI cable to scheme UART port
* Go to uart_att2313 directory
* Run ./write_fuses.sh script to setup fuses correctly
* Run ```make flash``` to build and upload firmware to controller
* Open your terminal programm

Now when you press switcher, the led will blink and "FE 02 02 42 FE 00" hex combination will send to terminal.
If it works as expected, you can assemble parts all together.

## Assembling ##

Since we used break away header it will be easy.

* Connect modem's UART port to control module UART

```
Modem Pins       Control Module Pins

VCC ------------ VCC

GND ------------ GND

TX-0 ----------- RX

RX-1------------ TX

RTS-0 ----+
          |
CTS-1 ----+
```
* Connect switcher to control module
* Connect on/off switcher to control module
* Connect battery supply

![Image Alt](https://raw.github.com/Nepherhotep/doomsday-button/master/img/assembling.jpg)
![Image Alt](https://raw.github.com/Nepherhotep/doomsday-button/master/img/assembling2.jpg)

The last thing - put everything inside the box. And that's it, the button is ready!

![Image Alt](https://raw.github.com/Nepherhotep/doomsday-button/master/img/result.jpg)
![Image Alt](https://raw.github.com/Nepherhotep/doomsday-button/master/img/result2.JPG)

Thanks you read to the end :)