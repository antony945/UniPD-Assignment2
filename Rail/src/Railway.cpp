#include "Railway.h"
#include <string>
#include <sstream>
#include <algorithm>

/* ---------------------------------------------------------- METODI PER SIMULAZIONE DEL GIORNO */
// tester() - CORRETTA
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

// daySimulation() - CORRETTA (forse)
void Railway::startSimulation() {
    // INIZIO
    output << "---------- START ----------\n";
    // QUI DENTRO FA LA SIMULAZIONE COMPLETA DI 1 GIORNO
    bool end = false;
    while(!end) {
        end = true;

        if(currentMinutes%DAY_MINUTES == 0) {
            output << "GIORNO " << currentMinutes/DAY_MINUTES << '\n'; 
        }

        // output << getCurrentTime() << '\n';

        // Controlla tutti gli eventi dei treni e preparali per l'avanzamento di un minuto
        for(Train* t : trains) {
            if(!t->getEnd()) {
                // output << "Il treno " << t.getType() << " " << t.getId() << '\n';
                end = false;
                manageEvents(*t);
            }
            
            // std::cout << "INDEX STATION: " << t.getNextStationIndex() << '\n';
            // std::cout << "INDEX TIMETABLE: " << t.getTimetableIndex() << '\n';
        }

        // Gestisci i treni parcheggiati per tutte le stazioni e in caso aggiusta velocità
        manageParkedTrains();
        // Controlla distanza tra treni e in caso aggiusta velocità
        // TODO: Da controllare, crea errori
        checkMinimumDistance();

        for(Train* t : trains) {
            if(!t->getEnd()) {
                // output << "Speed: " << t->getCurrentSpeed() << " km/h\n";
                // output << "Distance: " << t->getCurrentDistance() << " km\n";
                // output << '\n';
            }
        }

        // Avanza di un minuto la simulazione
        advanceTrains();
    }

    // FINE
    output << "\n---------- END ----------\n";
}

// manageEvents() - CORRETTA
void Railway::manageEvents(Train& t) {
    // Supponiamo:
    //      - TRENO 1 R in stazione 0 con timetable 100(partenza), 200(arrivo prima stazione secondaria), 250, 300(arrivo ultima stazione)
    //      - TRENO 2 AV in stazione 0 con timetable 150(partenza), 250(arrivo ultima stazione)
    //      - TRENO 3 SAV in stazione 0 con timetable 180(partenza), 220(arrivo ultima stazione)
    
    if(t.isInStation()) {
        trainInStation(t); 
    } else {
        trainOutStation(t);
    }
}

// trainOutStation() - CORRETTA (forse)
void Railway::trainOutStation(Train& t) {
    // std::cout << t.nextStationDistance() << '\n';

    // Questa funzione deve settare alla massima velocità consentita dal treno la velocita
    t.setMaxSpeed();

    // QUI È MAGGIORE DI -5, DEVE ENTRARE IN STAZIONE
    if(t.nextStationDistance() >= -5) {
        if(t.itCanTransit()) {
            // Se ha il binario su cui transitare entra in stazione e isInStation() diventa true
            t.enterStation();
            // Treno ha il permesso di entrare in zona stazione
            output << "Il treno " << t.getType() << " " << t.getId() << " sta entrando al binario " << t.getStationRail() << " di " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
        } else {
            // QUA DEVE ANDARE IN PARCHEGGIO
            if(!t.isParked()) {
                t.park();
                // Treno deve andare in parcheggio
                output << "Il treno " << t.getType() << " " << t.getId() << " è andato in parcheggio a " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
            }
            t.setStop();
        }

        return;
    }

    // QUA È TRA -20 E -5
    if(t.nextStationDistance() >= -20 && t.firstTimePre20km()) {
        // QUI È IN ANTICIPO, NON INVIA SEGNALAZIONE, SI METTE CHE ANDRÀ IN PARCHEGGIO
        if(t.inAnticipo(currentMinutes)) {
            // Setta canTransit a false
            t.setTransit(false);
            output << "Il treno " << t.getType() << " " << t.getId() << " è in anticipo. Andrà in parcheggio a " << t.nextStation().getName() << "." << '\n';
            t.setFirstTimePre20km(false);
            return;
        }

        // QUI MANDA LA SEGNALAZIONE E ASPETTA RISPOSTA
        // Manda segnalazione a stazione
        t.sendStationRequest();
        // Treno ha inviato segnalazione a stazione
        output << "Il treno " << t.getType() << " " << t.getId() << " ha inviato segnalazione a " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
        
        if(t.itCanTransit()) {
            output << "Permesso accordato. Il treno " << t.getType() << " " << t.getId() << " arriverà a " << t.nextStation().getName();
            if(t.onNormalRail())
                output << " al binario " << t.getStationRail() << ".\n";
            else
                output << " al binario di transito.\n";
        } else {
            output << "Permesso non accordato. Il treno " << t.getType() << " " << t.getId() << " andrà in parcheggio a " << t.nextStation().getName() << "." << '\n';
        }
        t.setFirstTimePre20km(false);
        return;
    }

    // QUI È CIÒ CHE FA SE NON HA EVENTI DA GESTIRE, OVVERO PROCEDERE CON VELOCITÀ MAX_SPEED
}

// trainInStation() - CORRETTA (forse)
void Railway::trainInStation(Train& t) {
    // std::cout << t.nextStationDistance() << '\n';

    // CONTROLLA SU CHE BINARIO DEVE PASSARE IL TRENO
    if(t.onNormalRail())
        t.setLimitedSpeed();
    else
        t.setMaxSpeed();

    // QUI È MAGGIORE DI 5
    if(t.nextStationDistance() >= 5) {
        if(t.onNormalRail()) {
            if(checkOutStationDistance(t))
                return;

            // Qui ho treno più vicino a meno di 10km che non ho fatto andare avanti, non è su binari di transito e ha velocità
            // massima maggiore uguale della mia
            // fermo tmpT (anche se si fermerebbe da solo andando avanti), lascio andare t
            // Libera binario con dentro t
            t.nextStation().freeRail(t);
        } else {
            // Non fare nulla perchè il binario di transito in realtà non è da liberare dato chd non è mai occupato
        }
        
        t.setMaxSpeed();
        // Treno ha il permesso di uscire da zona stazione
        output << "Il treno " << t.getType() << " " << t.getId() << " sta uscendo da " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
        // Fai uscire il treno dalla stazione (qui incrementa indice di nextStation e indice di indexTimetable)
        t.exitStation();
        return;
    }

    // QUI È AL BINARIO, TRA 0 E 5
    if(t.nextStationDistance() >= 0 && t.firstTime0km() && t.hasToStop()) {
        // CASO SPECIALE, PRIMA STAZIONE
        if(t.getCurrentDistance() == 0) { 
            t.setStop();
            
            if(t.hasToStart(currentMinutes)) {
                // Treno sta partendo da stazione
                t.sendStationRequest();
                if(!t.itCanTransit()) {
                    return;
                }
                t.setLimitedSpeed();
                t.setDelay(currentMinutes);
                output << "Il treno " << t.getType() << " " << t.getId() << " è in partenza dal binario " << t.getStationRail() << " di " << t.nextStation().getName() << " alle ore " << getCurrentTime() << " con " << t.getCurrentDelay() << " minuti di ritardo." << '\n';
                t.setFirstTime0km(false);
            }

            // Quando deve partire avrà velocità a 80
            return;            
        }
    
        // Questa funzione deve controllare se il treno è appena arrivato al binario della stazione
        if(t.justArrived()) {
            t.setStop();
            // Confronta tempo di arrivo con tempo indicato dalla timetable (funzione da override)
            t.setDelay(currentMinutes);
            // Treno è appena arrivato in stazione
            output << "Il treno " << t.getType() << " " << t.getId() << " è arrivato al binario " << t.getStationRail() << " di " << t.nextStation().getName() << " alle ore " << getCurrentTime() << " con " << t.getCurrentDelay() << " minuti di ritardo." << '\n';
            t.setFirstTimeArrived(false);
        }

        // CASO SPECIALE, ULTIMA STAZIONE (0 < nextStationDistance < 1.5)
        if(t.hasFinish()) {
            // Stampa messaggio finale
            output << "Il treno " << t.getType() << " " << t.getId() << " ha terminato la sua corsa." << '\n';
            // Setta fine
            t.setEnd();
            // Set stop
            t.setStop();
            // Libera binario
            t.nextStation().freeRail(t);
            t.setFirstTime0km(false);
            return;
        }

        // CASO NORMALE, STAZIONI INTERMEDIE
        // Se è arrivato qualche minuto in anticipo aspetta orario di partenza
        if(!t.hasToStart(currentMinutes)) {
            t.setStop();
            // output << "\tTreno " << t.getId() << " è fermo alla stazione..." << '\n';
            return;
        }

        // Qui siamo sicuramente >= al suo orario nella timetable
        if(t.isWaiting()){
            // Qui sta aspettando i passeggeri che salgano
            t.setStop();
            // output << "\tTreno " << t.getId() << " sta facendo salire passeggeri..." << '\n';
            return;
        } else {
            // Appena presi i passeggeri parti dal binario con velocità settata a 80
            t.setLimitedSpeed();
            output << "Il treno " << t.getType() << " " << t.getId() << " è in partenza dal binario " << t.getStationRail() << " di " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
            t.setFirstTime0km(false);
            return;
        }
    }

    // QUI È TRA -5 E 0
    // SEMPLICEMENTE PROSEGUE CON VELOCITÀ GIUSTA
}

// manageParkedTrains - CORRETTA (forse)
void Railway::manageParkedTrains() {
    for(Station* s : stations) {
        // ogni stazione si deve gestire il suo parcheggio
        // se ci sono treni che devono uscire allora chiama sendRequest per inviare segnalazione da parte di treno a stazione
        // se segnalazione va a buon fine allora canItTransit diventa true e si toglie il treno dal parcheggio
        // altrimenti canItTransit rimane false e si aspetta
        s->manageParking(currentMinutes);
    }
}

// advanceTrains() - CORRETTA
void Railway::advanceTrains() {
    // Avanza i minuti
    currentMinutes++;

    for(Train* t : trains) {
        if(!t->getEnd())
            // Aggiungi distanza percorsa in un minuto ad ogni treno
            t->increaseDistance();
    }
}

bool Railway::checkOutStationDistance(Train& t) {
    // Controlla che non ci siano altri treni nello stesso punto della linea che stanno uscendo
    // In caso stoppa quello attualmente dietro fino a che non si abbia raggiunto una distanza tra i due di 10km
    // guarda negli altri treni
    // Trova treno più vicino
    int min_distance = 10;
    Train* tmpT = nullptr;

    for(Train* other : trains) {
        if(other->getId() == t.getId())
            continue;

        if(other->getLeft() != t.getLeft())
            continue;

        int tmp = std::abs(other->getCurrentDistance()-t.getCurrentDistance());
        if(tmp < 10) {
            // Se qua incontro un other già fuori dalla stazione mi fermo semplicemente e lascio passare lui sicuramente
            // fin quando non mi sarà distanza più di 10km
            if(!other->isInStation() || !other->onNormalRail()) {
                t.setStop();
                return true;
            }

            if(tmp<min_distance) {
                min_distance = tmp;
                tmpT = other;
            }
        }
    }

    // Qui distanza minima tra i treni vicini a t è maggiore uguale di 10
    // Qui non ho trovato un treno a meno di 10km da t
    // fai avanzare t senza problemi
    if(tmpT == nullptr) {
        // Libera binario con dentro t
        t.nextStation().freeRail(t);
        t.setMaxSpeed();
        // Treno ha il permesso di entrare in zona stazione
        output << "Il treno " << t.getType() << " " << t.getId() << " sta uscendo da " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
        // Fai uscire il treno dalla stazione (qui incrementa indice di nextStation e indice di indexTimetable)
        t.exitStation();
        return true;
    }

    // Qui ho treno più vicino a meno di 10km ma che non ho fatto gia andare avanti e che non è su binari di transito
    // Se vedo che ha velocità massima > della mia allora lo faccio andare avanti
    if(tmpT->getMaxSpeed() > t.getMaxSpeed()) {
        // mando avanti tmpT, faccio aspettare t
        t.setStop();
        return true;
    }

    return false;
}

/* ---------------------------------------------------------- METODI INIZIALIZZAZIONE FERROVIA */
// CORRETTA
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

// CORRETTA
std::string Railway::getCurrentTime() const {
    int min = currentMinutes%DAY_MINUTES;
    int hour = min/60;
    int minutes = min%60;
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

// DA CONTROLLARE
void Railway::checkMinimumDistance() {
	int MIN_DIS = 10;		//distanza minima da rispettare (ho messo 15km per avere un margine: il treno pi� veloce da 300km/h pu� fare al massimo 5km al minuto.)

	for (int i = 0; i < trains.size(); i++) {
        if(trains[i]->getEnd())
            continue;
        if(trains[i]->isInStation())
            continue;
        if(trains[i]->isParked())
            continue;

        for (int j = i + 1; j < trains.size(); j++) {
            if(trains[j]->getEnd())
            continue;
            if(trains[j]->isInStation())
                continue;
            if(trains[j]->isParked())
                continue;

            //controllo prima se i due treni stanno andando sullo stesso binario, poi se sono troppo vicini
            if ((trains[i]->getLeft() == trains[j]->getLeft()) && (std::abs(trains[i]->getCurrentDistance() - trains[j]->getCurrentDistance()) < MIN_DIS)) {
                output << "distanza: " << std::abs(trains[i]->getCurrentDistance() - trains[j]->getCurrentDistance()) << '\n';
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
