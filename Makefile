all: server_web.o
	g++ server_web.o -o server_web

server_web.o: server_web.cpp
	g++ -c server_web.cpp


SensorHandler: SensorHandlerMain.o SensorInterface.o SensorHandler.o
		g++ -lwiringPi SensorHandlerMain.o SensorInterface.o SensorHandler.o -o SensorHandler

SensorInterface.o: SensorInterface.cpp
		g++ -c -lwiringPi SensorInterface.cpp -std=c++11

SensorHandler.o: SensorHandler.cpp
		g++ -c -lwiringPi -pthread SensorHandler.cpp -std=c++11

SensorHandlerMain.o: SensorHandlerMain.cpp
		g++ -c -lwiringPi SensorHandlerMain.cpp -std=c++11

	
test: test.o 
		g++ -ljsoncpp test.o -o test

test.o: test.cpp
		g++ -c -ljsoncpp test.cpp -std=c++11


clean:
	rm *.o server_web SensorHandler test