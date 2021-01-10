#include "Railway.h"
#include <string>
#include <sstream>
#include <algorithm>

/* ---------------------------------------------------------- METODI PER SIMULAZIONE DEL GIORNO */
// tester()
void Railway::tester() {
    output << "LISTA STAZIONI: ";
    for(auto s : stations) {
        output << s->getName() << "; ";
    }
    output << '\n';

    output << "LISTA TRENI\n";
    for(auto t : trains) {
        output << "ID: " << t->getId() << '\n';
        output << "TIMETABLE: ";
        for(auto tt : t->getTimetable()) {
            output << tt << "; ";
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
    output << "---------- START ----------\n\n";
    // QUI DENTRO FA LA SIMULAZIONE COMPLETA DI 1 GIORNO
    bool end = false;
    while(!end) {
        end = true;

        output << getCurrentMinutes() << '\n';

        // Controlla tutti gli eventi dei treni e preparali per l'avanzamento di un minuto
        for(Train* t : trains) {
            if(!t->getEnd()) {
                // output << "TRENO " << t->getId() << '\n';
                end = false;
                manageEvents(t);
                output << "Speed: " << t->getCurrentSpeed() << " km/h\n";
                output << "Distance: " << t->getCurrentDistance() << " km\n";
                output << '\n';
            }
            
            //std::cout << "INDEX STATION: " << t->getNextStationIndex() << '\n';
            //std::cout << "INDEX TIMETABLE: " << t->getTimetableIndex() << '\n';
        }

        // Gestisci i treni parcheggiati per tutte le stazioni e in caso aggiusta velocità
        manageParkedTrains();
        // Controlla distanza tra treni e in caso aggiusta velocità
        // TODO: Da controllare, crea errori
        checkMinimumDistance();
        // Avanza di un minuto la simulazione
        advanceTrains();
    }

    // FINE
    output << "---------- END ----------\n";
}

// manageEvents()
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

    if(t->isInStation()) {
        trainInStation(t); 
    } else {
        trainOutStation(t);
    }
}

// trainOutStation()
void Railway::trainOutStation(Train* t) {
    // Questa funzione deve settare alla massima velocità consentita dal treno la velocita
    t->setMaxSpeed();

    if(t->checkTrainDistance(-20)) {
        if(t->firstTimePre20km()) {
            // Invia segnalazione a stazione
            // Setta canTransit a true o false
            // Setta normalRail a true se si deve fermare, a false altrimenti
            if(t->inAnticipo(currentMinutes)) {
                // Setta canTransit a false
                t->setTransit(false);
                output << "Treno " << t->getId() << " in anticipo. Andrà in parcheggio di " << t->NextStation()->getName() << "." << '\n';
            } else {
                // Manda segnalazione a stazione
                t->sendStationRequest();
                // Treno ha inviato segnalazione a stazione
                output << "Treno " << t->getId() << " ha inviato segnalazione a " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
                
                if(t->itCanTransit()) {
                    output << "Treno " << t->getId() << " a breve arriverà a " << t->NextStation()->getName() << "." << '\n';
                } else {
                    output << "Treno " << t->getId() << " andrà in parcheggio di " << t->NextStation()->getName() << "." << '\n';
                }
            }

            t->setFirstTimePre20km(false);
        }
    } else if(t->checkTrainDistance(-5)) {
        if(t->firstTimePre5km()) {
            if(t->itCanTransit()) {
                // Se ha il binario su cui transitare entra in stazione e isInStation() diventa true
                t->enterStation();
                // Treno ha il permesso di entrare in zona stazione
                output << "Treno " << t->getId() << " sta entrando in " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
                t->setFirstTimePre5km(false);
            } else {
                t->setStop();

                if(!t->isParked()) {
                    // Treno deve andare in parcheggio
                    output << "Treno " << t->getId() << " è andato in parcheggio di " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
                }

                // Altrimenti vai/rimani in parcheggio aspettando l'autorizzazione per uscire dal parcheggio (non entrare in stazione)
                t->setParking(true);
            }
        }
    }
}

// trainInStation()
void Railway::trainInStation(Train* t) {
    // CONTROLLA SU CHE BINARIO DEVE PASSARE IL TRENO
    if(t->onNormalRail())
        t->setLimitedSpeed();
    else
        t->setMaxSpeed();

    if(t->checkTrainDistance(0)) {
        // CASO SPECIALE, PRIMA STAZIONE
        if(t->getCurrentDistance() == 0) {            
            t->setStop();
            
            if(t->hasToStart(currentMinutes)) {
                // Treno sta partendo da stazione
                t->sendStationRequest();
                t->setLimitedSpeed();
                output << "Treno " << t->getId() << " sta partendo da " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
            }

            // Quando deve partire avrà velocità a 80
            return;            
        }

        if(t->firstTime0km()) {
            // SOLO SE TRENO SI DEVE FERMARE
            if(t->hasToStop()) {
                // Questa funzione deve fare fermare il treno ogni qualvolta viene richiamata (quindi mette velocita a 0)
                t->setStop();

                // Questa funzione deve controllare se il treno è appena arrivato al binario della stazione
                if(t->justArrived()) {
                    // Confronta tempo di arrivo con tempo indicato dalla timetable (funzione da override)
                    t->setDelay(currentMinutes);
                    // Treno è appena arrivato in stazione
                    output << "Treno " << t->getId() << " è arrivato a " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << " con " << t->getCurrentDelay() << " minuti di ritardo." << '\n';
                    t->setFirstTimeArrived(false);
                }

                // Controlla se ultima stazione
                if(t->hasFinish()) {
                    // Stampa messaggio finale
                    output << "Treno " << t->getId() << " ha terminato la sua corsa." << '\n';
                    // Setta fine
                    t->setEnd();
                } else {
                    // Se è arrivato qualche minuto in anticipo aspetta orario di partenza
                    if(!t->hasToStart(currentMinutes)) {
                        output << "Treno " << t->getId() << " è fermo alla stazione..." << '\n';
                        return;
                    }

                    // Questa funzione deve solo dire se stationStopTime < 5
                    // Se sta aspettando non c'è nulla da fare
                    if(!t->isWaiting()){
                        // Appena presi i passeggeri parti dal binario con velocità settata a 80
                        t->setLimitedSpeed();
                        output << "Treno " << t->getId() << " sta partendo da " << t->NextStation()->getName() << " alle ore " << getCurrentTime() << "." << '\n';
                        t->setFirstTime0km(false);
                    } else {
                        output << "Treno " << t->getId() << " sta facendo salire passeggeri..." << '\n';
                    }
                }
            }
        }
    } else if(t->checkTrainDistance(5)) {
        if(t->onNormalRail()) {
            // Libera binario con dentro t
            t->NextStation()->freeRail(t);
        } else {
            // Non fare nulla perchè il binario di transito in realtà non è da liberare dato chd non è mai occupato
        }

        // Fai uscire il treno dalla stazione (qui incrementa indice di nextStation e indice di indexTimetable)
        t->exitStation();
        t->setFirstTimePre20km(true);
        t->setFirstTimePre5km(true);
        t->setFirstTime0km(true);
        t->setFirstTimeArrived(true);
    }
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
        int slow_road;
        int time_to_wait;
        if(i==0) {
            slow_road = 5;    // TODO: Quel 5 sarebbe meglio metterlo Station::SLOW_ROAD come static const int
            time_to_wait = 0;
        } else {
            slow_road = 5*2;
            time_to_wait = 5;   // TODO: Questo 5 sarebbe meglio metterlo Station::TIME_TO_WAIT_PASSENGER come static const int
        }
        int fast_road = all_road-slow_road;

        // Prendo tempo di percorrenza segnato
        // Slow speed (TODO: metterla come costante di qualche classe)
        int slow_speed = 80;
        // output << "FAST SPEED: " << fast_speed << '\n';

        // Tempo minimo per fare la tratta (converti in km/min)
        int slow_time = static_cast<int>(slow_road/(slow_speed/60.0)) + 1;
        int fast_time = static_cast<int>(fast_road/(fast_speed/60.0)) + 1;
        // output << "SLOW_TIME: " << slow_time << '\n';
        // output << "FAST_TIME: " << fast_time << '\n';
        int min_time = static_cast<int>(slow_time+fast_time+time_to_wait);
        // output << "MIN TIME: " << min_time << '\n';
        if(i<times.size()-1) {
            int time_to_do = times[i+1]-times[i];
            int offset = min_time-time_to_do;

            // Se MIN_TIME > TIME_TO_DO DEVO SPOSTARE TUTTO AVANTI DELLA DIFFERENZA TRA QUEI DUE
            if(offset > 0) {
                output << "CAMBIATA TIMETABLE\n";
                times[i+1] = times[i+1]+offset;
            }
        } else {
            output << "AGGIUNTO VALORE IN TIMETABLE\n";
            // Se non ci sono abbastanza orari, aggiungi l'orario minimo + 10 minuti
            times.push_back(times[i]+min_time+10);
        }
    }
}

void Railway::checkMinimumDistance() {

	int MIN_DIS = 15;		//distanza minima da rispettare (ho messo 15km per avere un margine: il treno pi� veloce da 300km/h pu� fare al massimo 5km al minuto.)

	for (int i = 0; i < trains.size(); i++) {
        if(!trains[i]->getEnd() && trains[i]->isInStation()) {
            for (int j = i + 1; j < trains.size(); j++) {
                if(!trains[j]->getEnd() && trains[j]->isInStation()) {
                    //controllo prima se i due treni stanno andando sullo stesso binario, poi se sono troppo vicini
                    if ((trains[i]->getLeft() == trains[j]->getLeft()) && (absoluteValue(trains[i]->getCurrentDistance(), trains[j]->getCurrentDistance()) < MIN_DIS)) {
                        std::cout << "VELOCITÀ CAMBIATA\n";
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

int absoluteValue(int x, int y) {	//funzione per calcolare il valore assoluto della sottrazione

	int diff = x > y ? x - y : y - x;

	return diff;
}