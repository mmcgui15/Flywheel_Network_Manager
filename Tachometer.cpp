#include "Tachometer.h"

char output[MAX_DATA_LENGTH];
char incoming[MAX_DATA_LENGTH];
const char* port = "\\\\.\\COM3";		//sensor port
int reading = 0;


SerialPort arduino(port);

void connection_test() {
    if (arduino.isConnected()) {
        cout << "Connection is Established";
    }
    else {
        cout << "Error in port name";
    }
}

int get_rpm() {

    arduino.readSerialPort(output, MAX_DATA_LENGTH);
    int new_reading = atoi(output);
    if (new_reading != reading) {
        reading = new_reading;
        //cout << "reading = " << reading << '\n';
    }
    return reading;
}

