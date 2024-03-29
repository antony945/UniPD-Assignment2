//Alberto Castagnaro 1219641
#ifndef AVTrain_h
#define AVTrain_h
#include "Train.h"

class AVTrain : public Train {
public:
    static constexpr int MAX_SPEED = 240;
public:
    AVTrain(int id_, bool left_,const std::vector<Station*>& stations_, const std::vector<int>& timetable_);
    AVTrain(const AVTrain&)= delete;
    AVTrain& operator=(const AVTrain&) =delete;
    AVTrain(AVTrain&&)=delete;
    AVTrain& operator=(AVTrain&&)=delete;
    ~AVTrain() override ;
    bool checkTimetable() override;
    bool hasToStop() const override;
    std::string getType() const override { return "AV"; }
};

#endif // AVTrain_h