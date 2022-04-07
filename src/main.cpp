#include <iostream>
using namespace std;

#include "TSP/Point.hpp"
#include "TSP/TSP.hpp"
#include "TSP/utils.hpp"
using namespace TSP;

TSP_Solver tsp;

Point pts[MAXN];

int main() {
    cin >> tsp.N;
    rep(i, 0, tsp.N) cin >> pts[i].x >> pts[i].y;
    rep(i, 0, tsp.N) {
        tsp.dist[i][i] = 0;
        rep(j, i + 1, tsp.N) tsp.dist[j][i] = tsp.dist[i][j] = pts[i].dist(pts[j]);
    }
    tsp.solve();
    cout << tsp.length << '\n';
    rep(i, 0, tsp.N) cout << tsp.hamilton_path[i] << ' ';
    return 0;
}