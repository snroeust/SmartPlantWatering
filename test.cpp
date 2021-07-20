#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream

using namespace std;

string executeShell(string command){

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


int main() {

    int i =0;
    int u = 0;
    int z = 0;


    string command = " echo \'{\n\t\"soilMoisture\": ";
    command.append(to_string(i));
    command.append(",\n\t\"temperature\":");
    command.append(to_string(i));
    command.append(",\n\t\"airHumidity\":");
    command.append(to_string(i));
    command.append("\n}\' > test.txt");
    cout << command << endl;
    executeShell(command);







}
