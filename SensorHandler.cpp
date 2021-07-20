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
#include<ctime>





using namespace std;

SensorHandler::SensorHandler(){
    this->running = true; 


    this->configData = new ConfigData();

    this->readConfig();


    s1 = new SensorInterface();
   

    this->SensorReaderWriter();  

}

SensorHandler::~SensorHandler(){
    delete this->configData;
    delete this->s1;    
}




void SensorHandler::SensorReaderWriter(){
    

    while(this->running){

          
        
        cout<< s1->getSoilHumidity() << "  " << s1->getAirTemperature() << "  " << s1->getAirHumidity() << endl;
        s1->setRelais(true);
        //s1->writeJson();

       
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); //wait for 500 milliseconds
        

        
        s1->updateValues();
        //s1->writeJson();


        cout<< s1->getSoilHumidity() << "  " << s1->getAirTemperature() << "  " << s1->getAirHumidity() << endl;
        s1->setRelais(false);
         

        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); //wait for 500 milliseconds
    }

}




void SensorHandler::WateringTimer(){

    while(this->running){




    }


}


void SensorHandler::StopThreads(){
    this->running = false;
}




void SensorHandler::readConfig(){
    int mode= 0;
    int interval = 0;
    int mode1duration =  0;
    float threshold=  0;
    float mode2duration =  0;
    
    try{

        ifstream inFile;
        inFile.open("./src/config.json"); //open the input file

        stringstream strStream;
        strStream << inFile.rdbuf(); //read the file
        string str = strStream.str(); //str holds the content of the file

        cout << str << "\n" << endl;; //you can do anything with the string!!!

        size_t End = str.find("}");
        size_t modePos = str.find("mode") + 6;
        size_t modeEndPos = str.find(",");
        mode = stoi(str.substr(modePos, modeEndPos - modePos));
        string rest = str.substr(modeEndPos+1, End - (modeEndPos+1));

        size_t intervalPos = rest.find("interval") + 10;
        size_t intervalEndPos = rest.find(",");
        interval = stoi(rest.substr(intervalPos, intervalEndPos - intervalPos));
        rest = rest.substr(intervalEndPos+1, End - (intervalEndPos+1));

        size_t duration1Pos = rest.find("mode1duration") + 15;
        size_t duration1EndPos = rest.find(",");
        mode1duration = stoi(rest.substr(duration1Pos, duration1EndPos - duration1Pos));
        rest = rest.substr(duration1EndPos+1, End - (duration1EndPos+1));

        
        size_t thresholdPos = rest.find("threshold") + 11;
        size_t thresholdEndPos = rest.find(",");
        threshold = stof(rest.substr(thresholdPos, thresholdEndPos - thresholdPos));
        rest = rest.substr(thresholdEndPos+1, End - (thresholdEndPos+1));

        
        size_t duration2Pos = rest.find("mode2duration") + 15;
        size_t duration2EndPos = rest.find(",");
        mode2duration = stoi(rest.substr(duration2Pos, duration2EndPos - duration2Pos));
        rest = rest.substr(duration2EndPos+1, End - (duration2EndPos+1));

        cout << " -->  Readed Config : "<<mode << " "<< interval << " "<<mode1duration << " "<<threshold << " "<<mode2duration << " " << endl;

        mtxConfigData.lock();
        this->configData->mode = mode;
        this->configData->interval = interval;
        this->configData->mode1duration = mode1duration;
        this->configData->threshold = threshold;
        this->configData->mode2duration = mode2duration;
        mtxConfigData.unlock();

    }
    catch (const std::exception& e) { 

        cout << "Exception in REad Config" << endl;
        this->configData->mode = this->configData->mode;
        this->configData->interval = this->configData->interval;
        this->configData->mode1duration = this->configData->mode1duration;
        this->configData->threshold = this->configData->threshold;
        this->configData->mode2duration = this->configData->mode2duration;
    }
}




