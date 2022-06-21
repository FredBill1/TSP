#ifndef _TSP_Tour_hpp
#define _TSP_Tour_hpp

#include <iterator>

#include "TSP/config.hpp"
namespace TSP {

struct Tour {
    int node[MAXN][2];
    Tour(int path[], int cnt) {
        for (int i = 0; i < cnt; ++i) {
            int u = path[i ? i - 1 : cnt - 1], v = path[i];
            node[u][1] = v;
            node[v][0] = u;
        }
    }
    struct iterator : std::iterator<std::bidirectional_iterator_tag, int> {
        Tour* tour;
        int cur, pre;
        iterator(Tour* t, int cur, int pre) : tour(t), cur(cur), pre(pre) {}
        inline iterator& operator++() {
            int nxt = tour->node[cur][0] == pre ? tour->node[cur][1] : tour->node[cur][0];
            pre = cur, cur = nxt;
            return *this;
        }
        inline iterator& operator--() {
            int pre_ = tour->node[pre][0] == cur ? tour->node[pre][1] : tour->node[pre][0];
            cur = pre, pre = pre_;
            return *this;
        }
        inline iterator operator++(int) {
            iterator ret = *this;
            return ++*this, ret;
        }
        inline iterator operator--(int) {
            iterator ret = *this;
            return --*this, ret;
        }
        inline int operator*() const { return cur; }
        inline bool operator==(const iterator& rhs) const { return cur == rhs.cur; }
        inline bool operator!=(const iterator& rhs) const { return !(*this == rhs); }
        inline void set_next(int x) const {
            int& pos = tour->node[cur][0] == pre ? tour->node[cur][1] : tour->node[cur][0];
            pos = x;
        }
        inline void set_prev(int x) {
            int& pos = tour->node[cur][0] == pre ? tour->node[cur][0] : tour->node[cur][1];
            pos = pre = x;
        }
        inline void toggle() { pre = tour->node[cur][0] == pre ? tour->node[cur][1] : tour->node[cur][0]; }
        inline int prev() { return pre; }
        inline int next() { return tour->node[cur][0] == pre ? tour->node[cur][1] : tour->node[cur][0]; }
    };
    inline iterator at(int pos, int dir = 0) { return iterator(this, pos, node[pos][dir]); }

    // any existing iterator between (before_first, before_last) will be invalid
    inline void reverse(const iterator& before_first, iterator& before_last) {
        // no need to check the position because it is guaranteed to be valid during 3-opt
        // if (before_first == before_last) return;
        iterator first = std::next(before_first);
        // if (first == before_last) return;
        iterator last = std::next(before_last);
        before_first.set_next(*before_last);
        before_last.set_next(*before_first);
        first.set_prev(*last);
        last.set_prev(*first);
        first.toggle();
        before_last = first;
    }
    inline void reverse(const iterator& before_first, iterator&& before_last) { reverse(before_first, before_last); }
    inline void rotate(iterator& before_first, iterator& before_n_first, iterator& before_last) {
        reverse(before_first, before_n_first);
        reverse(before_n_first, before_last);
        // reverse(before_last, before_first);
        reverse(before_first, before_last);
    }
    inline void rotate(iterator&& before_first, iterator&& before_n_first, iterator&& before_last) {
        rotate(before_first, before_n_first, before_last);
    }
};

};  // namespace TSP

#endif