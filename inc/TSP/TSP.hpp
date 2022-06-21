#ifndef _TSP_hpp
#define _TSP_hpp

namespace TSP {

class TSP_Solver {
    int *all_edges, all_edges_cnt;
    int *mst_node_rank;
    int *eulerian_circle;
    void solve_small_case();
    void MST();
    void odd_verts_minimum_weight_match();
    void get_eulerian_circle();
    void make_hamilton();
    void three_opt(int path[], int cnt, int max_iter, float term_cond, bool show_debug_info);

    inline float get_edge_dist(int edge) const { return dist[edge]; }

    float get_path_length(int path[], int cnt) const;

    float *dist;

 public:
    int N;
    TSP_Solver(int N) : N(N) {
        dist = new float[N * N];
        all_edges = new int[N * 3 / 2];
        mst_node_rank = new int[N];
        eulerian_circle = new int[N * 3 / 2];
        hamilton_path = new int[N];
    }
    ~TSP_Solver() {
        delete[] dist;
        delete[] all_edges;
        delete[] mst_node_rank;
        delete[] eulerian_circle;
        delete[] hamilton_path;
    }
    void set_dist(int i, int j, float d) { dist[j * N + i] = dist[i * N + j] = d; }
    void solve(int max_iter = -1, float term_cond = 1e-2f, bool show_debug_info = false);
    float length;
    int *hamilton_path;
};

}  // namespace TSP
#endif  // _TSP_hpp