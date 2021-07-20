#include "SensorHandler.hpp"
#include "SensorInterface.hpp"

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdint>





SensorHandler::SensorHandler(){
    this->running = true; 
    this->SensorReader();
}

SensorHandler::SensorHandler(){
   
}

SensorHandler::~SensorHandler(){
    
}




bool SensorHandler::SensorReader(){

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

