#include <iostream>
// #include "Station.h"
// #include "Train.h"
#include <vector>
#include <queue>
#include <sstream>
#include <string>
using namespace std;

class Stazione {
public:
    string nome;
    int distanza;
    static const int SLOW_SPEED = 80;
    static const int SPAZIO_LENTO = 5;
};

class Train {
public:
    int ID; // Identificativo treno
    int speed;  // Velocità attuale treno
    int starting_from;  // 0 se parte da inizio, 1 se da capolinea
    std::vector<int> times; // Orari di attuale corsa

    void printTimes() {
        for(auto i : times) {
            cout << "ORARIO: " << i << '\n';
        }
    }

protected:
    Train(int ID_, int starting_from_, std::vector<int> times_) : ID{ID_}, speed{0}, starting_from{starting_from_}, times{times_} {}
};

class RegionalTrain : public Train {
public:
    RegionalTrain(int ID_, int starting_from_, std::vector<int> times_) : Train(ID_, starting_from_, times_) {}
    static const int MAX_SPEED = 160;   // Massima velocità per questo tipo di treno
};

class HighSpeedTrain : public Train {
public:
    HighSpeedTrain(int ID_, int starting_from_, std::vector<int> times_) : Train(ID_, starting_from_, times_) {}
    static const int MAX_SPEED = 240;   // Massima velocità per questo tipo di treno
};

class SuperHighSpeedTrain : public Train {
public:
    SuperHighSpeedTrain(int ID_, int starting_from_, std::vector<int> times_) : Train(ID_, starting_from_, times_) {}
    static const int MAX_SPEED = 300;   // Massima velocità per questo tipo di treno
};

// orari scritti sottoforma di minuti passati da 00:00
void checkTime(int train_type, const vector<Stazione>& stations, vector<int>& times) {
    // qui ho la massima velocità dello specifico treno
    int FAST_SPEED;
    if(train_type == 1) {
        FAST_SPEED = RegionalTrain::MAX_SPEED;
    } else if(train_type == 2) {
        FAST_SPEED = HighSpeedTrain::MAX_SPEED;
    } else if(train_type == 3) {
        FAST_SPEED = SuperHighSpeedTrain::MAX_SPEED;
    }

    for(int i=0; i<times.size()-1; i++) {
        // Prendo distanza tra stazioni
        int ALL_ROAD = stations[i+1].distanza-stations[i].distanza;
        int SLOW_ROAD = Stazione::SPAZIO_LENTO*2;
        int FAST_ROAD = ALL_ROAD-SLOW_ROAD;

        // Prendo tempo di percorrenza segnato
        int TIME_TO_WAIT = 5;
        int TIME_TO_DO = times[i+1]-times[i]-TIME_TO_WAIT;

        // Devo poter percorrere ALL_ROAD in meno di TIME_TO_DO
        // Il minor tempo per percorrere ALL_ROAD rispettando i limiti è:
        // SLOW_ROAD/SLOW_SPEED + FAST_ROAD/FAST_SPEED
        // 10 km / 80/60 km/min + 20 km / 180/60 km/min
        int MIN_TIME = SLOW_ROAD/(Stazione::SLOW_SPEED*1/60) + FAST_ROAD/(FAST_SPEED*1/60);

        cout << "Percorro " << ALL_ROAD << " km in " << TIME_TO_DO << " minuti\n";
        cout << "Il minimo ammissibile per farli è " << MIN_TIME << " minuti\n";

        int OFFSET = MIN_TIME-TIME_TO_DO;

        // Se MIN_TIME > TIME_TO_DO DEVO SPOSTARE TUTTO AVANTI DELLA DIFFERENZA TRA QUEI DUE
        if(OFFSET > 0) {
            times[i+1] = times[i+1] + OFFSET;
        }

    }
}

// crea treno da riga di file table.txt
Train createTrainFromLine(const vector<Stazione>& stations, const string& str) {
    // List of string to save tokens 
    queue<string> tokens;
    // stringstream class check1 
    stringstream check1(str); 
    string intermediate; 
    // Tokenizing w.r.t. space ' ' 
    while(getline(check1, intermediate, ' ')) { 
        tokens.push(intermediate);
    }
    
    // estrai treno
    int id_;
    id_ = stoi(tokens.front());
    tokens.pop();
    // estrai se parte da inizio o da fine
    int starting_from_;
    starting_from_ = stoi(tokens.front());
    tokens.pop();
    // estrai tipologia di treno
    int train_type_;
    train_type_ = stoi(tokens.front());
    tokens.pop();
    // estrai orari
    vector<int> times_;
    while(!tokens.empty()) {
        times_.push_back(stoi(tokens.front()));
        tokens.pop();
    }

    // controlla orari
    checkTime(train_type_, stations, times_);
    
    // costruisci treno
    if(train_type_ == 1) {
        RegionalTrain tr{id_, starting_from_, times_};
        return tr;
    } else if(train_type_ == 2) {
        RegionalTrain tr{id_, starting_from_, times_};
        return tr;
    } else {
        SuperHighSpeedTrain tr{id_, starting_from_, times_};
        return tr;
    }
}

// crea vettore di stazioni
vector<Stazione> createStations() {
    vector<Stazione> s;
    Stazione sTest;
    // Crea Venezia
    sTest.nome = "Venezia";
    sTest.distanza = 0;
    s.push_back(sTest);
    // Crea Padova
    sTest.nome = "Padova";
    sTest.distanza = 30;
    s.push_back(sTest);
    // Crea Bologna
    sTest.nome = "Bologna";
    sTest.distanza = 50;
    s.push_back(sTest);
    return s;
}

int main(void) {
    vector <Stazione> st = createStations();
    string input;
    getline(cin, input);
    Train tr = createTrainFromLine(st, input);
    tr.printTimes();
    getline(cin, input);
    tr = createTrainFromLine(st, input);
    tr.printTimes();
    getline(cin, input);
    tr = createTrainFromLine(st, input);
    tr.printTimes();

    return 0;
}

