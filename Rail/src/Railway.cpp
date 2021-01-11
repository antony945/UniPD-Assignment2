#include "Railway.h"
#include <string>
#include <sstream>
#include <algorithm>

/* ---------------------------------------------------------- METODI PER SIMULAZIONE DEL GIORNO */
// tester()
void Railway::tester() {
    output << "-------- LISTA STAZIONI --------\n";
    for(auto s : stations) {
        output << s->getName() << "; ";
    }
    output << "\n\n";

    output << "------- LISTA TRENI --------\n";
    for(auto t : trains) {
        output << "ID: " << t->getId() << '\n';
        output << "TIMETABLE: ";
        for(auto tt : t->getTimetable()) {
            output << tt << "; ";
        }
        output << '\n';
        output << "STAZIONI: ";
        for(auto ts : t->getStations()) {
            output << ts->getName() << ", ";
            if(t->getLeft()) {
                output << ts->getDistanceLeft();
            } else {
                output << ts->getDistanceRight();
            }
            output << "; ";
        }
        output << '\n';
    }
    output << '\n';
}

std::string Railway::getCurrentTime() const {
    int hour = currentMinutes/60;
    int minutes = currentMinutes%60;
    std::string s;
    
    if(hour < 10) {
        s += "0";
    }
    s += std::to_string(hour);
    s += ":";
    
    if(minutes < 10) {
        s += "0";
    }
    s += std::to_string(minutes);
    return s;
}

// daySimulation()
void Railway::daySimulation() {
    // INIZIO
    output << "---------- START ----------\n";
    // QUI DENTRO FA LA SIMULAZIONE COMPLETA DI 1 GIORNO
    bool end = false;
    while(!end) {
        end = true;

        output << '\n' << getCurrentTime() << '\n';

        // Controlla tutti gli eventi dei treni e preparali per l'avanzamento di un minuto
        for(Train* t : trains) {
            if(!t->getEnd()) {
                // output << "TRENO " << t->getId() << '\n';
                end = false;
                manageEvents(t);
            }
            
            // std::cout << "INDEX STATION: " << t->getNextStationIndex() << '\n';
            // std::cout << "INDEX TIMETABLE: " << t->getTimetableIndex() << '\n';
        }

        // Gestisci i treni parcheggiati per tutte le stazioni e in caso aggiusta velocità
        manageParkedTrains();
        // Controlla distanza tra treni e in caso aggiusta velocità
        // TODO: Da controllare, crea errori
        //checkMinimumDistance();

        for(Train* t : trains) {
            if(!t->getEnd()) {
                //output << "Speed: " << t->getCurrentSpeed() << " km/h\n";
                //output << "Distance: " << t->getCurrentDistance() << " km\n";
                //output << '\n';
            }
        }

        // Avanza di un minuto la simulazione
        advanceTrains();
    }

    // FINE
    output << "\n---------- END ----------\n";
}

// manageEvents()
void Railway::manageEvents(Train* t) {
    // Supponiamo:
    //      - TRENO 1 R in stazione 0 con timetable 100(partenza), 200(arrivo prima stazione secondaria), 250, 300(arrivo ultima stazione)
    //      - TRENO 2 AV in stazione 0 con timetable 150(partenza), 250(arrivo ultima stazione)
    //      - TRENO 3 SAV in stazione 0 con timetable 180(partenza), 220(arrivo ultima stazione)
    
    if(t->isInStation()) {
        trainInStation(t); 
    } else {
        trainOutStation(t);
    }
}

// trainOutStation()
void Railway::trainOutStation(Train* t) {
    // std::cout << t->nextStationDistance() << '\n';

    // Questa funzione deve settare alla massima velocità consentita dal treno la velocita
    t->setMaxSpeed();

    // QUI È MAGGIORE DI -5, DEVE ENTRARE IN STAZIONE
    if(t->nextStationDistance() >= -5) {
        if(t->itCanTransit()) {
            // Se ha il binario su cui transitare entra in stazione e isInStation() diventa true
            t->enterStation();
            // Treno ha il permesso di entrare in zona stazione
            output << "Treno " << t->getId() << " sta entrando a " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
        } else {
            // QUA DEVE ANDARE IN PARCHEGGIO
            if(!t->isParked()) {
                t->park();
                // Treno deve andare in parcheggio
                output << "Treno " << t->getId() << " è andato in parcheggio a " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
            }
            t->setStop();
        }

        return;
    }

    // QUA È TRA -20 E -5
    if(t->nextStationDistance() >= -20 && t->firstTimePre20km()) {
        // QUI È IN ANTICIPO, NON INVIA SEGNALAZIONE, SI METTE CHE ANDRÀ IN PARCHEGGIO
        if(t->inAnticipo(currentMinutes)) {
            // Setta canTransit a false
            t->setTransit(false);
            output << "Treno " << t->getId() << " in anticipo. Andrà in parcheggio a " << t->NextStation()->getName() << "." << '\n';
            t->setFirstTimePre20km(false);
            return;
        }

        // QUI MANDA LA SEGNALAZIONE E ASPETTA RISPOSTA
        // Manda segnalazione a stazione
        t->sendStationRequest();
        // Treno ha inviato segnalazione a stazione
        output << "Treno " << t->getId() << " ha inviato segnalazione a " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
        
        if(t->itCanTransit()) {
            output << "Treno " << t->getId() << " a breve arriverà a " << t->NextStation()->getName();
            if(t->onNormalRail())
                output << " al binario " << t->getStationRail() << ".\n";
            else
                output << " al binario di transito.\n";
        } else {
            output << "Stazione piena. Treno " << t->getId() << " andrà in parcheggio a " << t->NextStation()->getName() << "." << '\n';
        }
        t->setFirstTimePre20km(false);
        return;
    }

    // QUI È CIÒ CHE FA SE NON HA EVENTI DA GESTIRE, OVVERO PROCEDERE CON VELOCITÀ MAX_SPEED
}

// trainInStation()
void Railway::trainInStation(Train* t) {
    // std::cout << t->nextStationDistance() << '\n';

    // CONTROLLA SU CHE BINARIO DEVE PASSARE IL TRENO
    if(t->onNormalRail())
        t->setLimitedSpeed();
    else
        t->setMaxSpeed();

    // QUI È MAGGIORE DI 5
    if(t->nextStationDistance() >= 5) {
        if(t->onNormalRail()) {
            // Libera binario con dentro t
            t->NextStation()->freeRail(t);
            t->setMaxSpeed();
        } else {
            // Non fare nulla perchè il binario di transito in realtà non è da liberare dato chd non è mai occupato
        }

        // Treno ha il permesso di entrare in zona stazione
        output << "Treno " << t->getId() << " sta uscendo da " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
        // Fai uscire il treno dalla stazione (qui incrementa indice di nextStation e indice di indexTimetable)
        t->exitStation();
        return;
    }

    // QUI È AL BINARIO, TRA 0 E 5
    if(t->nextStationDistance() >= 0 && t->firstTime0km() && t->hasToStop()) {
        // CASO SPECIALE, PRIMA STAZIONE
        if(t->getCurrentDistance() == 0) { 
            t->setStop();
            
            if(t->hasToStart(currentMinutes)) {
                // Treno sta partendo da stazione
                t->sendStationRequest();
                if(!t->itCanTransit()) {
                    output << "Treno " << t->getId() << " sta partendo da " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
                    return;
                }
                t->setLimitedSpeed();
                t->setDelay(currentMinutes);
                output << "Treno " << t->getId() << " sta partendo da " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << " con " << t->getCurrentDelay() << " minuti di ritardo." << '\n';
                t->setFirstTime0km(false);
            }

            // Quando deve partire avrà velocità a 80
            return;            
        }
    
        // Questa funzione deve controllare se il treno è appena arrivato al binario della stazione
        if(t->justArrived()) {
            t->setStop();
            // Confronta tempo di arrivo con tempo indicato dalla timetable (funzione da override)
            t->setDelay(currentMinutes);
            // Treno è appena arrivato in stazione
            output << "Treno " << t->getId() << " è arrivato a " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << " con " << t->getCurrentDelay() << " minuti di ritardo." << '\n';
            t->setFirstTimeArrived(false);
        }

        // CASO SPECIALE, ULTIMA STAZIONE (0 < nextStationDistance < 1.5)
        if(t->hasFinish()) {
            // Stampa messaggio finale
            output << "Treno " << t->getId() << " ha terminato la sua corsa." << '\n';
            // Setta fine
            t->setEnd();
            // Set stop
            t->setStop();
            // Libera binario
            t->NextStation()->freeRail(t);
            t->setFirstTime0km(false);
            return;
        }

        // CASO NORMALE, STAZIONI INTERMEDIE
        // Se è arrivato qualche minuto in anticipo aspetta orario di partenza
        if(!t->hasToStart(currentMinutes)) {
            t->setStop();
            // output << "\tTreno " << t->getId() << " è fermo alla stazione..." << '\n';
            return;
        }

        // Qui siamo sicuramente >= al suo orario nella timetable
        if(t->isWaiting()){
            // Qui sta aspettando i passeggeri che salgano
            t->setStop();
            // output << "\tTreno " << t->getId() << " sta facendo salire passeggeri..." << '\n';
            return;
        } else {
            // Appena presi i passeggeri parti dal binario con velocità settata a 80
            t->setLimitedSpeed();
            output << "Treno " << t->getId() << " sta partendo da " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
            t->setFirstTime0km(false);
            return;
        }
    }

    // QUI È TRA -5 E 0
    // SEMPLICEMENTE PROSEGUE CON VELOCITÀ GIUSTA
}

// manageParkedTrains
void Railway::manageParkedTrains() {
    for(Station* s : stations) {
        // ogni stazione si deve gestire il suo parcheggio
        // se ci sono treni che devono uscire allora chiama sendRequest per inviare segnalazione da parte di treno a stazione
        // se segnalazione va a buon fine allora canItTransit diventa true e si toglie il treno dal parcheggio
        // altrimenti canItTransit rimane false e si aspetta
        s->manageParking(currentMinutes);
    }
}

// advanceTrains()
void Railway::advanceTrains() {
    // Avanza i minuti
    currentMinutes++;

    for(Train* t : trains) {
        if(!t->getEnd())
            // Aggiungi distanza percorsa in un minuto ad ogni treno
            t->increaseDistance();
    }
}

/* ---------------------------------------------------------- METODI INIZIALIZZAZIONE FERROVIA */
// CORRETTA
Railway::Railway(const std::string& line_description_, const std::string& timetables_, const std::string& output_)
    : line_description{line_description_},
      timetables{timetables_},
      output{output_},
      currentMinutes{0},
      reported{false} {
    if(!line_description.is_open())
        throw std::invalid_argument("ERROR. Unable to open line_description file.");
    if(!timetables.is_open())
        throw std::invalid_argument("ERROR. Unable to open timetables file.");
    if(!output.is_open())
        throw std::invalid_argument("ERROR. Unable to open output file.");
    
    createStations();
    createTrains();
    for(Train* t : trains) {
        t->checkTimetable();
    }
}

// CORRETTA
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

// CORRETTA
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
            if(distance <= stations[stations.size()-1]->getDistanceLeft()+20)
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

    // Imposta anche distanze se va da capolinea a origine
    int c = stations[stations.size()-1]->getDistanceLeft();
    for(Station* s : stations) {
        int new_distance = std::abs(s->getDistanceLeft()-c);
        s->setDistanceRight(new_distance);
    }
}

// CORRETTA
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
            // Crea regionalTrain
            trains.push_back(new RegionalTrain{id, fromOrigin, trainStations, times});      
        } else if(type == 2) {
            // Crea AVTrain
            trains.push_back(new AVTrain{id, fromOrigin, trainStations, times});
        } else if(type == 3) {
            // Crea SuperAVTrain
            trains.push_back(new SuperAVTrain{id, fromOrigin, trainStations, times});
        }        
    }
}

// DA CONTROLLARE
void Railway::checkMinimumDistance() {

	int MIN_DIS = 15;		//distanza minima da rispettare (ho messo 15km per avere un margine: il treno pi� veloce da 300km/h pu� fare al massimo 5km al minuto.)

	for (int i = 0; i < trains.size(); i++) {
        if(!trains[i]->getEnd() && !trains[i]->isInStation()) {
            for (int j = i + 1; j < trains.size(); j++) {
                if(!trains[j]->getEnd() && !trains[j]->isInStation()) {
                    //controllo prima se i due treni stanno andando sullo stesso binario, poi se sono troppo vicini
                    if ((trains[i]->getLeft() == trains[j]->getLeft()) && (std::abs(trains[i]->getCurrentDistance() - trains[j]->getCurrentDistance()) < MIN_DIS)) {
                        output << "VELOCITÀ CAMBIATA\n";
                        //se il treno in trains[i] e' piu' veloce del treno in [j] e si trova anche dietro ad esso, riduco la velocit� di trains[i]
                        if (trains[i]->getCurrentSpeed() > trains[j]->getCurrentSpeed() && trains[i]->getCurrentDistance() < trains[j]->getCurrentDistance()) {
                            trains[i]->setSpeed(trains[j]->getCurrentSpeed());
                        }
                        //se il treno in trains[j] e' piu' veloce del treno in [i] e si trova anche dietro ad esso, riduco la velocit� di trains[j]
                        else if (trains[i]->getCurrentSpeed() < trains[j]->getCurrentSpeed() && trains[j]->getCurrentDistance() < trains[i]->getCurrentDistance()) {
                            trains[j]->setSpeed(trains[i]->getCurrentSpeed());
                        }
                        //altrimeni anche se sono vicini quello che � pi� veloce sta davanti e non ci sono problemi
                    }
                }
            }
        }
	}
}
