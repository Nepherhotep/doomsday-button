#!/usr/bin/env python
# Disconnect HID device from computer
import serial
import time


def disconnect():
    #setup and open serial
    ser = serial.Serial()
    ser.baudrate = 9600
    ser.port = "/dev/tty.usbserial-AH019ML6"
    ser.open()

    #send disconnect command
    ser.write('\x00')
    
    #wait close serial
    time.sleep(1)
    ser.close()


if __name__ == '__main__':
    disconnect()
