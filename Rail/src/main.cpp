#include <iostream>
#include "Railway.h"
using namespace std;

// File di test
int main() {
    Railway myRailway{"line_description.txt", "timetables.txt", "output.txt"};
    // myRailway.daySimulation();
    myRailway.tester();

    return 0;
}