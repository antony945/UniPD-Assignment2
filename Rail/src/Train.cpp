#include "Train.h"
#include "MainStation.h"
#include "LocalStation.h"
#include <algorithm>
#include <cmath>

Train::Train(int id_, bool fromOrigin_, const std::vector<Station*>& stations_, const std::vector<int>& timetable_)
  : id{id_},
    fromOrigin{fromOrigin_},
    stations{stations_},
    timetable{timetable_},
    currentDelay{0},
    currentSpeed{0},
    currentDistance{0},
    nextStationIndex{0} {}