#!/usr/bin/env python
# Emulate keypress on the connected host computer
import serial
import time


def serial_key_press():
    #setup and open serial
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = "/dev/tty.usbserial-AH019ML6"
    ser.open()

    #shoot cmd-R
    #ser.write('\xfe\x02\x08\x15')

    #shoot shift-F10
    ser.write('\xfe\x02\x02\x43')

    #release button
    ser.write('\xfe\x00')

    #close serial
    ser.close()


def main():
    #make a short delay before shooting a keypress
    for i in range(3, 0, -1):
        print i
        time.sleep(1)
    print 'shoot'
    serial_key_press()


if __name__ == '__main__':
    main()
