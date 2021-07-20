
#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H
#include <string>



class SensorInterface {    

    private:

        int soilHumidity;
        int airTemperature;
        int airHumidity;
        
    public:
        

        SensorInterface();
        ~SensorInterface();

        std::string executeShell(std::string command);

        void setDHTDate();
        int readSerial();
        void setRelais(bool);

        void updateValues();

        float getSoilHumidity();
        int getAirTemperature();
        int getAirHumidity();

};
#endif