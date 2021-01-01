#include "SuperAVTrain.h"
#include "MainStation.h"
#include "LocalStation.h"

SuperAVTrain::SuperAVTrain(int id_, bool left_, const std::vector<Station*>& stations_, const std::vector<int>& timetable_) : Train(id_, left_, stations_, timetable_) {}

bool SuperAVTrain::sendStopRequest(Station* s) {
    // Treno è alta velocità super: si ferma solo in stazioni principali
    // Invia richiesta di fermata solo a stazioni principali
    return s->isMain();
}

void SuperAVTrain::trainInformation(std::ostream& os) const {
    int i=0;
    os << "TRENO " << id << " - " << getType() << '\n';
    for(Station* s : stations) {
        os << s->getName() << ' ' << s->getDistance() << ": ";
        if(s->isMain())
            os << timetable[i++] << '\n';
        else
            os << "NO STOP\n";
    }

    os << '\n';
}