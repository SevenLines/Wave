//
// Created by m on 13.06.17.
//

#include "test_wave.h"
#include <opencv2/opencv.hpp>
#include "core/waveprocessor.h"


void TestWave::testWave() {
    cv::Mat image = cv::imread("/home/m/QtProjects/Wave2/tests/sample3.png", CV_LOAD_IMAGE_COLOR);
    auto processor = WaveProcessor(image);
    processor.process();
}



