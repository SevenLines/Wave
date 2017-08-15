#include <math.h>
#include <time.h>
#include <algorithm>
#include "core/Wave.h"
#include "MetaWave.h"

unsigned int Wave::WaveCounter = 2;

Wave::Wave(cv::Mat matrix, vector<Point> points, Skeleton *skeleton, MetaWave *metaWave) {
    this->id = ++Wave::WaveCounter;
    this->matrix = matrix;
    this->skeleton = skeleton;
    this->metaWave = metaWave;

    this->points1 = points;

    this->currentPoints = &(this->points1);
    this->nextPoints = &(this->points2);

    gWaveInfo.waveStyle = WaveStyle::Square;
    gWaveInfo.size = 0;
    gWaveInfo.step = 0;
    gWaveInfo.lastDirection = -1;

    gOptions.Filter = 1;
    gOptions.MaxDeviation = 0.5;
    gOptions.Memory = 10;
    gOptions.SetIntencity = 0.5;
}


SkeletizationOptions::SkeletizationOptions(float Intensety, float MaxDeviation, int Filter, int Memory,
                                           float MaxDirectionDeviation) {

}

bool Wave::next(vector<shared_ptr<Wave>> &waves) {
    ++this->stepNumber;
    this->edgedPixelsCount = 0;

    if (this->currentPoints->size() <= 0) {
        return false;
    }

    bool hasNext = false;

    this->nextPoints->clear();

    for (unsigned long j = 0; j < this->currentPoints->size(); ++j) {
        auto point = this->currentPoints->at(j);
        for (int i = 1; i <= 8; ++i) {
            if (this->gWaveInfo.waveStyle == WaveStyle::Diamond && i % 2 == 1)
                continue;

            auto neighboorPoint = point.getNeighboor(i);
            if (neighboorPoint.x < 0
                || neighboorPoint.y < 0
                || neighboorPoint.x >= this->matrix.cols
                || neighboorPoint.y >= this->matrix.rows)
                continue;
            auto cell_value = this->matrix.at<int>(neighboorPoint);

            if (cell_value == WAVE_EMPTY_CELL_VALUE) {
                ++this->edgedPixelsCount;
                continue;
            }

            if (cell_value == WAVE_FILLED_CELL_VALUE) {
                this->matrix.at<int>(neighboorPoint) = this->id;
                this->nextPoints->push_back(neighboorPoint);
                hasNext = true;
            } else if (cell_value != this->id) {
                // waves collison, add another wave points to current wave
                for (auto another_wave : waves) {
                    if (another_wave->id == cell_value) {
                        if (another_wave->currentPoints->size() > 0) {

                            if (this->lastNode && another_wave->lastNode)
                                this->lastNode->bind(another_wave->lastNode);

                            this->currentPoints->insert(this->currentPoints->end(),
                                                        another_wave->currentPoints->begin(),
                                                        another_wave->currentPoints->begin());
                            for (auto pnt : *another_wave->currentPoints) {
                                this->matrix.at<int>(pnt) = this->id;
                            }
                            another_wave->currentPoints->clear();
                        }
                    }
                }
                hasNext = true;
            }
        }
    }

    auto actual_size = this->gWaveInfo.waveStyle == WaveStyle::Diamond ?
                       this->currentPoints->size() / 2 : this->currentPoints->size();
    if (this->stepsSinceLastPlacement >= actual_size / 3) {
        auto avg_size = accumulate(this->lastWaveSizes.begin(), this->lastWaveSizes.end(), 0.0) / this->lastWaveSizes.size();
        // if wave changes to quickly dont place points
        if (fabs(1.0 - (avg_size / actual_size)) < 0.3) {
            this->placeNode();
        }
    } else {
        ++this->stepsSinceLastPlacement;
    }

    this->markCurrentPointsAsCleared();

    if (this->metaWave->onWavePointsCleared) {
        this->metaWave->onWavePointsCleared(this);
    }

    this->lastWaveSizes.push_back(actual_size);

    this->currentPoints = currentPoints == &this->points1 ? &this->points2 : &this->points1;
    this->nextPoints = nextPoints == &this->points1 ? &this->points2 : &this->points1;
    this->gWaveInfo.waveStyle = this->gWaveInfo.waveStyle == WaveStyle::Diamond
                                ? WaveStyle::Square : WaveStyle::Diamond;



    if (this->lastWaveSizes.size() > 10) {
        this->lastWaveSizes.pop_front();
    }

    return hasNext;
}

const vector<Point> &Wave::points() {
    return *this->currentPoints;
}

vector<shared_ptr<Wave>> Wave::split() {
    vector<shared_ptr<Wave>> waves;
    vector<Point> points = *this->currentPoints;
    vector<Point> newWavePoints;

    if (points.size() == 0) {
        return vector<shared_ptr<Wave>>();
    }

    if (this->requireSplitCheck()) {
        vector<shared_ptr<Wave>> vect;
        vect.push_back(this->shared_from_this());
        return vect;
    }

    do {
        newWavePoints.push_back(points[0]);
        points.erase(points.begin());

        for (int j = 0; j < newWavePoints.size(); ++j) {
            auto point1 = newWavePoints[j];
            for (int i = 0; i < points.size(); ++i) {
                auto point2 = points[i];
                if (point1.isNeighboor(point2, 2)) {
                    newWavePoints.push_back(point2);
                    points.erase(points.begin() + i);
                    --i;
                }
            }
        }

        if (this->currentPoints->size() == newWavePoints.size()) {
            waves.push_back(this->shared_from_this());
            return waves;
        } else {
            auto wave = make_shared<Wave>(this->matrix, newWavePoints, this->skeleton, this->metaWave);
            wave->lastNode = this->lastNode;
            waves.push_back(wave);
            newWavePoints.clear();
        }
    } while (points.size() > 0);
    return waves;
}

shared_ptr<Node> Wave::placeNode() {
    auto centerPoint = this->getCenterPoint();
    auto node = this->skeleton->addNode(centerPoint);
    if (this->lastNode)
        this->lastNode->bind(node);
    this->lastNode = node;
    this->stepsSinceLastPlacement = 0;
    return node;
}

Point Wave::getCenterPoint() {
    Point sumPoint(0, 0);
    for (auto point : *this->currentPoints) {
        sumPoint += point;
    }
    sumPoint.x /= this->currentPoints->size();
    sumPoint.y /= this->currentPoints->size();
    return sumPoint;
}

void Wave::markCurrentPointsAsCleared() {
    for (auto point : *this->currentPoints) {
        matrix.at<int>(point) = 255;
    }
}

bool Wave::requireSplitCheck() {
    return this->edgedPixelsCount >= 4;
}

