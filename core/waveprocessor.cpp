//
// Created by m on 10.06.17.
//

#include <functional>
#include "waveprocessor.h"

WaveProcessor::WaveProcessor(cv::Mat image) {
    this->image = image;
}


Skeleton* WaveProcessor::process() {
    cv::Mat grey;
    cv::cvtColor(image, grey, CV_RGB2GRAY);

    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.data;
    for( int i = 0; i < 256; ++i)
        p[i] = (uchar) (i == 255 ? 0 : 1);
    cv::LUT(grey, lookUpTable, grey);
    grey.convertTo(grey, CV_32S);

    Point startPosition(-1, -1);

    bool found = false;
    vector<Point> firstWavePoints;

    for (int i = 0; i < grey.rows; ++i) {
        for (int j = 0; j < grey.cols; ++j) {
            int cell_value = grey.at<int>(i, j);
            if (cell_value == WAVE_FILLED_CELL_VALUE) {
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

    if (found) {
        Skeleton *skeleton = new Skeleton();
        MetaWave metaWave(grey, firstWavePoints, skeleton);
        metaWave.onWaveNext = this->onWaveNext;
        metaWave.onWavePointsCleared = this->onWavePointsCleared;
        while (metaWave.next()) {}
        return skeleton;
    }
}