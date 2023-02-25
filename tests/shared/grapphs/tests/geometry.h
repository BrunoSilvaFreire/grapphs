
#ifndef GRAPPHS_GEOMETRY_H
#define GRAPPHS_GEOMETRY_H
#include <cmath>

inline float distance(float x0, float y0, float x1, float y1) {
    float diffX = (x1 - x0);
    float diffY = (y1 - y0);
    return std::sqrt(diffX * diffX + diffY * diffY);
}

#endif
