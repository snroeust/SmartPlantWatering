import serial
import time 
import json  
import RPi.GPIO as GPIO
import dht11

global ser
global instance

try:
    # initialize GPIO
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.cleanup()
    # read data using pin 14
    instance = dht11.DHT11(pin = 16)
except:
    print("GPIO Exception")


try:
    #ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
except:
    ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)

ser.flush()


dataJson = {
    'soilMoisture': 0,
    'temperature': 0,
    'airHumidity': 0
    }

while 1:
   
    #Read Data
    try:
        
        soilMoisture = int(list(ser.readline().decode('utf-8'))[:-2])
        result = instance.read()

        dataJson = {
            'soilMoisture': soilMoisture,
            'temperature': result.temperature,
            'airHumidity': result.humidity
        }
        
            
    except:
        print("Serial communication Exception")

    #write json
    try:
        with open('src/data.json', 'w') as outfile:
            json.dump(dataJson, outfile)
        
        print("Write Json success " + str(dataJson))
    except:
        print("Write Json Exception")
    
    

    time.sleep(10)
