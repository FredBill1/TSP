#include <fstream>
#include <iostream>
#include <memory>
using namespace std;

#include "TSP/Point.hpp"
#include "TSP/TSP.hpp"
#include "TSP/utils.hpp"
using namespace TSP;

int N;
int tmp;
float solution_length;

unique_ptr<TSP_Solver> tsp;

void read_problem(const char* dir) {
    ifstream fin(dir);
    fin >> N;
    tsp = make_unique<TSP_Solver>(N);
    unique_ptr<Point[]> pts = make_unique<Point[]>(N);
    rep(i, 0, N) fin >> tmp >> pts[i].x >> pts[i].y;
    fin.close();

    rep(i, 0, N) {
        tsp->set_dist(i, i, 0);
        rep(j, i + 1, N) tsp->set_dist(i, j, pts[i].dist(pts[j]));
    }

    cout << "total points: " << N << endl;
}

void read_solution(const char* dir) {
    ifstream fin(dir);
    rep(i, 0, N) fin >> tsp->hamilton_path[i], --tsp->hamilton_path[i];
    fin.close();

    tsp->calc_path_length();
    solution_length = tsp->length;

    cout << "solution length: " << solution_length << endl;
}

void solve_problem(int max_iter, float term_cond) { tsp->solve(max_iter, term_cond, true); }

void write_result(const char* name, const char* dir) {
    ofstream fout(dir, ios::app);
    fout << name << ',' << N << ',' << solution_length << ',' << tsp->length << ','
         << (tsp->length - solution_length) / solution_length << '\n';
    fout.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) { return 1; }
    read_problem(argv[1]);
    read_solution(argv[2]);
    solve_problem(5, 1e-2f);
    write_result(argv[1], argv[3]);
    return 0;
}
