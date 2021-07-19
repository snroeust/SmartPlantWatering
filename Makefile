all:  main.o 
		g++ -lwiringPi main.o -o SmartPlantWatering

main.o: main.cpp
		g++ -c -lwiringPi main.cpp -std=c++11

clean:
		rm *.o SmartPlantWatering