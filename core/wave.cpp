#include <math.h>
#include <time.h>
#include <algorithm>
#include "core/wave.h"

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

    /* оптимизируем */
//    points1.reserve(1000);
//    points2.reserve(1000);
//    points = &points1;
//    newList = &points2;
    /* используем указатели 
     * в результате за всю жизнь
     * волны существуе всего 
     * два массива под точки   
    */

//    gWaveID = 0;
}


SkeletizationOptions::SkeletizationOptions(float Intensety, float MaxDeviation, int Filter, int Memory,
                                           float MaxDirectionDeviation) {

}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

Point Point::getNeighboor(int i) {
    switch (i) {
        case 1:
            return Point(x - 1, y - 1);
        case 2:
            return Point(x, y - 1);
        case 3:
            return Point(x + 1, y - 1);
        case 4:
            return Point(x + 1, y);
        case 5:
            return Point(x + 1, y + 1);
        case 6:
            return Point(x, y + 1);
        case 7:
            return Point(x - 1, y + 1);
        case 8:
            return Point(x - 1, y);
        default:
            return *this;
    }
}

bool Point::isNeighboor(Point &point, int delta) {
    if (this->x == point.x && this->y == point.y)
        return false;
    return abs(this->x - point.x) <= delta && abs(this->y - point.y) <= delta;
}

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
        //wave->markCurrentPointsAsVisited((int) (i + 1));
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

bool Wave::next(vector<Wave *> &waves) {
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
            } else if (cell_value == WAVE_FILLED_CELL_VALUE) {
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

    // if the end of wave
//    if (!hasNext)
//        this->placeNode();
    if (this->stepNumber % 5 == 0) {
        this->placeNode();
    }

    this->markCurrentPointsAsCleared();

    if (this->metaWave->onWavePointsCleared) {
        this->metaWave->onWavePointsCleared(this);
    }

    this->currentPoints = currentPoints == &this->points1 ? &this->points2 : &this->points1;
    this->nextPoints = nextPoints == &this->points1 ? &this->points2 : &this->points1;
    this->gWaveInfo.waveStyle = this->gWaveInfo.waveStyle == WaveStyle::Diamond
                                ? WaveStyle::Square : WaveStyle::Diamond;

    return hasNext;
}

const vector<Point> &Wave::points() {
    return *this->currentPoints;
}

vector<Wave *> Wave::split() {
    vector<Wave *> waves;
    vector<Point> points = *this->currentPoints;
    vector<Point> newWavePoints;

    if (points.size() == 0) {
        return vector<Wave *>();
    }

    if (this->requireSplitCheck()) {
        vector<Wave *> vect;
        vect.push_back(this);
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
            waves.push_back(this);
            return waves;
        } else {
            Wave *wave = new Wave(this->matrix, newWavePoints, this->skeleton, this->metaWave);
            wave->lastNode = this->lastNode;
            waves.push_back(wave);
            newWavePoints.clear();
        }
    } while (points.size() > 0);
    return waves;
}

Node *Wave::placeNode() {
    auto centerPoint = this->getCenterPoint();
    auto node = this->skeleton->addNode(centerPoint);
    if (this->lastNode)
        this->lastNode->bind(node);
    this->lastNode = node;
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

void Wave::markCurrentPointsAsVisited() {
    for (auto point : *this->currentPoints) {
        matrix.at<int>(point) = this->id;
    }
}

void Wave::markCurrentPointsAsCleared() {
    for (auto point : *this->currentPoints) {
        matrix.at<int>(point) = 255;
    }
}

bool Wave::requireSplitCheck() {
    return this->edgedPixelsCount >= 4;
}

void Node::bind(Node *node) {
    this->nodes.push_back(node);
    node->nodes.push_back(this);
}


Node::Node(Point &_point, Skeleton *_skeleton) : point(_point.x, _point.y), skeleton(_skeleton) {
}

Node *Skeleton::addNode(Point &point) {
    auto node = new Node(point, this);
    nodes.push_back(node);
    return node;
}

Skeleton::~Skeleton() {
    for (auto node : nodes) {
        delete node;
    }
    nodes.clear();
}
