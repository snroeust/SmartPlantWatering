#include "sensor_interface.hpp"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <sstream>

#define PUMP 1

SensorInterface::SensorInterface()
{
   soilHumidity = 0;
   airTemperature = 0;
   airHumidity = 0;
   this->setDHTDate();
   this->readSerial();
}

SensorInterface::~SensorInterface()
{
}

std::string SensorInterface::executeShell(std::string command)
{

   char buffer[128];
   std::string result = "";

   // Open pipe to file
   FILE *pipe = popen(command.c_str(), "r");
   if (!pipe)
   {
      return "popen failed!";
   }
   // read till end of process:
   while (!feof(pipe))
   {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }
   pclose(pipe);
   return result;
}

void SensorInterface::setDHTDate()
{
   std::string ls = this->executeShell("python3 SensorDHTData.py");

   int temperature = 20;
   int hummidity = 50;

   try
   {
      temperature = std::stoi(ls.substr(ls.find("Temperature:") + 12, ls.find("C") - (ls.find("Temperature:") + 12)));
      hummidity = std::stoi(ls.substr(ls.find("Humidity:") + 9, ls.find("%") - (ls.find("Humidity:") + 9)));

      this->airTemperature = temperature;
      this->airHumidity = hummidity;
   }
   catch (const std::exception &e)
   {
      if (this->airTemperature == 0 && this->airHumidity == 0)
      {
         this->airTemperature = temperature;
         this->airHumidity = hummidity;
      }
      else
      {
         this->airTemperature = this->airTemperature;
         this->airHumidity = this->airHumidity;
      }
   }
}

int SensorInterface::readSerial()
{

   int fd;
   // Setup serial port on ODROID
   if ((fd = serialOpen("/dev/ttyACM0", 9600)) < 0)
   {
      fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
      return 1;
   }
   if (wiringPiSetup() == -1)
   {
      fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
      return 1;
   }

   serialFlush(fd);
   delay(1000);

   char result[4];
   int index = 0;

   while (serialDataAvail(fd))
   {
      char tmp = serialGetchar(fd);
      result[index] = tmp;
      index++;
      std::cout << tmp << std::endl;
   }

   std::string tmpResult(result);
   try
   {

      this->soilHumidity = stof(tmpResult) / 10;
   }
   catch (const std::exception &e)
   {
      std::cout << "EXCEPTION SERIAL" << std::endl;
      std::cout << tmpResult << std::endl;
   }

   serialClose(fd);
   return 0;
}

void SensorInterface::setRelais(bool on)
{
   wiringPiSetup();
   pinMode(1, OUTPUT);

   if (on)
   {
      digitalWrite(1, HIGH); // On
      delay (500) ;
   }
   else
   {
      digitalWrite(1, LOW); // Off
      delay (500);
   }
}
float SensorInterface::getSoilHumidity()
{
   return this->soilHumidity;
}
int SensorInterface::getAirTemperature()
{
   return this->airTemperature;
}
int SensorInterface::getAirHumidity()
{
   return this->airHumidity;
}

void SensorInterface::updateValues()
{
   this->setDHTDate();
   this->readSerial();
}

void SensorInterface::writeJson()
{
   this->executeShell("sudo rm ./src/data.json");
   std::string command = " echo \'{\n\t\"soilMoisture\": ";
   command.append(std::to_string(this->soilHumidity));
   command.append(",\n\t\"temperature\":");
   command.append(std::to_string(this->airTemperature));
   command.append(",\n\t\"airHumidity\":");
   command.append(std::to_string(this->airHumidity));
   command.append("\n}\' > ./src/data.json");
   this->executeShell(command);
}