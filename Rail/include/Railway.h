#ifndef Railway_h
#define Railway_h
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include "Station.h"
#include "Train.h"
#include "MainStation.h"
#include "SecondaryStation.h"
#include "RegionalTrain.h"
#include "AVTrain.h"
#include "SuperAVTrain.h"

class Railway {
private:
    // Minuti totali in un giorno
    static const int DAY_MINUTES = 1440;
    // File input per creare stazioni
    std::ifstream line_description;
    // File input per creare treni
    std::ifstream timetables;
    // File per l'output
    std::ofstream output;
    // Vettore di stazioni
    std::vector<Station*> stations;
    // Vettore di treni
    std::vector<Train*> trains;
    // Minuti attualmente passati
    int currentMinutes;

/* ---------------------------------------------------------- METODI INIZIALIZZAZIONE FERROVIA */
public:
    // Costruttore prende in input tre file, uno per creare stazioni, uno per creare treni, uno per restituire output, DEBUGGATO
    Railway(const std::string&, const std::string&, const std::string&);
    // Copy constructor (disabilitato)
    Railway(const Railway&) = delete;
    // Move constructor (disabilitato)
    Railway(Railway&&) = delete;
    // Copy assignment (disabilitato)
    Railway& operator==(const Railway&) = delete;
    // Move assignment (disabilitato)
    Railway& operator==(Railway&&) = delete;

    // Distruttore libera i file costruiti
    ~Railway();
    // Getter per minutes
    int getCurrentMinutes() const { return currentMinutes; }
    // Minuti in stringa (es. se minuti sono 90 deve restituire 01:30)
    std::string getCurrentTime() const;
    // Metodo per svolgere simulazione di 1 giorno lavorativo
    void daySimulation();
    // Tester
    void tester();

private:
    // Inizializza vettore di stations
    void createStations();
    // Inizializza vettore di trains
    void createTrains();
    // Controlla timeTable di un treno avente velocità in Km/h MAX_SPEED
    void checkTimetable(int, const std::vector<Station*>&, std::vector<int>&);
    // Controlla e gestisce tutti gli eventi possibili
    void manageEvents(Train*);
    // Gestisci eventi di treno in stazione
    void trainInStation(Train*);
    // Gestisci eventi di treno fuori stazione
    void trainOutStation(Train*);
    // Controlla la distanza tra tutti i treni IN STAZIONE (!!!) e in caso aggiusta le velocità
    void checkMinimumDistance();
    // Gestisci i treni parcheggiati per tutte le stazioni e quelli che vuoi far partire falli entrare in stazione
    void manageParkedTrains();
    // Fai avanzare tutti i treni
    void advanceTrains();
};

int absoluteValue(int x, int y);

#endif // Railway_h
