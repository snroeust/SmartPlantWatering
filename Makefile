all: server_web sensor_handler

server_web: server_web.o
	g++ server_web.o -o server_web

server_web.o: server_web.cpp
	g++ -c server_web.cpp

sensor_handler: sensor_interface.o sensor_handler.o
	g++ -lwiringPi -pthread sensor_interface.o sensor_handler.o -o sensor_handler

sensor_interface.o: sensor_interface.cpp
	g++ -c sensor_interface.cpp -lwiringPi -std=c++11

sensor_handler.o: sensor_handler.cpp
	g++ -c sensor_handler.cpp -lwiringPi -std=c++11

clean:
	rm *.o server_web sensor_handler test