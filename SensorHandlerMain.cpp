#include <iostream>
#include "SensorHandler.hpp"





int main (void)
{
  

    std::cout << "SensorHandler Started" << std::endl;
    
    SensorHandler* s1 = new SensorHandler();
    s1->SensorReaderWriter();  




    return 0 ;
}