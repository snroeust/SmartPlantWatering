
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <errno.h>



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


void writeJson()
{
   //this->executeShell("rm ./src/data.json");
   std::string command = " echo \'{\n\t\"soilMoisture\": ";
   command.append(std::to_string(1));
   command.append(",\n\t\"temperature\":");
   command.append(std::to_string(1));
   command.append(",\n\t\"airHumidity\":");
   command.append(std::to_string(1));
   command.append("\n}\' > ./src/data.json");
   executeShell(command);
}



int main()
{
   writeJson();
    
    return 0;
}