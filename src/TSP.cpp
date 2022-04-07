#include "TSP/TSP.hpp"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstring>
#include <numeric>

#include "TSP/Unionfind.hpp"
#include "TSP/utils.hpp"

namespace TSP {

// Kruskal
void TSP_Solver::MST() {
    static Unionfind<MAXN> uf;
    std::memset(mst_node_rank, 0, sizeof(mst_node_rank[0]) * N);
    uf.init(N);
    int mst_edges_count = 0;
    static int edges[MAXN * (MAXN - 1) / 2];
    int edges_count = 0;
    rep(u, 0, N) rep(v, u + 1, N) edges[edges_count++] = u * MAXN + v;
    std::sort(edges, edges + edges_count, [this](int x, int y) { return get_edge_dist(x) < get_edge_dist(y); });
    rep(i, 0, edges_count) {
        int edge = edges[i];
        int u = edge / MAXN, v = edge % MAXN;
        if (uf.merge(u, v)) {
            all_edges[mst_edges_count++] = edge;
            ++mst_node_rank[u], ++mst_node_rank[v];
            if (mst_edges_count == N - 1) break;
        }
    }
}

void TSP_Solver::odd_verts_minimum_weight_match() {
    int odd_verts[MAXN];
    int *odd_vert_edges = all_edges + (N - 1);
    int odd_vert_edges_cnt = 0, odd_verts_cnt = 0;
    rep(i, 0, N) if (mst_node_rank[i] & 1) odd_verts[odd_verts_cnt++] = i;
    for (int i = 0; i < odd_verts_cnt; i += 2) {
        int u = odd_verts[i], res = i + 1;
        rep(j, i + 2, odd_verts_cnt) if (dist[u][odd_verts[j]] < dist[u][odd_verts[res]]) res = j;
        if (res != i + 1) std::swap(odd_verts[i + 1], odd_verts[res]);
        odd_vert_edges[odd_vert_edges_cnt++] = u * MAXN + odd_verts[i + 1];
    }
    all_edges_cnt = odd_vert_edges_cnt + N - 1;
}

// Hierholzer's algorithm
void TSP_Solver::get_eulerian_circle() {
    int head[MAXN], to[MAXN * 3], nxt[MAXN * 3];
    memset(head, 0xff, sizeof(head[0]) * N);
    for (int i = 0, cnt = 0; i < all_edges_cnt; ++i) {
        int edge = all_edges[i], u = edge / MAXN, v = edge % MAXN;
        to[cnt] = v, nxt[cnt] = head[u], head[u] = cnt++;
        to[cnt] = u, nxt[cnt] = head[v], head[v] = cnt++;
    }
    int Stack[MAXN * 3 / 2];
    int sp = 1, res_i = 0, u = Stack[0] = 0;
    while (sp) {
        if (head[u] != -1) {
            int v = to[head[u]];
            Stack[sp++] = v;
            head[u] = nxt[head[u]];  // delete u->v

            // delete v->u
            if (to[head[v]] == u) head[v] = nxt[head[v]];
            else {
                int t = head[v];
                while (to[nxt[t]] != u) t = nxt[t];
                nxt[t] = nxt[nxt[t]];
            }

            u = v;
        } else {
            u = Stack[--sp];
            eulerian_circle[res_i++] = u;
        }
    }
}

void TSP_Solver::make_hamilton() {
    std::bitset<MAXN> bs;
    bs[0] = true;
    hamilton_path[0] = 0;
    for (int i = 0, p = 0; i < all_edges_cnt; ++i) {
        int cur = eulerian_circle[i];
        if (!bs[cur]) {
            bs[cur] = true;
            hamilton_path[++p] = cur;
        }
    }
}

static inline void rotate_1(int arr[], int cnt) {
    int tmp = arr[0];
    rep(i, 1, cnt) arr[i - 1] = arr[i];
    arr[cnt - 1] = tmp;
}

float TSP_Solver::get_path_length(int path[], int cnt) const {
    float res = 0;
    rep(i, 1, cnt) res += dist[path[i - 1]][path[i]];
    res += dist[path[cnt - 1]][path[0]];
    return res;
}

float TSP_Solver::three_opt_iter(int path[], int cnt) {
    float res = 0;
    rep(i, 0, cnt) {
        if (i) rotate_1(path, cnt);
        int &A = path[0], &B = path[1];
        rep(j, 2, cnt - 2) {
            int &C = path[j], &D = path[j + 1];
            rep(k, j + 2, cnt) {
                int E = path[k], F = path[k + 1 == cnt ? 0 : k + 1];
                float d0 = dist[A][B] + dist[C][D] + dist[E][F];
                float dxs[4]{dist[A][C] + dist[B][D] + dist[E][F], dist[A][B] + dist[C][E] + dist[D][F],
                             dist[A][D] + dist[E][B] + dist[C][F], dist[F][B] + dist[C][D] + dist[E][A]};
                int x = std::min_element(dxs, dxs + 4) - dxs;
                if (dxs[x] < d0) {
                    res += d0 - dxs[x];
                    switch (x) {
                    case 0: std::reverse(path + 1, path + (j + 1)); break;
                    case 1: std::reverse(path + (j + 1), path + (k + 1)); break;
                    case 3: std::reverse(path + 1, path + (k + 1)); break;
                    case 2: std::rotate(path + 1, path + (j + 1), path + (k + 1)); break;
                    }
                }
            }
        }
    }
    return res;
}

void TSP_Solver::three_opt(int path[], int cnt) {
    if (cnt < 5) return;
    for (;;) {
        float delta = three_opt_iter(path, cnt);
        if (delta <= length * term_cond) break;
        length -= delta;
    }
}

void TSP_Solver::make_shorter() {
    three_opt(hamilton_path, N);
    int nxt[MAXN];
    rep(i, 0, N - 1) nxt[hamilton_path[i]] = hamilton_path[i + 1];
    nxt[hamilton_path[N - 1]] = hamilton_path[0];
    for (int i = 0, u = 0; i < N; ++i) {
        hamilton_path[i] = u;
        u = nxt[u];
    }
}

void TSP_Solver::solve() {
    MST();
    odd_verts_minimum_weight_match();
    get_eulerian_circle();
    make_hamilton();
    length = get_path_length(hamilton_path, N);
    make_shorter();
}

}  // namespace TSP
