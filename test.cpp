// basic file operations
#include <iostream>
#include <fstream>
#include <string>

using namespace std;





std::string executeShell(std::string command)
{
   char buffer[128];
   std::string result = "";

   // Open pipe to file
   FILE *pipe = popen(command.c_str(), "r");
   if (!pipe)
   {
      return "popen failed!";
   }
   
   return result;
}

int main()
{
  executeShell("python3 SensorDHTData.py");
  return 0;
}