//Alberto Castagnaro 1219641
#ifndef SuperAVTrain_h
#define SuperAVTrain_h
#include "Train.h"

class SuperAVTrain : public Train {
public:
    static constexpr int MAX_SPEED = 300;
public:
    SuperAVTrain(int id_, bool left_,const std::vector<Station*>& stations_,const std::vector<int>& timetable_);
    SuperAVTrain(const SuperAVTrain&)= delete;
    SuperAVTrain& operator=(const SuperAVTrain&) =delete;
    SuperAVTrain(SuperAVTrain&&)=delete;
    SuperAVTrain& operator=(SuperAVTrain&&)=delete;
    ~SuperAVTrain() override ;
    void checkTimetable() override;
    bool hasToStop() const override;
    bool isRegional() const override { return false; }
    bool isAV() const override { return false; }
    bool isSuperAV() const override { return true; }
};

#endif // SuperAVTrain_h