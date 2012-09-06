#!/usr/bin/env python
# Try to reconnect HID modem to last computer
import serial
import time


def reconnect():
    #setup and open serial
    ser = serial.Serial()
    ser.baudrate = 9600
    ser.port = "/dev/tty.usbserial-AH019ML6"
    ser.open()

    #send disconnect command
    ser.write('$$$')
    print "Requesting command mode"
    time.sleep(1)
    if ser.read(3) == "CMD":
        print "Command mode enabled"

    ser.write('C\r')
    time.sleep(10)
    
    #close serial
    print "Closing serial"
    ser.close()


if __name__ == '__main__':
    reconnect()
