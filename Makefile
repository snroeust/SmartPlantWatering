all:  main.o 
		g++  main.o -o SmartPlantWatering

main.o: main.cpp
		g++ -c main.cpp -std=c++11

clean:
		rm *.o SmartPlantWatering