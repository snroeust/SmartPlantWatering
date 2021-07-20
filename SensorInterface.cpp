#include "SensorInterface.hpp"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <sstream>


using namespace std;


SensorInterface::SensorInterface(){
   soilHumidity = 50;
   airTemperature= 0;
   airHumidity = 0;
   this->setDHTDate();
   this->readSerial();

}

SensorInterface::~SensorInterface(){
  
}

string SensorInterface::executeShell(string command){

    char buffer[128];
   string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }
   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }
   pclose(pipe);
   return result;
}


void SensorInterface::setDHTDate(){
   string ls = this->executeShell("python3 SensorDHTData.py");


   int temperature = 20;
   int hummidity = 50;
  
   try {
      temperature = stoi(ls.substr(ls.find("Temperature:")+12, ls.find("C") - (ls.find("Temperature:")+12)));
      hummidity = stoi(ls.substr (ls.find("Humidity:")+9, ls.find("%") - (ls.find("Humidity:")+9)));

      this->airTemperature = temperature;
      this->airHumidity = hummidity;

   }
   catch (const std::exception& e) { 
      if(this->airTemperature == 0 && this->airHumidity == 0){
         this->airTemperature = temperature;
         this->airHumidity = hummidity;
      }
      else{
         this->airTemperature = this->airTemperature;
         this->airHumidity =  this->airHumidity;
      }
   }
}


int SensorInterface::readSerial(){

   int fd;
   // Setup serial port on ODROID
   if ((fd = serialOpen ("/dev/ttyACM0",9600)) < 0) {
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
      return 1 ;
   }
   if (wiringPiSetup () == -1) {
      fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
      return 1 ;
   }

   serialPrintf(fd,"A"); // send enter key to read data from sensor
   delay(1000);

   char result[4];
   int index =0;

   while (serialDataAvail (fd)) {
      char tmp = serialGetchar(fd);
      result[index] = tmp;
      index++;
   }

   string tmpResult(result);   
   
   try{
      this->soilHumidity = (float)stoi(tmpResult) /10;
   }
     
   catch (const std::exception& e) { 
      cout << "Exception in Serial Communication. " << endl;
      this->soilHumidity = this->soilHumidity;
   }

   serialClose(fd); 
   return 0; 
}


void SensorInterface::setRelais(bool on){
     
   #define PUMP     1
   wiringPiSetup () ;
   pinMode (PUMP, OUTPUT) ;

   if(on){
      digitalWrite (PUMP, 1) ;     // On

   }
   else{
      digitalWrite (PUMP, 0) ;     // Off
   }
  
}
float SensorInterface::getSoilHumidity(){
   return this->soilHumidity;
}
int SensorInterface::getAirTemperature(){
   return this->airTemperature;
}
int SensorInterface::getAirHumidity(){
   return this->airHumidity;
}

void SensorInterface::updateValues(){
   this->setDHTDate();
   this->readSerial();
}


void SensorInterface::writeJson(){
   this->executeShell("sudo rm ./src/data.json");
   string command = " echo ""{\n\t ""soilMoisture"": " + ",\n\t""temperature"": "+  ",\n\t ""airHumidity"": " +  "\n}"" > ./src/data.json";
   cout << command << endl;
   //this->executeShell(command);
}