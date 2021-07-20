#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream

using namespace std;

int main() {

    int mode= 0;
    int interval = 123;
    int mode1duration =  10;
    float threshold=  12.500000;
    float mode2duration =  2;

    
    try{

        ifstream inFile;
        inFile.open("./src/config.json"); //open the input file

        stringstream strStream;
        strStream << inFile.rdbuf(); //read the file
        string str = strStream.str(); //str holds the content of the file

        cout << str << "\n" << endl;; //you can do anything with the string!!!

        size_t End = str.find("}");
        size_t modePos = str.find("mode") + 6;
        size_t modeEndPos = str.find(",");
        mode = stoi(str.substr(modePos, modeEndPos - modePos));
        string rest = str.substr(modeEndPos+1, End - (modeEndPos+1));

        size_t intervalPos = rest.find("interval") + 10;
        size_t intervalEndPos = rest.find(",");
        interval = stoi(rest.substr(intervalPos, intervalEndPos - intervalPos));
        rest = rest.substr(intervalEndPos+1, End - (intervalEndPos+1));

        size_t duration1Pos = rest.find("mode1duration") + 15;
        size_t duration1EndPos = rest.find(",");
        mode1duration = stoi(rest.substr(duration1Pos, duration1EndPos - duration1Pos));
        rest = rest.substr(duration1EndPos+1, End - (duration1EndPos+1));

        
        size_t thresholdPos = rest.find("threshold") + 11;
        size_t thresholdEndPos = rest.find(",");
        threshold = stof(rest.substr(thresholdPos, thresholdEndPos - thresholdPos));
        rest = rest.substr(thresholdEndPos+1, End - (thresholdEndPos+1));

        
        size_t duration2Pos = rest.find("mode2duration") + 15;
        size_t duration2EndPos = rest.find(",");
        mode2duration = stoi(rest.substr(duration2Pos, duration2EndPos - duration2Pos));
        rest = rest.substr(duration2EndPos+1, End - (duration2EndPos+1));

        cout << " -->  Readed Config : "<<mode << " "<< interval << " "<<mode1duration << " "<<threshold << " "<<mode2duration << " " << endl;
    

    }
    catch (const std::exception& e) { 
        cout << "Exception in REad Config" << endl;
    }

    
    















}
