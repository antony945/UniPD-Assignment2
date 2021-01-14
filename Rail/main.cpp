#include <iostream>
#include "Railway.h"
using namespace std;

// File di test
int main() {
    // Costruisci ferrovia
    Railway myRailway{"../line_description.txt", "../timetables.txt", "../output.txt"};
    // Stampa informazioni su stazioni e treni
    myRailway.printInfo();
    // Avvia simulazione
    myRailway.daySimulation();
    // Termina
    return 0;
}
