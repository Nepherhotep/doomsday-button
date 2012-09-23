## Overview ##

First of all, what is a project about? Doomsday Button actually is a wireless keyboard with only one button.
Pressing this button hits a special hotkey, in my case it is SHIFT-F10, I use it to run "Make project" command in Idea IDE and XCode.

Here is a photo of the resulting device:

![Image Alt](https://raw.github.com/Nepherhotep/doomsday-button/master/img/result.jpg)

Whatch on Youtube how it looks like:
http://www.youtube.com/watch?v=JafRsrstjKw

Why I decided to build it?
Once I found it quite inconvenient to press Fn+SHIFT+F10 every time I want to build project.
So I had an idea to assemble a big red button and use it as a hotkey.

## Parts ##

After a short research, I decided that my button would be connected via bluetooth as a HID device.
For that purposes I ordered Bluesmirf HID Modem https://www.sparkfun.com/products/10938 

Actually it is a main part of the button since it already can works as a bluetooth HID device.
The modem is controlled with UART port. We also need ATTiny2313 controller as a "brain" of scheme.

Here is a whole listing of necessary parts:
* Bluesmirf HID modem
* ATTiny2313 controller 
* Board 11х16 holes
* ISP AVR programmer. Any will do, as for me I used this one https://www.sparkfun.com/products/9825 
* FTDI TTL cable. Also any will be ok. I used https://www.sparkfun.com/products/9717 . This cable will be very handful to setup modem and to debug ATTiny2313.
* Resistors - 10 kOm (x2 items), 200 Om (x1 item)
* Capacitor - 0.1 mF (x1 item)
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