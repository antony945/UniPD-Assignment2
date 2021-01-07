#include "Railway.h"
#include <string>
#include <sstream>
#include <algorithm>

/* ---------------------------------------------------------- METODI PER SIMULAZIONE DEL GIORNO */
// tester()
void Railway::tester() {
    output << "TESTER\n";
}

// daySimulation()
void Railway::daySimulation() {
    // INIZIO
    output << "START\n";
    // QUI DENTRO FA LA SIMULAZIONE COMPLETA DI 1 GIORNO
    while(!trains.empty()) {
        // Controlla tutti gli eventi dei treni e preparali per l'avanzamento di un minuto
        for(Train* t : trains) {
            if(t->hasFinish()) {
                trains.remove(t);
                delete t;
            } else {
                manageEvents(t);
            }
        }
        
        // Avanza di un minuto la simulazione
        advanceTrains();

    }
    // FINE
    output << "END\n";
}

// manageEvents() TODO: Da pensare bene, parte importante
void Railway::manageEvents(Train* t) {
    // Supponiamo:
    //      - TRENO 1 R in stazione 0 con timetable 100(partenza), 200(arrivo prima stazione secondaria), 250, 300(arrivo ultima stazione)
    //      - TRENO 2 AV in stazione 0 con timetable 150(partenza), 250(arrivo ultima stazione)
    //      - TRENO 3 SAV in stazione 0 con timetable 180(partenza), 220(arrivo ultima stazione)
    
    // Controlla posizione del treno:
    // SE TRENO VA NORMALE
    // |----|                      |------|                      |------|                      |----|
    // |ST  |---------->-----------|  ST  |---------->-----------|  ST  |---------->-----------|  ST|
    // |----|                      |----- |                      |------|                      |----|
    // 0    5                      15 20 25                      35 40 45                      55  60

    // SE TRENO VA AL CONTRARIO
    // |----|                      |------|                      |------|                      |----|
    // |ST  |----------<-----------|  ST  |----------<-----------|  ST  |----------<-----------|  ST|
    // |----|                      |----- |                      |------|                      |----|
    // 0    5                      15 20 25                      35 40 45                      55  60
    
    // Se non è in stazione (treno non compreso tra le barrette | S |):
    //      - Setta velocità massima per quel determinato treno
    //      - Controlla se con quella velocità lui e il treno subito prima, dopo un minuto rispetterebbero la distanza di 10km, altrimenti diminuisci velocità
    //      Inoltre controlla:
    //      - Se siamo in momento di uscita dalla stazione, libera binario di stazione e aumenta nextStationIndex
    //          - Qui controlla se c'è un treno fermo in parcheggio che deve arrivare in stazione, se si settalo in uno stato da farlo andare nel binario appena liberato
    //      - Se siamo in momento di invio segnalazione a stazione, invia segnalazione a prossima stazione
    //      - Se siamo in momento di arrivo alla stazione e deve parcheggiare perchè stazione piena o perchè è troppo in anticipo, fermati in parcheggio
    //      - Se siamo in momento di arrivo alla stazione e deve mettersi in un binario, mettiti a 80km/h e vai sul binario segnalato (entra "in stazione")
    //          - In questo caso ricadono anche i treni che devono uscire dal parcheggio e mettersi in un binario
    // Se è in stazione (treno compreso tra le barrette | S |):
    //      - Se siamo in momento di treno al binario e deve prendere passeggeri, controlla orario di arrivo di timetable per eventuali ritardi,
    //        aspetta 5 minuti e poi mettiti in stato "deve partire"
    //      - Se siamo in momento di treno al binario e ha gia preso i passeggeri, controlla se binario unico principale è disponibile perchè non possono
    //        partire dalla stessa stazione più treni contemporaneamente
    //          - Se si, setta velocità a 80 km/h
    //
    // Dopo tutto ciò la funzione advanceTrains() farà avanzare la simulazione di un minuto e quindi i treni nel modo corretto

    /* ---------------------------------------------------------- RILEVAZIONI CHILOMETRICHE DEL TRENO */
    // if(checkTrainDistance(t, -20)) { // 20 KM PRIMA DI NEXTSTATIONINDEX
    //     // Invia segnalazione a prossima stazione
    //     // t->sendStopRequest(stations[t->getNextStationIndex()]);
    //     // Ricevi segnalazione da stazione
    //     // stations[t->getNextStationIndex()]->
    //     // t->setNextRail(s->sendAck(Train* t));
    // } else if(checkTrainDistance(t, -5)) {  // 5 KM PRIMA DI NEXTSTATIONINDEX
    //     // A seconda di ciò che ha inviato la stazione (se parcheggio o numero di binario)
    //     if(t->hasToPark()) { // CONTROLLA SE DEVE PARCHEGGIARE E ASPETTARE
    //         // aggiundi treno alla vettore dei treni parcheggiati
    //         // setta velocità a 0
    //     } else if(t->hasToEnterRail()) { // CONTROLLA SE DEVE ENTRARE NELLA STAZIONE
    //         // changeRail accetta una reference a una Rail
    //         // t->changeRail(t->getNextRail());
    //         // inserisci treno in vettore di rail in station corrispondente
    //         // cambia velocità a 80   
    //     }
    // } else if(checkTrainDistance(t, 0)) { // 0 KM PRIMA DI NEXTSTATIONINDEX, TODO: To check
    //     if(t->hasJustArrived(currentMinutes)) { // CONTROLLA SE È APPENA ARRIVATO IN STAZIONE È ARRIVATO IN STAZIONE
    //         // cambia velocità a 0
    //         // controlla e setta Ritardo (o anticipo)
    //         // se è arrivato a stazione metti t->salitaPasseggeri() a true
    //     } else if(t->hasToStart(currentMinutes)) { // CONTROLLA PARTENZE DEI TRENI DA TIMETABLE SAPENDO CHE MINUTI SONO (0 KM PRIMA DI NEXTSTATIONINDEX)
    //         // IN QUESTO MINUTO IL TRENO T DEVE PARTIRE DALLA STAZIONE CORRENTE
    //         // setta velocità a 80
    //     }
    // } else if(checkTrainDistance(t, 5)) { // 5 KM DOPO DI NEXTSTATIONINDEX
    //     // Fai in modo che treno esca dalla stazione
    //     // t->changeRail(t->getNextRail());
       
    //     // Avanza indice di nextStation
    //     t->setNextStation();

    //     // cambia velocità del treno in modo da seguire timetable (velocità compresa tra 80 e getMaxSpeed())
    // } else if(t->checkNearestTrainDistance()) { // CONTROLLA CHE DISTANZA MINIMA TRA TRENI SIA RISPETTATA
    //     // cambia velocità in modo che non ci siano più conflitti
    // } else { // SE NON CI SONO EVENTI ED È TUTTO OK
    
    // }
}

// advanceTrains()
void Railway::advanceTrains() {
    // Avanza i minuti
    currentMinutes++;

    for(Train* t : trains) {
        // Aggiungi distanza percorsa in un minuto ad ogni treno
        t->increaseDistance();
    }
}


/* ---------------------------------------------------------- METODI INIZIALIZZAZIONE FERROVIA */
Railway::Railway(const std::string& line_description_, const std::string& timetables_, const std::string& output_)
    : line_description{line_description_},
      timetables{timetables_},
      output{output_},
      currentMinutes{0} {
    if(!line_description.is_open())
        throw std::invalid_argument("ERROR. Unable to open line_description file.");
    if(!timetables.is_open())
        throw std::invalid_argument("ERROR. Unable to open timetables file.");
    if(!output.is_open())
        throw std::invalid_argument("ERROR. Unable to open output file.");
    
    createStations();
    createTrains();
}

Railway::~Railway() {
    // Libera memoria da array di Station* stations
    for(auto i : stations)
        delete i;
    // Libera memoria da array di Train* trains
    for(auto i : trains)
        delete i;
    
    // Libera file di line_description
    line_description.clear();
    line_description.close();
    // Libera file di timetables
    timetables.clear();
    timetables.close();
    // Libera file di output
    output.clear();
    output.close();
}

void Railway::createStations() {
    // Ottieni stazione principale
    int i = 0;
    std::string line;
    std::getline(line_description, line);
    stations.push_back(new MainStation(line, 0));

    // Ottieni tutte le altre stazioni
    while(!line_description.eof()) {
        std::getline(line_description, line);
        std::string name;
        int type;
        int distance;

        // Trova indice di fine nome
        int endName;
        if(line.find(" 0") != std::string::npos) {
            // tipo 0
            endName = line.find(" 0");
        } else if(line.find(" 1") != std::string::npos) {
            // tipo 1
            endName = line.find(" 1");
        }

        try {
            // Estrai nome
            name = line.substr(0, endName);
            // Estrai tipo
            type = std::stoi(line.substr(endName+1, 1));
            // Estrai distanza
            distance = std::stoi(line.substr(endName+3));

            // Controlla che distanza non sia minore uguale di 20km
            if(distance <= stations[stations.size()-1]->getDistance()+20)
                continue;
        } catch(const std::exception& e) {
            // Errore in input
            throw std::invalid_argument("ERROR. Incorrect input");
        }

        if(type==0) {
            // Crea stazione principale
            stations.push_back(new MainStation{name, distance});
        } else if(type==1) {
            // Crea stazione locale
            stations.push_back(new SecondaryStation{name, distance});
        }
    }

    // Controlla che ultima stazione sia principale
    if(!stations[stations.size()-1]->isMain())
        throw std::invalid_argument("ERROR. Last station must be a main station.");
}

void Railway::createTrains() {
    while(!timetables.eof()) {
        std::string line;
        std::getline(timetables, line);
        std::stringstream ss(line);
        // Ottieni ID
        int id;
        ss >> id;
        // Ottieni metodo di percorrenza di tratta
        int startingFrom;
        ss >> startingFrom;
        // Converti in bool
        bool fromOrigin;
        if(startingFrom == 0) {
            fromOrigin = true;
        } else if(startingFrom == 1) {
            fromOrigin = false;
        }

        // Crea vettore di stazioni da assegnare al treno a seconda se parta o meno dall'origine
        std::vector<Station*> trainStations{stations};
        if(!fromOrigin) {
            std::reverse(trainStations.begin(), trainStations.end());
        }

        // Crea vettore di stazioni principali da utilizzare in checkTable
        std::vector<Station*> mainTrainStations;
        for(Station* s : trainStations)
            if(s->isMain()) mainTrainStations.push_back(s);

        // Ottieni tipologia treno
        int type;
        ss >> type;

        // Ottieni timetable treno (da controllare con checkTimetable)
        std::vector<int> times;
        // Leggi orari
        for(int i=0; !ss.eof(); ++i) {
            int timeStation;
            ss >> timeStation;
            times.push_back(timeStation);
        }

        if(type == 1) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(RegionalTrain::MAX_SPEED, trainStations, times);
            // Crea regionalTrain
            trains.push_back(new RegionalTrain{id, fromOrigin, trainStations, times});      
        } else if(type == 2) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(AVTrain::MAX_SPEED, mainTrainStations, times);
            // Crea AVTrain
            trains.push_back(new AVTrain{id, fromOrigin, trainStations, times});
        } else if(type == 3) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(SuperAVTrain::MAX_SPEED, mainTrainStations, times);
            // Crea SuperAVTrain
            trains.push_back(new SuperAVTrain{id, fromOrigin, trainStations, times});
        }        
    }
}

void Railway::checkTimetable(int fast_speed, const std::vector<Station*>& stations, std::vector<int>& times) {
    for(int i=0; i<stations.size()-1; i++) {
        // Prendo distanza tra stazioni
        int all_road = stations[i+1]->getDistance()-stations[i]->getDistance();
        int slow_road = 5*2;    // TODO: Quel 5 sarebbe meglio metterlo Station::SLOW_ROAD come static const int
        int fast_road = all_road-slow_road;

        // Prendo tempo di percorrenza segnato
        int time_to_wait = 5;   // TODO: Questo 5 sarebbe meglio metterlo Station::TIME_TO_WAIT_PASSENGER come static const int

        // Slow speed (TODO: metterla come costante di qualche classe)
        int slow_speed = 80;

        // Tempo minimo per fare la tratta (converti in km/min)
        int min_time = (slow_road/(slow_speed/60)) + (fast_road/(fast_speed/60));

        if(i<times.size()-1) {
            int time_to_do = times[i+1]-times[i]-time_to_wait;
            int offset = min_time-time_to_do;

            // Se MIN_TIME > TIME_TO_DO DEVO SPOSTARE TUTTO AVANTI DELLA DIFFERENZA TRA QUEI DUE
            if(offset > 0) {
                output << "CAMBIATA TIMETABLE.\n";
                times[i+1] = times[i+1]+offset;
            }
        } else {
            output << "AGGIUNTO VALORE IN TIMETABLE.\n";
            // Se non ci sono abbastanza orari, aggiungi l'orario minimo + 10 minuti
            times.push_back(times[i]+min_time+10);
        }
    }
}
