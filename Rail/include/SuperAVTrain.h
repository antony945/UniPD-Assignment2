//Alberto Castagnaro 1219641
#ifndef SuperAVTrain_h
#define SuperAVTrain_h
#include "Train.h"

class SuperAVTrain : public Train {
public:
    ///max speed that train can reach
    static constexpr int MAX_SPEED = 300;
public:
    SuperAVTrain(int id_, bool left_,const std::vector<Station*>& stations_,const std::vector<int>& timetable_);
    SuperAVTrain(const SuperAVTrain&)= delete;
    SuperAVTrain& operator=(const SuperAVTrain&) =delete;
    SuperAVTrain(SuperAVTrain&&)=delete;
    SuperAVTrain& operator=(SuperAVTrain&&)=delete;
    ~SuperAVTrain();
    bool checkTimetable() override;
    bool hasToStop() const override;
    std::string getType() const override { return "SUPER AV"; }
};

#endif // SuperAVTrain_h