
#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H
#include <thread>         
#include <mutex>
#include "SensorInterface.hpp"          


class SensorHandler {    

    private:
        bool running;
        std::mutex mtxSensorInterface;           // mutex for critical section
        SensorInterface* sensorInterface;
        std::mutex mtxConfigData;           // mutex for critical section
        ConfigData* configData;


    public:
        

        SensorHandler();
        // destructor
        ~SensorHandler();

        void SensorReaderWriter();

        void WateringTimer();


        void StopThreads();

        void readConfig();


};
#endif