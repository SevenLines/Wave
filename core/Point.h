//
// Created by m on 18.06.17.
//

#ifndef TESTPROJECT_POINT_H
#define TESTPROJECT_POINT_H

#include <opencv2/opencv.hpp>

class Point : public cv::Point {
public:
    Point(int x, int y);

    Point getNeighboor(int i);

    bool isNeighboor(Point &point, int delta=1);
};


#endif //TESTPROJECT_POINT_H
