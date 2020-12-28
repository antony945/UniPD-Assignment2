#include "Train.h"

// PURE BASE CLASS - Train
Train::Train(int id_, int startedFrom_, std::vector<int> timetable_) : id{id_}, startedFrom{startedFrom_}, timetable{timetable_} {}

// DERIVED CLASS - RegionalTrain
RegionalTrain::RegionalTrain(int id_, int startedFrom_, std::vector<int> timetable_) : Train{id_, startedFrom_, timetable_} {}

// DERIVED CLASS - HighSpeedTrain
HighSpeedTrain::HighSpeedTrain(int id_, int startedFrom_, std::vector<int> timetable_) : Train{id_, startedFrom_, timetable_} {}

// DERIVED CLASS - SuperHighSpeedTrain
SuperHighSpeedTrain::SuperHighSpeedTrain(int id_, int startedFrom_, std::vector<int> timetable_) : Train{id_, startedFrom_, timetable_} {}