// Antonio Franzoso, matricola 1219610
#include "Railway.h"
#include <string>
#include <sstream>
#include <algorithm>

/* ---------------------------------------------------------- METODI PER SIMULAZIONE DEL GIORNO */
// printInfo
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

// daySimulation
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

// manageEvents
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

// trainOutStation
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

// trainInStation
void Railway::trainInStation(Train& t) {
    if(t.onNormalRail())
        // Se treno sta andando su binario standard setta velocità a 80 km/h
        t.setLimitedSpeed();
    else
        // Se treno sta andando su binario di transito setta velocità alla massima velocità del treno
        t.setMaxSpeed();

    // Caso in cui treno debba uscire dalla stazione
    if(t.nextStationDistance() >= 5) {
        // Solo se è sul binario standard
        if(t.onNormalRail()) {
            // Controlla se far fermare o meno il treno in base ai treni a meno di 10km
            if(checkOutStationDistance(t))
                return;

            // Libera binario con dentro t
            t.nextStation().freeRail(t);
        } else {
            // Non fare nulla perchè il binario di transito in realtà non è da liberare dato che non viene mai effetivamente occupato
        }
        
        // Setta massima velocità
        t.setMaxSpeed();
        // Treno ha il permesso di uscire da zona stazione
        output << "Il treno " << t.getType() << " " << t.getId() << " sta uscendo da " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
        // Fai uscire il treno dalla stazione (qui incrementa indice di nextStation e indice di indexTimetable)
        t.exitStation();
        return;
    }

    // Caso in cui treno si trovi al binario e debba fermarsi alla stazione (quindi non sia in binario di transito)
    if(t.nextStationDistance() >= 0 && t.firstTime0km() && t.hasToStop()) {
        // CASO SPECIALE, PRIMA STAZIONE
        if(t.getCurrentDistance() == 0) { 
            t.setStop();
            
            // Nel momento in cui deve partire
            if(t.hasToStart(currentMinutes)) {
                // Treno sta partendo da stazione, invia segnalazione
                t.sendStationRequest();
                // Se i binari della prima stazione sono occupati aspetta che se ne liberi almeno uno
                if(!t.itCanTransit()) {
                    return;
                }
                // Setta velocità a 80 km/h
                t.setLimitedSpeed();
                // Setta delay dato che può essere che abbia dovuto aspettare in caso di più di due treni partiti allo stesso orari
                t.setDelay(currentMinutes);
                // Fai partire treno
                output << "Il treno " << t.getType() << " " << t.getId() << " è in partenza dal binario " << t.getStationRail() << " di " << t.nextStation().getName() << " alle ore " << getCurrentTime() << " con " << t.getCurrentDelay() << " minuti di ritardo." << '\n';
                // Evita di far rieseguire il blocco se l'ha già eseguito
                t.setFirstTime0km(false);
            }

            // Quando deve partire avrà velocità a 80
            return;            
        }
    
        // Cntrolla se il treno è appena arrivato al binario della stazione
        if(t.justArrived()) {
            // Ferma il treno
            t.setStop();
            // Confronta tempo di arrivo con tempo indicato dalla timetable
            t.setDelay(currentMinutes);
            // Treno è appena arrivato in stazione
            output << "Il treno " << t.getType() << " " << t.getId() << " è arrivato al binario " << t.getStationRail() << " di " << t.nextStation().getName() << " alle ore " << getCurrentTime() << " con " << t.getCurrentDelay() << " minuti di ritardo." << '\n';
            // Evita di far rieseguire il blocco se l'ha già eseguito
            t.setFirstTimeArrived(false);
        }

        // CASO SPECIALE, ULTIMA STAZIONE
        if(t.hasFinish()) {
            // Stampa messaggio finale
            output << "Il treno " << t.getType() << " " << t.getId() << " ha terminato la sua corsa." << '\n';
            // Setta fine
            t.setEnd();
            // Ferma il treno
            t.setStop();
            // Libera binario
            t.nextStation().freeRail(t);
            // Evita di far rieseguire il blocco se l'ha già eseguito
            t.setFirstTime0km(false);
            return;
        }

        // CASO NORMALE, STAZIONI INTERMEDIE
        // Se è arrivato qualche minuto in anticipo aspetta orario di partenza
        if(!t.hasToStart(currentMinutes)) {
            // Aspetta tenendo fermo il treno
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
            // Fai partire treno
            output << "Il treno " << t.getType() << " " << t.getId() << " è in partenza dal binario " << t.getStationRail() << " di " << t.nextStation().getName() << " alle ore " << getCurrentTime() << "." << '\n';
            // Evita di far rieseguire il blocco se l'ha già eseguito
            t.setFirstTime0km(false);
            return;
        }
    }

    // Se è in stazione e non entra negli altri casi, prosegue con velocità massima se in binario di transito, di 80km/h se binari standard
}

// manageParkedTrains
void Railway::manageParkedTrains() {
    for(Station* s : stations) {
        // Ogni stazione si deve gestire il suo parcheggio facendo uscire i treni che reputa necessario fare uscire
        s->manageParking(currentMinutes);
    }
}

// advanceTrains
void Railway::advanceTrains() {
    // Avanza i minuti
    currentMinutes++;

    // Per ogni treno della simulazione
    for(Train* t : trains) {
        // Se il treno non ha finito la corsa
        if(!t->getEnd())
            // Aggiungi distanza percorsa in un minuto al treno
            t->increaseDistance();
    }
}

// checkOutStationDistance
bool Railway::checkOutStationDistance(Train& t) {
    // Controlla che non ci siano altri treni nello stesso punto della linea che stanno uscendo
    // In caso stoppa quello con velocità più lenta fino a che non si abbia raggiunto una distanza tra i due di 10km
    // guarda negli altri treni
    // Trova treno più vicino
    int min_distance = MINIMUM_DISTANCE;
    Train* tmpT = nullptr;

    for(Train* other : trains) {
        // Skippa il treno attuale
        if(other->getId() == t.getId())
            continue;
        // Skippa treni che vadano dalla parte opposta di t
        if(other->getLeft() != t.getLeft())
            continue;

        int tmp = std::abs(other->getCurrentDistance()-t.getCurrentDistance());
        if(tmp < MINIMUM_DISTANCE) {
            // Se incontro un other già fuori dalla stazione mi fermo semplicemente e lascio passare lui sicuramente
            // fin quando non mi sarà distanza maggiore di 10km
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

    // In questo punto tmpT contiene indirizzo di treno (in stazione e su binari standard) più vicino a t

    // Se tmpT == nullptr distanza minima tra i treni vicini a t è maggiore uguale di 10
    // Non ho trovato un treno a meno di 10km da t
    // Fai avanzare t senza problemi
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
        // Faccio aspettare t
        t.setStop();
        return true;
    }

    // In altri casi sarà t ad andare avanti perchè avrà o velocità maggiore o uguale agli altri
    return false;
}

/* ---------------------------------------------------------- METODI INIZIALIZZAZIONE FERROVIA */
// Railway
Railway::Railway(const std::string& line_description_, const std::string& timetables_, const std::string& output_)
    : line_description{line_description_},
      timetables{timetables_},
      output{output_},
      currentMinutes{0} {
    // Controlla che i file siano aperti nel modo corretto
    if(!line_description.is_open())
        throw std::invalid_argument("ERROR. Unable to open line_description file.");
    if(!timetables.is_open())
        throw std::invalid_argument("ERROR. Unable to open timetables file.");
    if(!output.is_open())
        throw std::invalid_argument("ERROR. Unable to open output file.");
    
    // Crea le stazioni per la simulazione
    createStations();
    // Crea i treni per la simulazione
    createTrains();
    // Controlla/aggiusta timetable di ogni treno
    for(Train* t : trains) {
        // Se timetable aggiunge orari perchè ne mancano non viene considerata modificata
        // Si considera modificata solo se cambia orari a causa di incompatibilità con velocità
        if(t->checkTimetable())
            // Stampa cambiamento di timetable
            output << "La timetable del treno " << t->getType() << " " << t->getId() << " è stata modificata perché non compatibile con la sua velocità massima.\n";
    }
}

// ~Railway
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

// createStations
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
                // Serve per ignorare i valori della timetable relativi a tali stazioni a distanza minore di 20km che vengono ignorate
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

    // Imposta anche distanze di stazioni se tratta viene percorsa da capolinea a origine
    int c = stations[stations.size()-1]->getDistanceLeft();
    for(Station* s : stations) {
        int new_distance = std::abs(s->getDistanceLeft()-c);
        s->setDistanceRight(new_distance);
    }
}

// createTrains
void Railway::createTrains() {
    while(!timetables.eof()) {
        std::string line;
        std::getline(timetables, line);
        std::stringstream ss(line);
        // Ottieni ID
        int id;
        ss >> id;
        // Ottieni verso di percorrenza di tratta
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

            // Se valore si riferiva a stazione eliminata perchè a meno di 20km
            if(j<timetable_index_to_delete.size() && i==timetable_index_to_delete[j]) {
                // Ignora valore e vai avanti
                j++;
            } else {
                // Aggiungi valore in timetable
                times.push_back(timeStation);
            }
        }

        // Ignora treni in partenza il giorno dopo
        if(times[0] >= DAY_MINUTES)
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

// getCurrentTime
std::string Railway::getCurrentTime() const {
    int min = currentMinutes%DAY_MINUTES;
    // Trova ore
    int hour = min/60;
    // Trova minuti
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

    // Restituisci stringa con numero di minuti convertiti in orario HH:MM
    return s;
}

// checkMinimumDistance
void Railway::checkMinimumDistance() {
    // Distanza minima da rispettare
	int MIN_DIS = MINIMUM_DISTANCE;

    // Compara ogni treno della simulazione
	for (int i = 0; i < trains.size(); i++) {
        // Che non abbia finito la tratta..
        if(trains[i]->getEnd())
            continue;
        // che non sia in stazione..
        if(trains[i]->isInStation())
            continue;
        // che non sia parcheggiato..
        if(trains[i]->isParked())
            continue;

        // con ogni altro treno della simulazione
        for (int j = i + 1; j < trains.size(); j++) {
            // che non abbia finito la tratta..
            if(trains[j]->getEnd())
                continue;
            // che non sia in stazione..
            if(trains[j]->isInStation())
                continue;
            // che non sia parcheggiato..
            if(trains[j]->isParked())
                continue;

            // Controllo prima se i due treni stanno andando sullo stesso binario, poi se sono troppo vicini
            if ((trains[i]->getLeft() == trains[j]->getLeft()) && (std::abs(trains[i]->getCurrentDistance() - trains[j]->getCurrentDistance()) < MIN_DIS)) {
                // Se il treno in trains[i] e' piu' veloce del treno in [j] e si trova anche dietro ad esso, riduco la velocita di trains[i] adeguandola a quella di trains[j]
                if (trains[i]->getCurrentSpeed() > trains[j]->getCurrentSpeed() && trains[i]->getCurrentDistance() < trains[j]->getCurrentDistance()) {
                    trains[i]->setSpeed(trains[j]->getCurrentSpeed());
                }
                // Se il treno in trains[j] e' piu' veloce del treno in [i] e si trova anche dietro ad esso, riduco la velocità di trains[j] adeguandola a quella di trains[i]
                else if (trains[i]->getCurrentSpeed() < trains[j]->getCurrentSpeed() && trains[j]->getCurrentDistance() < trains[i]->getCurrentDistance()) {
                    trains[j]->setSpeed(trains[i]->getCurrentSpeed());
                }

                // Altrimenti anche se sono vicini quello che è più veloce sta davanti e non ci sono problemi
            }
        }
	}
}
