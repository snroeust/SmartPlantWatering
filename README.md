# SmartPlantWatering

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
    - start the TCP-Webserver with:     ./server_web
    - start the SensorHandler with:     ./SensorHandler

    - using any Webbrowser go to http://<RaspberryPi-IPAddress>     (f.E.: http://192.168.178.12)
    - or using your RaspberryPi go to http://127.0.0.1