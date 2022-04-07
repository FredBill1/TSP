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
int nxt[1 << MAXN][MAXN];

void solve() {
    rep(S, 0, 1 << N) rep(u, 0, N) dp[S][u] = HUGE_VALF;
    dp[(1 << N) - 1][0] = 0;
    qer(S, (1 << N) - 2, 0) rep(u, 0, N) rep(v, 0, N) if (!(S >> v & 1)) {
        float cur = dp[S | 1 << v][v] + dist[u][v];
        if (cur < dp[S][u]) {
            dp[S][u] = cur;
            nxt[S][u] = v;
        }
    }
    length = dp[0][0];
    for (int i = 0, S = 0, u = 0; i < N; ++i) {
        hamilton_path[i] = u;
        S |= 1 << (u = nxt[S][u]);
    }
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