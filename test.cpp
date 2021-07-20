#include <wiringPi.h>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdint>
#include <chrono>
#include <fstream>
#include <sstream> //std::stringstream
#include<ctime>
#include <thread>         
#include <mutex>
#include <string>


int main (void)
{
  wiringPiSetup () ;
  pinMode (1, OUTPUT) ;
  for (;;)
  {
    digitalWrite (1, HIGH) ; delay (500) ;
    digitalWrite (1,  LOW) ; delay (500) ;
  }
  return 0 ;
}
