#include <cmath>
#include <iostream>
using namespace std;

#include "TSP/TSP.hpp"
#include "TSP/utils.hpp"
using namespace TSP;

TSP_Solver tsp;

struct Point {
    float x, y;
    Point(float _x = 0., float _y = 0.) : x(_x), y(_y) {}
    float dist(const Point &p) const {
        float dx = x - p.x, dy = y - p.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

Point pts[MAXN];

int main() {
    cin >> tsp.N;
    rep(i, 0, tsp.N) cin >> pts[i].x >> pts[i].y;
    rep(i, 0, tsp.N) {
        tsp.dist[i][i] = 0;
        rep(j, i + 1, tsp.N) tsp.dist[j][i] = tsp.dist[i][j] = pts[i].dist(pts[j]);
    }
    tsp.solve();
    rep(i, 0, tsp.N) cout << tsp.hamilton_path[i] << ' ';
    return 0;
}