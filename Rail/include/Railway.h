#ifndef Railway_h
#define Railway_h
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include "Station.h"
#include "Train.h"
#include "MainStation.h"
#include "LocalStation.h"
#include "RegionalTrain.h"
#include "AVTrain.h"
#include "SuperAVTrain.h"
#include "Rail.h"

/*
A ogni minuto che passa:
-Ogni stazione controlla se ha treni (tra quelli in sosta ovviamente) che devono partire.
    Guarda nel suo array di treni e controlla ogni orario.
    Nel caso il treno abbia un ritardo lo somma all'ora della sua originale partenza da quella stazione.
    Dopo aver fatto questo calcolo per tutti gli orari dei treni,
    la stazione confronta tra i treni che devono partire, quale di questi è più veloce e fa partire quello,
    indipendentemente dall’ordine di arrivo da questa stazione.
    (Oppure si può anche far partire quello che è arrivato prima ma sarebbe meno efficiente secondo me).
-Ogni treno:
	- se non fermo, incrementa la distanza percorsa in base alla sua velocità.
	- se fermo nel deposito della stazione incrementa il suo ritardo.
	- se non fermo, controlla la distanza dalla  prossima stazione e nel caso questa fosse inferiore a 	(?non ricordo) manda un segnale alla stazione.
*/

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
    // Vettore di stazioni percorse dall'ultima alla prima
    std::vector<Station*> reverseStations;
    // Vettore di treni
    std::list<Train*> trains;
    // Minuti attualmente passati
    int currentMinutes;

/* ---------------------------------------------------------- METODI INIZIALIZZAZIONE FERROVIA */
public:
    // Costruttore prende in input tre file, uno per creare stazioni, uno per creare treni, uno per restituire output, DEBUGGATO
    Railway(const std::string&, const std::string&, const std::string&);
    // Distruttore libera i file costruiti, DEBUGGATO
    ~Railway();
    // Getter per minutes
    int getCurrentMinutes() const { return currentMinutes; }
    // Metodo per svolgere simulazione di 1 giorno lavorativo
    void daySimulation();
    // Tester
    void tester() {
        // Ottieni lista stazioni per ogni treno
        for(Train* t : trains) {
            t->trainInformation(std::cout);
        }
    }

private:
    // Inizializza vettore di stations
    void createStations();
    // Inizializza vettore di trains
    void createTrains();
    // Controlla timeTable di un treno avente velocità in Km/h MAX_SPEED
    void checkTimetable(int, const std::vector<Station*>&, std::vector<int>&);

    // Controlla e gestisce tutti gli eventi possibili
    void manageEvents(Train* t);
    // Controlla se treno è a tot km da stazione successiva
    bool checkTrainDistance(Train* t, int distance_from_station) {
        double d = stations[t->getNextStationIndex()]->getDistance()+distance_from_station;
        double min = d-2;
        double max = d+2;
        return (t->getCurrentDistance()>=min && t->getCurrentDistance()<=max);
    }
};

#endif // Railway_h
