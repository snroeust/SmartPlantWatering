
#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H
#include <thread>         
#include <mutex>
#include "SensorInterface.hpp"          

class ConfigData{
    public:
        int mode; 
        int interval;
        int mode1duration;
        float threshold;
        int mode2duration;
};

class SensorHandler {    

    private:
        bool running;
        std::mutex mtxSensorInterface;           // mutex for critical section
        SensorInterface* s1;
        std::mutex mtxConfigData;           // mutex for critical section
        ConfigData* configData;


    public:
        
        // constructor
        SensorHandler(int);
        // default constructor
        SensorHandler();
        // destructor
        ~SensorHandler();

        void SensorReaderWriter();

        void WateringTimer();


        void StopThreads();

        void readConfig();
        void writeData();



};
#endif