#include "TSP/TSP.hpp"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstring>
#include <limits>
#include <numeric>

#include "TSP/AdjacencyList.hpp"
#include "TSP/Tour.hpp"
#include "TSP/Unionfind.hpp"
#include "TSP/utils.hpp"

namespace TSP {

constexpr int small_case_N = 5;

// Bruteforce
void TSP_Solver::solve_small_case() {
    int cur[small_case_N];
    std::iota(cur, cur + N, 0), std::iota(hamilton_path, hamilton_path + N, 0);
    float min_len = get_path_length(cur, N);
    while (std::next_permutation(cur, cur + N))
        if (float len = get_path_length(cur, N); len < min_len) min_len = len, std::copy(cur, cur + N, hamilton_path);
}

// Kruskal algorithm to find the minimum spanning tree
void TSP_Solver::MST() {
    Unionfind<MAXN> uf;
    std::memset(mst_node_rank, 0, sizeof(mst_node_rank[0]) * N);
    uf.init(N);
    int mst_edges_count = 0;
    int edges[MAXN * (MAXN - 1) / 2];
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

// use greedy method with O(V^2 log V^2) time complexity to find the approximate minimum weight matching
void TSP_Solver::odd_verts_minimum_weight_match() {
    int odd_verts[MAXN];
    int *odd_vert_edges = all_edges + (N - 1);
    int odd_vert_edges_cnt = 0, odd_verts_cnt = 0;
    rep(i, 0, N) if (mst_node_rank[i] & 1) odd_verts[odd_verts_cnt++] = i;
    int odd_verts_all_edges[MAXN * (MAXN - 1) / 2], odd_vert_all_edges_cnt = 0;
    rep(u, 1, odd_verts_cnt) rep(v, 0, u) odd_verts_all_edges[odd_vert_all_edges_cnt++] = odd_verts[u] * MAXN + odd_verts[v];
    std::sort(odd_verts_all_edges, odd_verts_all_edges + odd_vert_all_edges_cnt,
              [this](int x, int y) { return get_edge_dist(x) < get_edge_dist(y); });
    std::bitset<MAXN> vi;
    for (int i = 0; i < odd_vert_all_edges_cnt && (odd_vert_edges_cnt << 1) < odd_verts_cnt; ++i) {
        int edge = odd_verts_all_edges[i];
        int u = edge / MAXN, v = edge % MAXN;
        if (!(vi.test(u) || vi.test(v))) vi.set(u), vi.set(v), odd_vert_edges[odd_vert_edges_cnt++] = edge;
    }
    all_edges_cnt = odd_vert_edges_cnt + N - 1;
}

// Hierholzer's algorithm to find the Eulerian circle of the undirected graph
void TSP_Solver::get_eulerian_circle() {
    AdjacencyList graph;
    for (int i = 0, cnt = 0; i < all_edges_cnt; ++i) {
        int e = all_edges[i], u = e / MAXN, v = e % MAXN;
        graph.add_edge(u, v, cnt++);
        graph.add_edge(v, u, cnt++);
    }
    int Stack[MAXN * 3 / 2], sp = 1, res_i = 0, u = Stack[0] = 0;
    while (sp) {
        if (int e = graph.vex[u].first_edge; e != -1) {
            int v = graph.edge[e].to;
            Stack[sp++] = v;

            // delete u->v
            graph.vex[u].first_edge = graph.edge[e].next;
            if (graph.edge[e].next != -1) graph.edge[graph.edge[e].next].prev = -1;

            // delete v->u
            e ^= 1;  // v->u
            (graph.vex[v].first_edge == e ? graph.vex[v].first_edge : graph.edge[graph.edge[e].prev].next) = graph.edge[e].next;
            if (graph.edge[e].next != -1) graph.edge[graph.edge[e].next].prev = graph.edge[e].prev;

            u = v;
        } else {
            u = Stack[--sp];
            eulerian_circle[res_i++] = u;
        }
    }
}

// traverse the graph and push the first occurrence of each vertex into the tour path
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

// get the length of the tour path
float TSP_Solver::get_path_length(int path[], int cnt) const {
    float res = 0;
    rep(i, 1, cnt) res += dist[path[i - 1]][path[i]];
    res += dist[path[cnt - 1]][path[0]];
    return res;
}

static inline float three_opt_iter(const float dist[MAXN][MAXN], Tour &tour, int N) {
    float res = 0;
    auto a = tour.at(0);
    for (int i = 0; i < N; ++i, ++a) {
        for (auto e = std::next(a, 4); e.next() != a.cur; ++e) {
            for (auto c = std::next(a, 2); c.next() != e.cur; ++c) {
                int A = *a, B = a.next(), C = *c, D = c.next(), E = *e, F = e.next();
                float d0 = dist[A][B] + dist[C][D] + dist[E][F];
                float dxs[4]{dist[A][C] + dist[B][D] + dist[E][F], dist[A][B] + dist[C][E] + dist[D][F],
                             dist[A][D] + dist[E][B] + dist[C][F], dist[F][B] + dist[C][D] + dist[E][A]};
                int x = int(std::min_element(dxs, dxs + 4) - dxs);
                if (dxs[x] < d0) {
                    res += d0 - dxs[x];
                    switch (x) {
                    case 0: tour.reverse(a, c); break;
                    case 1: tour.reverse(c, e); break;
                    case 3: tour.reverse(a, e); break;
                    case 2: tour.rotate(a, c, e); break;
                    }
                    break;
                }
            }
        }
    }
    return res;
}

// 3-opt heuristic algorighm to improve the tour path
// https://en.wikipedia.org/wiki/3-opt
void TSP_Solver::three_opt(int path[], int cnt, int max_iter, float term_cond) {
    // if (cnt <= small_case_N) return;  // won't happen because TSP_Solver::solve() already checks this
    if (max_iter == 0) return;
    Tour tour(path, cnt);
    for (int iter = 0; max_iter == -1 || iter < max_iter; ++iter) {
        float delta = three_opt_iter(dist, tour, cnt);
        if (delta <= length * term_cond) break;
        length -= delta;
    }
    auto it = tour.at(0);
    for (int i = 0; i < cnt; ++i, ++it) path[i] = *it;
}

void TSP_Solver::solve(int max_iter, float term_cond) {
    if (N <= small_case_N) return solve_small_case();
    MST();
    odd_verts_minimum_weight_match();
    get_eulerian_circle();
    make_hamilton();
    length = get_path_length(hamilton_path, N);
    three_opt(hamilton_path, N, max_iter, term_cond);

    // make the first vertex of the path be 0
    std::rotate(hamilton_path, std::find(hamilton_path, hamilton_path + N, 0), hamilton_path + N);
}

void TSP_Solver::solve_without_returning(int max_iter, float term_cond) {
    dist[0][N] = dist[N][0] = dist[N][N] = 0;
    for (int u = 1; u < N; ++u) dist[u][N] = dist[N][u] = std::numeric_limits<float>::infinity();

    int res[MAXN - 1];
    float res_len = std::numeric_limits<float>::infinity();
    for (int u = 1; u < N; ++u) {
        dist[u][N] = dist[N][u] = 0;
        ++N, solve(max_iter, term_cond), --N;
        if (length < res_len) {
            if (hamilton_path[1] == N) std::reverse(hamilton_path + 1, hamilton_path + (N + 1));
            std::copy(hamilton_path, hamilton_path + N, res);
            res_len = length;
        }
        dist[u][N] = dist[N][u] = std::numeric_limits<float>::infinity();
    }

    length = res_len;
    std::copy(res, res + N, hamilton_path);
}

}  // namespace TSP
