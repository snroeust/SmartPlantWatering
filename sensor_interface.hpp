#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H
#include <string>


class ConfigData{
    public:
        int mode; 
        int interval;
        int mode1duration;
        float threshold;
        int mode2duration;           
};


class SensorInterface {    

    private:

        float soilHumidity;
        int airTemperature;
        int airHumidity;
        
    public:
        

        SensorInterface();
        ~SensorInterface();

        std::string executeShell(std::string command);

        void setRelais(bool);


};
#endif