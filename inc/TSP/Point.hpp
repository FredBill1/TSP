#ifndef _tsp_Point_hpp
#define _tsp_Point_hpp

#include <cmath>

namespace TSP {
struct Point {
    float x, y;
    Point(float _x = 0., float _y = 0.) : x(_x), y(_y) {}
    float dist(const Point &p) const {
        float dx = x - p.x, dy = y - p.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};
}  // namespace TSP
#endif  // _tsp_Point_hpp