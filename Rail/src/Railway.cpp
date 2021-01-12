#include "Railway.h"
#include <string>
#include <sstream>
#include <algorithm>

/* ---------------------------------------------------------- METODI PER SIMULAZIONE DEL GIORNO */
// printInfo()
void Railway::printInfo() {
    // Stampa lista stazioni
    output << "---------------------- LISTA STAZIONI ----------------------\n";
    for(auto s : stations) {
        output << s->getName() << "; ";
    }
    output << "\n\n";

    // Stampa lista treni con relative stazioni
    output << "----------------------- LISTA TRENI ------------------------\n";
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

// daySimulation()
void Railway::daySimulation() {
    // INIZIO
    output << "-------------------------- START --------------------------\n";
    // Controlla se tutti i treni hanno finito il loro viaggio
    bool end = false;
    while(!end) {
        end = true;

        // Imposta stampa del giorno dato che simulazione può finire anche nel giorno successivo
        if(currentMinutes%DAY_MINUTES == 0) {
            output << "GIORNO " << currentMinutes/DAY_MINUTES << '\n'; 
        }

        for(Train* t : trains) {
            // Solo se il treno non ha finito la sua corsa
            if(!t->getEnd()) {
                end = false;
                // Controlla tutti gli eventi dei treni e preparali per l'avanzamento di un minuto
                manageEvents(*t);
            }
        }

        // Gestisci i treni parcheggiati per tutte le stazioni e in caso aggiusta velocità
        manageParkedTrains();
        // Controlla distanza tra treni fuori dalla stazione e in caso aggiusta velocità
        checkMinimumDistance();
        // Avanza di un minuto la simulazione
        advanceTrains();
    }

    // FINE
    output << "\n--------------------------- END ---------------------------\n";
}

// manageEvents()
void Railway::manageEvents(Train& t) {
    // Se treno è in area della stazione (cioè se si trova sui binari della stazione)
    if(t.isInStation()) {
        // Gestisci eventi di treno in stazione
        trainInStation(t); 
    } else {
        // Gestisci eventi di treno fuori stazione
        trainOutStation(t);
    }
}

// trainOutStation()
void Railway::trainOutStation(Train& t) {
    // Setta alla massima velocità consentita dal treno la velocita
    t.setMaxSpeed();

    // Caso in cui il treno debba entrare nell'area della stazione
    if(t.nextStationDistance() >= -5) {
        // Se ha il binario su cui transitare
        if(t.itCanTransit()) {
            // Entra in stazione e isInStation() diventa true
            t.enterStation();
            // Treno ha il permesso di entrare in zona stazione
            output << "Il treno " << t.getType() << " " << t.getId() << " sta entrando al binario " << t.getStationRail() << " di " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
        } else {
            // Altrimenti se deve parcheggiare e non è gia parcheggiato
            if(!t.isParked()) {
                // Parcheggia il treno
                t.park();
                // Treno deve andare in parcheggio
                output << "Il treno " << t.getType() << " " << t.getId() << " è andato in parcheggio a " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
            }
            // Setta velocità del treno a 0 km/h
            t.setStop();
        }

        // Esci dalla funzione
        return;
    }

    // Caso in cui il treno superi per la prima volta la distanza di -20 km dalla prossima stazione
    if(t.nextStationDistance() >= -20 && t.firstTimePre20km()) {
        // Se riconosce di essere in anticipo non invia segnalazione e si mette in parcheggio
        if(t.isEarly(currentMinutes)) {
            // Setta canTransit a false
            t.setTransit(false);
            // Treno andrà in parcheggio
            output << "Il treno " << t.getType() << " " << t.getId() << " è in anticipo. Andrà in parcheggio a " << t.nextStation().getName() << "." << '\n';
            // Serve per evitare di eseguire l'enunciato più di una volta
            t.setFirstTimePre20km(false);
            return;
        }

        // Se non è in anticipo manda segnalazione a stazione che gli setta il fatto di poter entrare in stazione o meno
        t.sendStationRequest();
        // Treno ha inviato segnalazione a stazione
        output << "Il treno " << t.getType() << " " << t.getId() << " ha inviato segnalazione a " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
        
        // Se stazione fa entrare treno in stazione
        if(t.itCanTransit()) {
            output << "Permesso accordato. Il treno " << t.getType() << " " << t.getId() << " arriverà a " << t.nextStation().getName();
            // Se è sui binari standard
            if(t.onNormalRail())
                output << " al binario " << t.getStationRail() << ".\n";
            else    // Se è sul binario di transito
                output << " al binario di transito.\n";
        } else {
            // Treno non fatto entrare nella stazione, andrà in parcheggio
            output << "Permesso non accordato. Il treno " << t.getType() << " " << t.getId() << " andrà in parcheggio a " << t.nextStation().getName() << "." << '\n';
        }
        // Serve per evitare di eseguire l'enunciato più di una volta
        t.setFirstTimePre20km(false);
        return;
    }

    // Se non è in stazione e non ha eventi da gestire, procede con velocità max_speed
}

// trainInStation()
void Railway::trainInStation(Train& t) {
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

// checkOutStationDistance() - CORRETTA
bool Railway::checkOutStationDistance(Train& t) {
    // Controlla che non ci siano altri treni nello stesso punto della linea che stanno uscendo
    // In caso stoppa quello attualmente dietro fino a che non si abbia raggiunto una distanza tra i due di 10km
    // guarda negli altri treni
    // Trova treno più vicino
    int min_distance = MINIMUM_DISTANCE;
    Train* tmpT = nullptr;

    for(Train* other : trains) {
        if(other->getId() == t.getId())
            continue;

        if(other->getLeft() != t.getLeft())
            continue;

        int tmp = std::abs(other->getCurrentDistance()-t.getCurrentDistance());
        if(tmp < MINIMUM_DISTANCE) {
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
        if(t->checkTimetable())
            output << "La timetable del treno " << t->getType() << " " << t->getId() << " è stata modificata perché non compatibile con la sua velocità massima.\n";
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
    int counter = 0;
    std::string line;
    std::getline(line_description, line);
    stations.push_back(new MainStation(line, 0));

    // Ottieni tutte le altre stazioni
    while(!line_description.eof()) {
        counter++;
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
            if(distance <= stations[stations.size()-1]->getDistanceLeft()+20) {
                timetable_index_to_delete.push_back(counter);
                continue;
            }
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
        for(int i=0, j=0; !ss.eof(); ++i) {
            int timeStation;
            ss >> timeStation;

            if(j<timetable_index_to_delete.size() && i==timetable_index_to_delete[j]) {
                // Ignora valore e vai avanti
                j++;
            } else {
                // aggiungi valore in timetable
                times.push_back(timeStation);
            }
        }
        // Ignora treni in partenza il giorno dopo
        if(times[0] > DAY_MINUTES)
            continue;

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
	int MIN_DIS = MINIMUM_DISTANCE;		//distanza minima da rispettare (ho messo 15km per avere un margine: il treno pi� veloce da 300km/h pu� fare al massimo 5km al minuto.)

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
