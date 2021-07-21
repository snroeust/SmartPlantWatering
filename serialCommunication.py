import serial
import time    

global ser
try:
    #ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
except:
    ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)

ser.flush()

while 1:
    print(ser.readline())
    time.sleep(1)


"""inputValue = "Test"
ser.write(inputValue.encode('utf-8'))"""
