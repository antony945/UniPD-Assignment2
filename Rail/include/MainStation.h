#ifndef MainStation_h
#define MainStation_h
#include "Station.h"

class MainStation : public Station {
private:
    static int NUM_OF_MAIN_STATIONS;
public:
    static int SIZE() { return NUM_OF_MAIN_STATIONS; }
public:
    // Costruttore con nome e distanza da origine (deve aumentare NUM_OF_MAIN_STATIONS)
    MainStation(const std::string&, int);
    
    bool isMain() const override { return true; }
};

#endif // MainStation_h