#ifndef Railway_h
#define Railway_h
#include "Station.h"
#include "MainStation.h"
#include "LocalStation.h"
#include "Train.h"
#include "RegionalTrain.h"
#include "AVTrain.h"
#include "SuperAVTrain.h"
#include "Rail.h"
#include <fstream>
#include <vector>

class Railway {
private:
    // File input per creare stazioni
    std::ifstream line_description;
    // File input per creare treni
    std::ifstream timetables;
    // Vettore di stazioni
    std::vector<Station> stations;
    // Vettore di treni
    std::vector<Train> trains;
public:
    // Costruttore prende in input due file, uno per creare stazioni, uno per creare treni
    Railway(const std::string&, const std::string&);
    // Distruttore libera i file costruiti
    ~Railway();

private:
    // Inizializza vettore di stations
    void createStations();
    // Inizializza vettore di trains
    void createTrains();
    // Controlla timeTable di un treno avente velocità in Km/h MAX_SPEED
    void checkTimetable(int, std::vector<int>&);
};

#endif // Railway_h
