//
// Created by m on 13.06.17.
//

#include "test_wave.h"
#include <opencv2/opencv.hpp>
#include "core/waveprocessor.h"


void TestWave::testWave() {
    cv::Mat image = cv::imread("/home/m/QtProjects/Wave2/tests/profile_test.png", CV_LOAD_IMAGE_COLOR);
    auto processor = WaveProcessor(image);
    cv::Vec3b colors[] = {
            cv::Vec3b(235,192,91),
            cv::Vec3b(76,231,253),
            cv::Vec3b(61,197,155),
            cv::Vec3b(33,121,250),
    };
    processor.onWavePointsCleared = [&](Wave *wave) {
       for(auto point : wave->points()) {
            image.at<cv::Vec3b>(point) = colors[(wave->id - 1) % 4];
        }
    };

    auto skeleton = processor.process();

    for (auto node : skeleton->nodes) {
        for (auto another_node : node->nodes) {
            cv::line(image, node->point, another_node->point, cv::Scalar(0, 0, 0));
        }
    }

    for (auto node : skeleton->nodes) {
        cv::circle(image, node->point, 3, cv::Scalar(128,186,229), CV_FILLED);
        cv::circle(image, node->point, 3, cv::Scalar(52,89,229));
    }
    cv::imwrite("/home/m/QtProjects/Wave2/tests/out.png", image);
}



