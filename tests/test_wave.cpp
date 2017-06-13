//
// Created by m on 13.06.17.
//

#include "test_wave.h"
#include <opencv2/opencv.hpp>
#include "core/waveprocessor.h"


void TestWave::testWave() {
    cv::Mat image = cv::imread("/home/m/QtProjects/Wave2/tests/sample3.png", CV_LOAD_IMAGE_COLOR);
    auto processor = WaveProcessor(image);
    auto skeleton = processor.process();

    for (auto node : skeleton->nodes) {
        for (auto another_node : node->nodes) {
            cv::line(image, node->point, another_node->point, cv::Scalar(255, 255, 0));
        }
    }

    for (auto node : skeleton->nodes) {
        cv::circle(image, node->point, 3, cv::Scalar(255, 255, 0));
        cv::circle(image, node->point, 3, cv::Scalar(255, 255, 200), CV_FILLED);
    }
    cv::imwrite("/home/m/QtProjects/Wave2/tests/out.png", image);
}



