import serial
import time


def disconnect():
    #setup and open serial
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = "/dev/tty.usbserial-AH019ML6"
    ser.open()

    #send disconnect command
    ser.write('\x00')

    #close serial
    ser.close()


if __name__ == '__main__':
    disconnect()
