#include "My_func.h"
#include "Train.h"
#include "Station.h"
using namespace std;

void f1(Train& t1) {
    cout << t1.get_maxSpeed() << '\n';
}

int main() {
    RegionalTrain t1{1, 0, {}};
    HighSpeedTrain t2{1, 0, {}};
    SuperHighSpeedTrain t3{1, 0, {}};

    f1(t1);
    f1(t2);
    f1(t3);

    return 0;
}