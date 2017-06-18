//
// Created by m on 18.06.17.
//

#include "Point.h"

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