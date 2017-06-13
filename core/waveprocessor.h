//
// Created by m on 10.06.17.
//

#ifndef TESTPROJECT_WAVEPROCESSOR_H
#define TESTPROJECT_WAVEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include "core/wave.h"
#include <functional>

class WaveProcessor {
public:
    WaveProcessor(cv::Mat image);

    void process(function<void(MetaWave)>onMetaWaveNext= nullptr);

    cv::Mat image;
};

#endif //TESTPROJECT_WAVEPROCESSOR_H
