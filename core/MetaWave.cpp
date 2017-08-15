//
// Created by m on 18.06.17.
//

#include "MetaWave.h"
using namespace std;

bool MetaWave::next() {
    bool hasNext = false;
    ++this->step;
    // made another wave iteration
    for (int i = 0; i < this->_waves.size(); ++i) {
        auto &wave = this->_waves[i];
        hasNext |= wave->next(this->_waves);
        if (this->onWaveNext) {
            this->onWaveNext(wave.get());
        }
    }

    // join waves

    // split newly create waves
    vector<shared_ptr<Wave>> newWaveList;
    for (auto wave : this->_waves) {
        auto waves = wave->split();
        newWaveList.insert(newWaveList.end(), waves.begin(), waves.end());
    }

    this->_waves = newWaveList;

    for (int i = 0; i < this->_waves.size(); ++i) {
        auto &wave = this->_waves[i];
        wave->markCurrentPointsAsCleared();
    }

    return hasNext;
}

MetaWave::MetaWave(cv::Mat image, vector<Point> points, Skeleton *skeleton) {
    image.copyTo(this->image);
    auto wave = make_unique<Wave>(this->image, points, skeleton, this);
    wave->placeNode();
    wave->markCurrentPointsAsCleared();
    this->_waves.push_back(std::move(wave));
}

