// Antonio Franzoso, matricola 1219610
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
public:
    // Minimum distance between trains
    static constexpr int MINIMUM_DISTANCE = 10;
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
    // Indici di valori in timetable da eliminare
    std::vector<int> timetable_index_to_delete;

/* ---------------------------------------------------------- METODI INIZIALIZZAZIONE FERROVIA */
public:
    // Costruttore prende in input tre file, uno per creare stazioni, uno per creare treni, uno per restituire output
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
    // Metodo per svolgere simulazione di linea ferroviaria
    void daySimulation();
    // Stampa informazioni sui treni e stazioni della simulazione
    void printInfo();

private:
    // Inizializza vettore di stations
    void createStations();
    // Inizializza vettore di trains
    void createTrains();
    
    // Controlla e gestisce tutti gli eventi possibili
    void manageEvents(Train&);
    // Gestisci eventi di treno in stazione
    void trainInStation(Train&);
    // Gestisci eventi di treno fuori stazione
    void trainOutStation(Train&);
    // Controlle le distanze tra i treni all'uscita della stazione e in caso aggiusta le velocità
    bool checkOutStationDistance(Train&);
    // Controlla la distanza tra tutti i treni fuori dalla stazione e in caso aggiusta le velocità
    void checkMinimumDistance();
    // Gestisci i treni parcheggiati per tutte le stazioni e quelli che vuoi far partire falli entrare in stazione
    void manageParkedTrains();
    // Fai avanzare tutti i treni
    void advanceTrains();
};

#endif // Railway_h
