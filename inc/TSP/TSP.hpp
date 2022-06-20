#ifndef _TSP_hpp
#define _TSP_hpp
#include "TSP/config.hpp"
namespace TSP {

class TSP_Solver {
    int all_edges[MAXN * 3 / 2], all_edges_cnt;
    int mst_node_rank[MAXN];
    int eulerian_circle[MAXN * 3 / 2];
    void solve_small_case();
    void MST();
    void odd_verts_minimum_weight_match();
    void get_eulerian_circle();
    void make_hamilton();
    void make_shorter();

    inline float get_edge_dist(int edge) const { return dist[edge / MAXN][edge % MAXN]; }

    float get_path_length(int path[], int cnt) const;

    void three_opt(int path[], int cnt);

 public:
    int N;
    float dist[MAXN][MAXN];
    void solve();
    void solve_without_returning();
    float length;
    int hamilton_path[MAXN];
};

}  // namespace TSP
#endif  // _TSP_hpp