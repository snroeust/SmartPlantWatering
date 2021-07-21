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

  

    std::string finalJSON = "{\n\t\"soilMoisture\": ";
    finalJSON += std::to_string(x);
    finalJSON += ",\n\t\"temperature\": ";
    finalJSON += std::to_string(y);
    finalJSON += ",\n\t\"airHumidity\": ";
    finalJSON += std::to_string(z);
     finalJSON += "\n}";

    std::cout << finalJSON << std::endl;

    //delete json and create new one
    std::remove("src/data.json");
    std::cout << "removed src/data.json" << std::endl;

    std::ofstream MyFile("src/data.json");

    // Write to the file
    MyFile << finalJSON;

    // Close the file
    MyFile.close();
}

int main()
{
    writeFile();
}