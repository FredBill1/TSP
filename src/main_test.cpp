#include <iostream>
using namespace std;

#include "TSP/Point.hpp"
#include "TSP/TSP.hpp"
#include "TSP/utils.hpp"
using namespace TSP;

int main() {
    int N;
    cin >> N;
    TSP_Solver tsp(N);

    Point* pts = new Point[N];
    rep(i, 0, N) cin >> pts[i].x >> pts[i].y;
    rep(i, 0, N) {
        tsp.set_dist(i, i, 0);
        rep(j, i + 1, N) tsp.set_dist(i, j, pts[i].dist(pts[j]));
    }
    delete[] pts;

    tsp.solve();
    cout << tsp.length << '\n';
    rep(i, 0, tsp.N) cout << tsp.hamilton_path[i] << ' ';
    return 0;
}