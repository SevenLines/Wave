//
// Created by m on 18.06.17.
//

#ifndef TESTPROJECT_METAWAVE_H
#define TESTPROJECT_METAWAVE_H


#include "Wave.h"
#include <vector>

class MetaWave {
private:
    int step = 0;
    cv::Mat image;
    vector<shared_ptr<Wave>> _waves;
    Skeleton* skeleton;
public:
    MetaWave(cv::Mat image, vector<Point> points, Skeleton* skeleton);
    std::function<void(Wave*)> onWaveNext = nullptr;
    std::function<void(Wave*)> onWavePointsCleared = nullptr;

    bool next();
};



#endif //TESTPROJECT_METAWAVE_H
