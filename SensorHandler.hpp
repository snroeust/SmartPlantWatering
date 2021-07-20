
#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H
#include <thread>         
#include <mutex>
#include "SensorInterface.hpp"          


class SensorHandler {    

    private:
        bool running;



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