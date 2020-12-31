#include "Train.h"

Train::Train(int id_, bool fromOrigin_, const std::vector<int>& timetable_)
    : id{id_},
      fromOrigin{fromOrigin_},
      timetable{timetable_},
      currentDelay{0},
      currentSpeed{0},
      currentDistance{0},
      nextStationIndex{0} {}