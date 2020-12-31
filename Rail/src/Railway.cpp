#include "Railway.h"
#include <string>
#include <sstream>

/* ---------------------------------------------------------- METODI PER SIMULAZIONE DEL GIORNO */
void Railway::daySimulation() {
    // QUI DENTRO FA LA SIMULAZIONE COMPLETA DI 1 GIORNO
    // Controlla se succede qualcosa in quel momento
        // cerca in timetable se c'è orario di partenza o arrivo di qualche treno,
        // cerca se siamo avanti di 5 minuti rispetto a timetable e quindi se c'è da far partire il treno
        // controlla se mancano 20km a prossima stazione, in caso manda segnalazione da treno a stazione e abbassa velocita
        // controlla se distnza tra treni è maggiore uguale a 10, deve rimanere cosi, in caso cambia velocità
        // altri controlli che ora non mi vengono in mente
    // avanza di un minuto facendo cambiare la distanza percorsa dal treno con la funzione treno.addDistance(oneMinuteDistance)
    while(!isOver()) {
        manageEvents();
    }
}

// TODO: Da controllare
bool Railway::isOver() {
    for(Train* t : trains) {
        if(! (t->getCurrentDistance() >= stations[stations.size()-1]->getDistance()))
            return false;
        
        // Togli treno dalla lista di treni dato che ha finito il viaggio
        trains.remove(t);
    }
    
    return true;
}

// TODO: Da pensare bene, parte importante
void Railway::manageEvents() {
    // Controlla se succede qualcosa in quel momento
        // cerca in timetable se c'è orario di partenza o arrivo di qualche treno,
        // cerca se siamo avanti di 5 minuti rispetto a timetable e quindi se c'è da far partire il treno
        // controlla se mancano 20km a prossima stazione, in caso manda segnalazione da treno a stazione e abbassa velocita
        // controlla se distnza tra treni è maggiore uguale a 10, deve rimanere cosi, in caso cambia velocità
        // altri controlli che ora non mi vengono in mente
    
    for(Train* t : trains) {
        std::vector<int> trainTimetable = t->getTimetable();

        /* ---------------------------------------------------------- RILEVAZIONI CHILOMETRICHE DEL TRENO */
        if(t->nearNextStation()) { // CONTROLLA SE DEVE INVIARE SEGNALAZIONE A PROSSIMA STAZIONE (20 KM PRIMA DI NEXTSTATIONINDEX)
            // t->sendRequest(Station+ s);
            // t->setNextRail(s->sendAck(Train* t));
        } else if(t->hasToPark()) { // CONTROLLA SE DEVE PARCHEGGIARE E ASPETTARE (5 KM PRIMA DI NEXTSTATIONINDEX)
            // aggiundi treno alla coda del parcheggio
            // park.enqueue(t);
            // setta velocità a 0
        } else if(t->isInStation()) { // CONTROLLA SE DEVE ENTRARE NELLA STAZIONE (5 KM PRIMA DI NEXTSTATIONINDEX)
            // changeRail accetta una reference a una Rail
            // t->changeRail(t->getNextRail());
            // inserisci treno in vettore di rail in station corrispondente
            // cambia velocità a 80
        } else if(t->isArrivedAtStation()) { // CONTROLLA A CHE ORA È ARRIVATO IN STAZIONE (0 KM PRIMA DI NEXTSTATIONINDEX)
            // cambia velocità a 0
            // controlla e setta Ritardo (o anticipo)
            // se è arrivato a stazione metti t->salitaPasseggeri() a true
            // deve diventare false dopo 5 minuti
        } else if(t->hasToStart(currentMinutes)) { // CONTROLLA PARTENZE DEI TRENI DA TIMETABLE SAPENDO CHE MINUTI SONO (0 KM PRIMA DI NEXTSTATIONINDEX)
            // IN QUESTO MINUTO IL TRENO T DEVE PARTIRE DALLA STAZIONE CORRENTE
            // setta velocità a 80  
        } else if(t->isOutStation()) { // CONTROLLA SE STA USCENDO DALLA STAZIONE (5 KM DOPO DI NEXTSTATIONINDEX)
            // t->changeRail(t->getNextRail());
            // inserisci treno in vettore di rail in Railway
            if(t->isFromOrigin()) {
                // rails[0]->addTrain(t);
            } else {
                // rails[1]->addTrain(t);
            }
            // aumenta nextStationIndex di 1
            // cambia velocità del treno in modo da seguire timetable (velocità compresa tra 80 e getMaxSpeed())
        } else if(t->checkNearestTrainDistance()) { // CONTROLLA CHE DISTANZA MINIMA TRA TRENI SIA RISPETTATA
            // cambia velocità in modo che non ci siano più conflitti
        } else { // SE NON CI SONO EVENTI ED È TUTTO OK
            // Aggiungi minuto
            currentMinutes++;

            // In TEORIA non c'è niente da fare devi solo aggiungere la distanza in questa funzione
            double minuteDistance = static_cast<double>(t->getCurrentSpeed()/60);
            t->addDistance(minuteDistance);
        }
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
    mainStations.push_back(stations[i++]);

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
        } catch(const std::exception& e) {
            // Errore in input
            throw std::invalid_argument("ERROR. Incorrect input");
        }

        if(type==0) {
            // Crea stazione principale
            stations.push_back(new MainStation{name, distance});
            // Aggiungi ad array di stazioni principali
            mainStations.push_back(stations[i++]);
        } else if(type==1) {
            // Crea stazione locale
            stations.push_back(new LocalStation{name, distance});
        }
    }
}

void Railway::createTrains() {
    while(!timetables.eof()) {
        std::string line;
        std::getline(timetables, line);
        std::stringstream ss(line);
        // Ottieni ID
        int id;
        ss >> id;
        // Ottieni se andata o ritorno
        int startingFrom;
        ss >> startingFrom;
        // Converti in bool
        bool fromOrigin;
        if(startingFrom == 0) {
            fromOrigin = true;
        } else if(startingFrom == 1) {
            fromOrigin = false;
        }

        // Ottieni tipologia treno
        int type;
        ss >> type;

        // Ottieni timetable treno (forse sbagliata)
        std::vector<int> times;
        // Leggi orari
        for(int i=0; !ss.eof(); ++i) {
            int timeStation;
            ss >> timeStation;
            times.push_back(timeStation);
        }

        if(type == 1) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(RegionalTrain::MAX_SPEED, stations, times);

            // Crea regionalTrain
            trains.push_back(new RegionalTrain{id, fromOrigin, times});

            // // Controlla se andata o ritorno
            // if(startingFrom == 0) {
            //     // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS + NUM_LOCAL_STATIONS
            //     for(int i=0, j=0; i<stations.size(); ++i) {
            //         stations[i].addTrain(tr, times[j++]);
            //     }
            // } else if(startingFrom == 1) {
            //     // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS + NUM_LOCAL_STATIONS
            //     for(int i=stations.size()-1, j=0; i>0; --i) {
            //         stations[i].addTrain(tr, times[j++]);
            //     }
            // }            
        } else if(type == 2) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(AVTrain::MAX_SPEED, mainStations, times);

            // Crea AVTrain
            trains.push_back(new AVTrain{id, fromOrigin, times});

            // // Controlla se andata o ritorno
            // if(startingFrom == 0) {
            //     // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS
            //     for(int i=0, j=0; i<stations.size(); ++i) {
            //         if(stations[i].isMain())
            //             stations[i].addTrain(tr, times[j++]);
            //     }
            // } else if(startingFrom == 1) {
            //     // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS
            //     for(int i=stations.size()-1, j=0; i>0; --i) {
            //         if(stations[i].isMain())
            //             stations[i].addTrain(tr, times[j++]);
            //     }
            // }
        } else if(type == 3) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(SuperAVTrain::MAX_SPEED, mainStations, times);

            // Crea SuperAVTrain
            trains.push_back(new SuperAVTrain{id, fromOrigin, times});

            // // Controlla se andata o ritorno
            // if(startingFrom == 0) {
            //     // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS
            //     for(int i=0, j=0; i<stations.size(); ++i) {
            //         if(stations[i].isMain())
            //             stations[i].addTrain(tr, times[j++]);
            //     }
            // } else if(startingFrom == 1) {
            //     // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS
            //     for(int i=stations.size()-1, j=0; i>0; --i) {
            //         if(stations[i].isMain())
            //             stations[i].addTrain(tr, times[j++]);
            //     }
            // }
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
