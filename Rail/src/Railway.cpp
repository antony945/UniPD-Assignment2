#include "Railway.h"
#include <string>
#include <sstream>

Railway::Railway(const std::string& line_description_, const std::string& timetables_) {
    line_description.open(line_description_);
    timetables.open(timetables_);
    createStations();
    createTrains();
}

Railway::~Railway() {
    line_description.clear();
    line_description.close();
    timetables.clear();
    timetables.close();
}

void Railway::createStations() {
    // Ottieni stazione principale
    std::string line;
    std::getline(line_description, line);
    MainStation head{line, 0};
    stations.push_back(head);

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
            MainStation ms{name, distance};
            stations.push_back(ms);
        } else if(type==1) {
            // Crea stazione locale
            LocalStation ls{name, distance};
            stations.push_back(ls);
        }
    }
}

void Railway::createTrains() {
    while(!line_description.eof()) {
        std::string line;
        std::getline(line_description, line);
        std::stringstream ss(line);

        // Ottieni ID
        int id;
        ss >> id;
        // Ottieni se andata o ritorno
        int startingFrom;
        ss >> startingFrom;
        // Ottieni tipologia treno
        int type;
        ss >> type;
        // Ottieni timetable treno (forse sbagliata)
        std::vector<int> times;
        // Leggi orari nel numero corretto
        for(int i=0; !ss.eof(); ++i) {
            int timeStation;
            ss >> timeStation;
            times.push_back(timeStation);
        }

        if(type == 0) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(RegionalTrain::MAX_SPEED, times);

            // Crea regionalTrain
            RegionalTrain tr{id, startingFrom, times};
            trains.push_back(tr);

            // Controlla se andata o ritorno
            if(startingFrom == 0) {
                // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS + NUM_LOCAL_STATIONS
                for(int i=0, j=0; i<stations.size(); ++i) {
                    stations[i].addTrain(tr, times[j++]);
                }
            } else if(startingFrom == 1) {
                // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS + NUM_LOCAL_STATIONS
                for(int i=stations.size()-1, j=0; i>0; --i) {
                    stations[i].addTrain(tr, times[j++]);
                }
            }            
        } else if(type == 1) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(AVTrain::MAX_SPEED, times);

            // Crea AVTrain
            AVTrain tr{id, startingFrom, times};
            trains.push_back(tr);

            // Controlla se andata o ritorno
            if(startingFrom == 0) {
                // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS
                for(int i=0, j=0; i<stations.size(); ++i) {
                    if(stations[i].isMain())
                        stations[i].addTrain(tr, times[j++]);
                }
            } else if(startingFrom == 1) {
                // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS
                for(int i=stations.size()-1, j=0; i>0; --i) {
                    if(stations[i].isMain())
                        stations[i].addTrain(tr, times[j++]);
                }
            }
        } else if(type == 2) {
            // Dopo questa funzione, times contiene timetable corretta
            checkTimetable(SuperAVTrain::MAX_SPEED, times);

            // Crea SuperAVTrain
            SuperAVTrain tr{id, startingFrom, times};
            trains.push_back(tr);

            // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS + NUM_LOCAL_STATIONS
            for(int i=0, j=0; i<stations.size(); ++i) {
                if(stations[i].isMain())
                    stations[i].addTrain(tr, times[j++]);
            }

            // Controlla se andata o ritorno
            if(startingFrom == 0) {
                // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS
                for(int i=0, j=0; i<stations.size(); ++i) {
                    if(stations[i].isMain())
                        stations[i].addTrain(tr, times[j++]);
                }
            } else if(startingFrom == 1) {
                // Leggi orari sapendo che sono uguali a NUM_MAIN_STATIONS
                for(int i=stations.size()-1, j=0; i>0; --i) {
                    if(stations[i].isMain())
                        stations[i].addTrain(tr, times[j++]);
                }
            }
        }        
    }
}

void Railway::checkTimetable(int fast_speed, std::vector<int>& times) {
    for(int i=0; i<times.size()-1; i++) {
        // Prendo distanza tra stazioni
        int all_road = stations[i+1].getDistance()-stations[i].getDistance();
        int slow_road = 5*2;    // TODO: Quel 5 sarebbe meglio metterlo Station::SLOW_ROAD come static const int
        int fast_road = all_road-slow_road;

        // Prendo tempo di percorrenza segnato
        int time_to_wait = 5;   // TODO: Questo 5 sarebbe meglio metterlo Station::TIME_TO_WAIT_PASSENGER come static const int
        int time_to_do = times[i+1]-times[i]-time_to_wait;

        // Slow speed (TODO: metterla come costante di qualche classe)
        int slow_speed = 80;

        // Devo poter percorrere ALL_ROAD in meno di TIME_TO_DO
        // Il minor tempo per percorrere ALL_ROAD rispettando i limiti è:
        // SLOW_ROAD/SLOW_SPEED + FAST_ROAD/FAST_SPEED
        // 10 km / 80/60 km/min + 20 km / 180/60 km/min
        int min_time = slow_road/(slow_speed*1/60) + fast_road/(fast_speed*1/60);

        // std::cout << "Percorro " << all_road << " km in " << time_to_do << " minuti\n";
        // std::cout << "Il minimo ammissibile per farli è " << min_time << " minuti\n";

        int offset = min_time-time_to_do;

        // Se MIN_TIME > TIME_TO_DO DEVO SPOSTARE TUTTO AVANTI DELLA DIFFERENZA TRA QUEI DUE
        if(offset > 0) {
            times[i+1] = times[i+1] + offset;
        }
    }
}