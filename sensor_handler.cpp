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
    std::cout << "starting Thread WateringTimer" << std::endl;
    while (sensorHandler->running)
    {
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