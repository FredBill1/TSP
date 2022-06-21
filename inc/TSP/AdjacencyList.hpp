#ifndef _TSP_AdjacencyList_hpp
#define _TSP_AdjacencyList_hpp

#include "TSP/config.hpp"

namespace TSP {

struct AdjacencyList {
    struct Vex {
        int first_edge = -1;  // the index of the first edge in the adjacency linked list
    };
    struct Edge {
        int to;          // the index of the destination vertex
        int next, prev;  // the index of the next and previous edges
    };
    Vex* vex;
    Edge* edge;
    AdjacencyList(int N) {
        vex = new Vex[N];
        edge = new Edge[N * 3];
    }
    ~AdjacencyList() {
        delete[] vex;
        delete[] edge;
    }
    void add_edge(int u, int v, int e) {
        edge[e].to = v;
        edge[e].next = vex[u].first_edge;
        edge[e].prev = -1;
        vex[u].first_edge = e;
        if (edge[e].next != -1) edge[edge[e].next].prev = e;
    }
};

}  // namespace TSP

#endif  // _TSP_AdjacencyList_hpp