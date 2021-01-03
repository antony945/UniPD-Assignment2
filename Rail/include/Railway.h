#ifndef Railway_h
#define Railway_h
#include "Station.h"
#include "Train.h"
#include "Rail.h"
#include <fstream>
#include <vector>

class Railway {
private:
    std::ifstream line_description;
    std::ifstream timetables;
    std::vector<Station> stations;
    std::vector<Train> trains;

public:
    Railway(const std::string& line_description_, const std::string& timetables_);
    ~Railway();

private:
    void createStations();
    void createTrains();
    void checkTimetable();
};

#endif // Railway_h
