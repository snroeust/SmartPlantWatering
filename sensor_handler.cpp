#include "sensor_handler.hpp"
#include "sensor_interface.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdint>
#include <chrono>
#include <thread>
#include <thread> // std::thread
#include <mutex>  // std::mutex
#include <fstream>
#include <sstream> //std::stringstream
#include <ctime>
#include <thread>

SensorHandler::SensorHandler()
{
    this->running = true;

    this->configData = new ConfigData();
    this->readConfig();

    sensorInterface = new SensorInterface();
}

SensorHandler::~SensorHandler()
{
    delete this->configData;
    delete this->sensorInterface;
}

void sensorReaderWriter(SensorHandler *sensorHandler)
{
    std::cout << "starting Thread SensorReaderWriter" << std::endl;
    while (sensorHandler->running)
    {
        std::cout << sensorHandler->sensorInterface->getSoilHumidity() << "  " << sensorHandler->sensorInterface->getAirTemperature() << "  " << sensorHandler->sensorInterface->getAirHumidity() << std::endl;
        sensorHandler->sensorInterface->setRelais(true);
        //s1->writeJson();

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        sensorHandler->sensorInterface->updateValues();
        //s1->writeJson();

        std::cout << sensorHandler->sensorInterface->getSoilHumidity() << "  " << sensorHandler->sensorInterface->getAirTemperature() << "  " << sensorHandler->sensorInterface->getAirHumidity() << std::endl;
        sensorHandler->sensorInterface->setRelais(false);

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

void wateringTimer(SensorHandler *sensorHandler)
{
    int SleepTime = 10000;
    std::cout << "starting Thread WateringTimer" << std::endl;
    std::time_t EndWatering = std::time(0);  
    std::time_t IntervallEnd = 0;

    while (sensorHandler->running)
    {
        std::time_t StartTime = std::time(0);  //seconds since 1970

        // read config 
        sensorHandler->mtxConfigData.lock();
        int mode = sensorHandler->configData->mode;
        int interval = sensorHandler->configData->interval;
        int mode1duration = sensorHandler->configData->mode1duration;
        int threshold = sensorHandler->configData->threshold;
        int mode2duration = sensorHandler->configData->mode2duration;
        sensorHandler->mtxConfigData.unlock();

        //get hummidity Value
        sensorHandler->mtxSensorInterface.lock();
        float soilHumidity = sensorHandler->sensorInterface->getSoilHumidity();
        sensorHandler->mtxSensorInterface.unlock();


        if(mode == 1){
            // Intervall Mode
            if(IntervallEnd = 0){
                //Intervall neu als Option gesetzt
                IntervallEnd = std::time(0)+interval;               
            }
            else{
                //intervall besteht gucken ob pflanze gegossen werden muss
                if(std::time(0) >= IntervallEnd){

                    //Plant need water
                    EndWatering = std::time(0) +  mode1duration;
                    while(std::time(0)<EndWatering){
                        //pump
                        sensorHandler->mtxSensorInterface.lock();
                        sensorHandler->sensorInterface->setRelais(true);
                        sensorHandler->mtxSensorInterface.unlock();
                        // wait 500 millisec
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    }
                    sensorHandler->mtxSensorInterface.lock();
                    sensorHandler->sensorInterface->setRelais(false);
                    sensorHandler->mtxSensorInterface.unlock();

                    //intervall wird auf null gesetzt damit der Modi sich aendern kann 
                    //ansonsten wir das intervall anhand der ersten If im naechen durchlaufgesetzt
                    IntervallEnd = 0;
                }
            }
        }
        else if(mode == 2){
            // automatic mode
            if(soilHumidity < threshold){
                //Plant need water
                EndWatering = std::time(0) +  mode2duration;
                while(std::time(0)<EndWatering){
                    //pump
                    sensorHandler->mtxSensorInterface.lock();
                    sensorHandler->sensorInterface->setRelais(true);
                    sensorHandler->mtxSensorInterface.unlock();
                    // wait 500 millisec
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }

                sensorHandler->mtxSensorInterface.lock();
                sensorHandler->sensorInterface->setRelais(false);
                sensorHandler->mtxSensorInterface.unlock();


            }
        }

        std::time_t EndTime = std::time(0);  //seconds since 1970
        int toSleep = SleepTime-((EndTime - StartTime)*1000));
        if(toSleep > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(toSleep));
        }
    }
}

void SensorHandler::StopThreads()
{
    this->running = false;
}

void SensorHandler::readConfig()
{
    int mode = 0;
    int interval = 0;
    int mode1duration = 0;
    float threshold = 0;
    float mode2duration = 0;

    try
    {

        std::ifstream inFile;
        inFile.open("./src/config.json"); //open the input file

        std::stringstream strStream;
        strStream << inFile.rdbuf();       //read the file
        std::string str = strStream.str(); //str holds the content of the file

        std::cout << str << "\n"
                  << std::endl;
        ; //you can do anything with the string!!!

        size_t End = str.find("}");
        size_t modePos = str.find("mode") + 6;
        size_t modeEndPos = str.find(",");
        mode = stoi(str.substr(modePos, modeEndPos - modePos));
        std::string rest = str.substr(modeEndPos + 1, End - (modeEndPos + 1));

        size_t intervalPos = rest.find("interval") + 10;
        size_t intervalEndPos = rest.find(",");
        interval = stoi(rest.substr(intervalPos, intervalEndPos - intervalPos));
        rest = rest.substr(intervalEndPos + 1, End - (intervalEndPos + 1));

        size_t duration1Pos = rest.find("mode1duration") + 15;
        size_t duration1EndPos = rest.find(",");
        mode1duration = stoi(rest.substr(duration1Pos, duration1EndPos - duration1Pos));
        rest = rest.substr(duration1EndPos + 1, End - (duration1EndPos + 1));

        size_t thresholdPos = rest.find("threshold") + 11;
        size_t thresholdEndPos = rest.find(",");
        threshold = stof(rest.substr(thresholdPos, thresholdEndPos - thresholdPos));
        rest = rest.substr(thresholdEndPos + 1, End - (thresholdEndPos + 1));

        size_t duration2Pos = rest.find("mode2duration") + 15;
        size_t duration2EndPos = rest.find(",");
        mode2duration = stoi(rest.substr(duration2Pos, duration2EndPos - duration2Pos));
        rest = rest.substr(duration2EndPos + 1, End - (duration2EndPos + 1));

        std::cout << " -->  Readed Config : " << mode << " " << interval << " " << mode1duration << " " << threshold << " " << mode2duration << " " << std::endl;

        mtxConfigData.lock();
        this->configData->mode = mode;
        this->configData->interval = interval;
        this->configData->mode1duration = mode1duration;
        this->configData->threshold = threshold;
        this->configData->mode2duration = mode2duration;
        mtxConfigData.unlock();
    }
    catch (const std::exception &e)
    {

        std::cout << "Exception in REad Config" << std::endl;
        this->configData->mode = this->configData->mode;
        this->configData->interval = this->configData->interval;
        this->configData->mode1duration = this->configData->mode1duration;
        this->configData->threshold = this->configData->threshold;
        this->configData->mode2duration = this->configData->mode2duration;
    }
}

int main(void)
{
    SensorHandler *sensorHandler = new SensorHandler();

    std::thread t[2];
    //Launch the sender/receiver thread
    t[0] = std::thread(sensorReaderWriter, sensorHandler);
    t[1] = std::thread(wateringTimer, sensorHandler);

    //Join the threads with the main thread
    t[0].join();
    t[1].join();

    return 0;
}