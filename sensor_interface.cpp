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
#include <fstream>


#define PUMP 1

SensorInterface::SensorInterface()
{
   
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






