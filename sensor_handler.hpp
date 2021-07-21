
#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H
#include <thread>         
#include <mutex>
#include "sensor_interface.hpp"          


class SensorHandler {    

    private:
       


    public:
        bool running;
        std::mutex mtxSensorInterface;           // mutex for critical section
        SensorInterface* sensorInterface;
        std::mutex mtxConfigData;           // mutex for critical section
        ConfigData* configData;

        SensorHandler();
        // destructor
        ~SensorHandler();

        void SensorReaderWriter();

        void WateringTimer();


        void StopThreads();

        void readConfig();
        void readData();



};
#endif