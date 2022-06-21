#ifndef _tsp_Unioufind_hpp
#define _tsp_Unioufind_hpp
namespace TSP {

struct Unionfind {
    int* fa;
    Unionfind(int n) {
        fa = new int[n + 1];
        for (int i = 0; i <= n; ++i) fa[i] = i;
    }
    ~Unionfind() { delete[] fa; }
    inline void init(int n) {}
    inline int find(int x) {
        if (x == fa[x]) return x;
        int t = x;
        while (t != fa[t]) t = fa[t];
        while (x != t) {
            int u = fa[x];
            fa[x] = t;
            x = u;
        }
        return x;
    }
    inline bool merge(int x1, int x2) {
        int f1 = find(x1), f2 = find(x2);
        if (f1 == f2) return false;
        fa[f1] = f2;
        return true;
    }
};

}  // namespace TSP
#endif  // _tsp_Unioufind_hpp