# SmartPlantWatering

---used Hardware for this project---
    - Raspberry Pi 4b (2GB RAM)
    - Arduino Uno Rev3 to read the analog sensor
    - digital DHT11 Sensor (temperature & humidity)
    - analog MAKERFACTORY MF-4838244 Sensor (soil moisture)
    - 5V DC Tauchpumpe
    - Relais 5V/230V

---Required packes---
    - gcc version 9.3.0
    - g++ 9.3.0
    - GNU Make 4.2.1 Built for x86_64-pc-linux-gnu
    - Wiring Pi (gpio version: 2.50)
    - Python 3.7.3
    - pip 18.1
    
    U can get these packages via:       apt install gcc g++ make wiringpi python3-pip

    when python3 and pip are installed run the command:
    - pip3 install dht11    (module for DHT11 temperature & humidity sensor)

---How to Compile---
    - make all

---How to run---
    - start the TCP-Webserver with:     sudo ./server_web           (sudo because the server is running on port 80)
    - start the SensorHandler with:     ./sensor_handler            (can't be executed as sudo, because of the library wiringPi)
    - start the JSON-API with:          sudo python3 Json_API.py

---How to use---
    - using any Webbrowser go to http://<RaspberryPi-IPAddress>     (f.E.: http://192.168.178.12)
    - or using your RaspberryPi go to http://127.0.0.1