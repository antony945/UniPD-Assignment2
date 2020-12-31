#ifndef LocalStation_h
#define LocalStation_h
#include "Station.h"

class LocalStation : public Station {
private:
    static int NUM_OF_LOCAL_STATIONS;
public:
    static int SIZE() { return NUM_OF_LOCAL_STATIONS; }
public:
    // Costruttore con nome e distanza da origine (deve aumentare NUM_OF_LOCAL_STATIONS)
    LocalStation(const std::string&, int);

    bool isMain() const override { return false; }
};

#endif // LocalStation_h