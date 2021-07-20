
#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H




class SensorHandler {    

    private:
        bool running;


    public:
        
        // constructor
        SensorHandler(int);
        // default constructor
        SensorHandler();
        // destructor
        ~SensorHandler();

        void SensorReaderWriter();



};
#endif