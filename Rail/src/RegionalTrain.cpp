#include "RegionalTrain.h"
#include "MainStation.h"
#include "LocalStation.h"

RegionalTrain::RegionalTrain(int id_, bool left_, const std::vector<Station*>& stations_, const std::vector<int>& timetable_) : Train(id_, left_, stations_, timetable_) {}

bool RegionalTrain::sendStopRequest(Station* s) {
    // Treno è regionale: si ferma in tutte le stazioni
    // Invia richiesta di fermata alla stazione
    return true;
}

void RegionalTrain::trainInformation(std::ostream& os) const {
    int i=0;
    os << "TRENO " << id << " - " << getType() << '\n';
    for(Station* s : stations) {
        os << s->getName() << ' ' << s->getDistance() << ": ";
        os << timetable[i++] << '\n';
    }

    os << '\n';
}