#ifndef WAVECLASS_H
#define WAVECLASS_H

#include <vector>
#include <list>
#include <opencv2/opencv.hpp>
#include <memory>
#include <functional>
#include "Point.h"
#include "Skeleton.h"

using namespace std;

enum class WaveStyle {
    Square, Diamond
};

const int WAVE_EMPTY_CELL_VALUE = 0;
const int WAVE_FILLED_CELL_VALUE = 1;

struct SkeletizationOptions {

    float SetIntencity;         // частота проставления точек, через сколько итераций в зависимости от дины волны
    float MaxDeviation;         // максимальна допустимая резкость смены длина волны, чтобы поставить точку
    float MaxDirectionDeviation;// отклонение направлениия волны
    unsigned int Filter;        // допустимое растояние между двумя точками чтобы считать волну целостной
    unsigned int Memory;        // кол-во запоминаемых состояний волны. 0 - динамическая память
    unsigned int Smoothing;     // сила сглаживания графа

    SkeletizationOptions(float Intensety = 0.5,
                         float MaxDeviation = 0.3,
                         int Filter = 3,
                         int Memory = 5,
                         float MaxDirectionDeviation = 15.0f / 180.0f * 3.14f);
};

struct WaveInfo {
    int size = 0; /* размер волны */
    int step = 0; /* текущий шаг волны*/
    int avgSize = 0; /* средняя длина за последние n итерций*/
    int avgSizeOverall = 0; /* средняя длина за последние n итерций*/
    float lastDirection;
    int speedupSplit = 0;
    int overallSplits = 0;
    float currentDirection;
    float lastTimeTakes;
    WaveStyle waveStyle;
    list<int> memory;
};

class MetaWave;

class Wave : public enable_shared_from_this<Wave> /*волна*/
{
private:
    static unsigned int WaveCounter;

    int stepNumber = 0;
    bool edgedPixelsCount = 0;
    WaveInfo gWaveInfo;
    SkeletizationOptions gOptions;

    cv::Mat matrix;
    vector<Point> points1;
    vector<Point> points2;
    vector<Point> *currentPoints;
    vector<Point> *nextPoints;
    vector<int> waveIdsToAdd;
    MetaWave *metaWave;
    shared_ptr<Node> lastNode = nullptr;

    Skeleton *skeleton = nullptr;
public:

    int id;

    Wave(cv::Mat matrix, vector<Point> points, Skeleton *skeleton, MetaWave *metaWave);

    const vector<Point> &points();

    bool next(vector<shared_ptr<Wave>> &waves);
    bool requireSplitCheck();

    shared_ptr<Node> placeNode();
    Point getCenterPoint();

    vector<shared_ptr<Wave>> split();

    void markCurrentPointsAsCleared();

};


#endif // WAVECLASS_H
