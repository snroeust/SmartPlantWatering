#include "SensorHandler.hpp"
#include "SensorInterface.hpp"

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdint>

#include <chrono>
#include <thread>
#include <thread>         // std::thread

#include <mutex>          // std::mutex

#include <fstream>
#include <sstream> //std::stringstream






using namespace std;

SensorHandler::SensorHandler(){
    this->running = true; 
    this->SensorReaderWriter();
}



SensorHandler::~SensorHandler(){
    
}




void SensorHandler::SensorReaderWriter(){

    while(this->running){
        SensorInterface* s1 = new SensorInterface(); 

        cout<< s1->getSoilHumidity() << "  " << s1->getAirTemperature() << "  " << s1->getAirHumidity() << endl;
        s1->setRelais(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); //wait for 500 milliseconds

        s1->updateValues();
        cout<< s1->getSoilHumidity() << "  " << s1->getAirTemperature() << "  " << s1->getAirHumidity() << endl;
        s1->setRelais(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); //wait for 500 milliseconds
    }

}

void SensorHandler::WateringTimer(){

}


void SensorHandler::StopThreads(){
    this->running = false;
}

void SensorHandler::readConfig(){


    

    
}
void SensorHandler::writeData(){


    
}
