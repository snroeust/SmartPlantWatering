# this File prints the Data given from the DHT Sensor
"""import RPi.GPIO as GPIO
import dht11

# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()

# read data using pin 14
instance = dht11.DHT11(pin = 14)
result = instance.read()

if result.is_valid():
    print("Temperature: %d C" % result.temperature)
    print("Humidity: %d %%" % result.humidity)
else:
    print("Error: %d" % result.error_code)"""

#mock object
#print("Temperature: %d C" % 20)
#print("Humidity: %d %%" % 50)
print("Error: %d" % 1)
