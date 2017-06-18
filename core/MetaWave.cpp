//
// Created by m on 18.06.17.
//

#include "MetaWave.h"


bool MetaWave::next() {
    bool hasNext = false;
    ++this->step;
    // made another wave iteration
    for (int i = 0; i < this->_waves.size(); ++i) {
        auto &wave = this->_waves[i];
        hasNext |= wave->next(this->_waves);
        if (this->onWaveNext) {
            this->onWaveNext(wave);
        }
    }

    // join waves

    // split newly create waves
    vector<Wave *> newWaveList;
    for (auto wave : this->_waves) {
        auto waves = wave->split();
        newWaveList.insert(newWaveList.end(), waves.begin(), waves.end());
    }

    // remove all unused waves
    for (auto wave : this->_waves) {
        if (find(newWaveList.begin(), newWaveList.end(), wave) == newWaveList.end()) {
            delete wave;
        }
    }
    this->_waves.clear();
    this->_waves = newWaveList;

    for (int i = 0; i < this->_waves.size(); ++i) {
        auto &wave = this->_waves[i];
        wave->markCurrentPointsAsCleared();
    }

    return hasNext;
}

MetaWave::MetaWave(cv::Mat image, vector<Point> points, Skeleton *skeleton) {
    image.copyTo(this->image);
    Wave *wave = new Wave(this->image, points, skeleton, this);
    wave->placeNode();
    wave->markCurrentPointsAsCleared();
    this->_waves.push_back(wave);
}

const vector<Wave *> &MetaWave::waves() {
    return this->_waves;
}