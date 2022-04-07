#include <algorithm>
#include <iostream>
using namespace std;

#include "TSP/Point.hpp"
#include "TSP/utils.hpp"
using namespace TSP;

constexpr int MAXN = 21;
Point pts[MAXN];
int N;
float dist[MAXN][MAXN], length;
int hamilton_path[MAXN];
float dp[1 << MAXN][MAXN];
int pre[1 << MAXN][MAXN];

void solve() {
    rep(S, 0, 1 << N) rep(u, 0, N) dp[S][u] = HUGE_VALF;
    dp[1][0] = 0;
    rep(S, 1, 1 << N) rep(u, 0, N) if (S >> u & 1) rep(v, 0, N) if (!(S >> v & 1)) {
        float cur = dp[S][u] + dist[u][v];
        int T = S | (1 << v);
        if (cur < dp[T][v]) dp[T][v] = cur, pre[T][v] = u;
    }
    int V = 1;
    rep(v, 2, N) if (dp[(1 << N) - 1][v] + dist[v][0] < dp[(1 << N) - 1][V] + dist[V][0]) V = v;
    length = dp[(1 << N) - 1][V] + dist[V][0];
    for (int i = N - 1, S = (1 << N) - 1, u = V; i >= 0; --i) V = pre[S][u], hamilton_path[i] = u, S -= 1 << u, u = V;
}

int main() {
    cin >> N;
    rep(i, 0, N) cin >> pts[i].x >> pts[i].y;
    rep(i, 0, N) {
        dist[i][i] = 0;
        rep(j, i + 1, N) dist[j][i] = dist[i][j] = pts[i].dist(pts[j]);
    }
    solve();
    cout << length << '\n';
    rep(i, 0, N) cout << hamilton_path[i] << ' ';
    return 0;
}