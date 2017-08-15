//
// Created by m on 10.06.17.
//

#ifndef TESTPROJECT_WAVEPROCESSOR_H
#define TESTPROJECT_WAVEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include "core/Wave.h"
#include <functional>

using namespace std;

class WaveProcessor {
public:
    WaveProcessor(cv::Mat image);

    Skeleton *process();

    function<void(Wave*)> onWaveNext = nullptr;
    function<void(Wave*)> onWavePointsCleared = nullptr;

    cv::Mat image;
};

#endif //TESTPROJECT_WAVEPROCESSOR_H
