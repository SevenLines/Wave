//
// Created by m on 10.06.17.
//

#include <functional>
#include "waveprocessor.h"

WaveProcessor::WaveProcessor(cv::Mat image) {
    this->image = image;
}


Skeleton* WaveProcessor::process(function<void(MetaWave)>onMetaWaveNext) {
    cv::Mat grey;
    cv::cvtColor(image, grey, CV_RGB2GRAY);
    CV_Assert(grey.depth() == CV_8U);
    Point startPosition(-1, -1);

    bool found = false;
    vector<Point> firstWavePoints;

    for (int i = 0; i < grey.rows; ++i) {
        for (int j = 0; j < grey.cols; ++j) {
            uchar color = grey.at<uchar>(i, j);
            if (color == 0) {
                std::cout << "found";
                startPosition.x = j;
                startPosition.y = i;
                found = true;
                firstWavePoints.push_back(startPosition);
                break;
            }
        }
        if (found) {
            break;
        }
    }

    Skeleton *skeleton = new Skeleton();
    MetaWave metaWave(grey, firstWavePoints, skeleton);
    while (metaWave.next()) {}
    return skeleton;
}