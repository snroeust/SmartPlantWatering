// basic file operations
#include <iostream>
#include <fstream>
#include <string>

using namespace std;





int writeFile () 
{

  float x = 0;
  int y = 0;
  int z = 0;

  

  ofstream DataFile;
  DataFile.open ("./src/data.json");
  DataFile << "{\n";
  DataFile << "\t\"soilMoisture\": ";
  DataFile << to_string(x);
  DataFile << ",\n\t\"temperature\": ";
  DataFile << to_string(y);
  DataFile << ",\n\t\"airHumidity\": ";
  DataFile << to_string(y);
  DataFile << "\n}";
  DataFile.close();
  return 0;
}

int main()
{
    writeFile();
}