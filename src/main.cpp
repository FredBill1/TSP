#include <fstream>
#include <iostream>
using namespace std;

#include "TSP/Point.hpp"
#include "TSP/TSP.hpp"
#include "TSP/utils.hpp"
using namespace TSP;

TSP_Solver tsp;

Point pts[MAXN];
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    cout << "Input file:  " << argv[1] << '\n';
    cout << "Output file: " << argv[2] << '\n';
    cout << "Start reading file..." << endl;

    ifstream fin(argv[1]);
    int max_iter;
    float term_cond;
    fin >> max_iter >> term_cond >> tsp.N;
    rep(i, 0, tsp.N) fin >> pts[i].x >> pts[i].y;
    fin.close();

    cout << "Total: " << tsp.N << " points\n";
    cout << "maximum iteration: " << max_iter << '\n';
    cout << "termination condition: " << term_cond << '\n';
    cout << "Solving TSP..." << endl;
    rep(i, 0, tsp.N) {
        tsp.dist[i][i] = 0;
        rep(j, i + 1, tsp.N) tsp.dist[j][i] = tsp.dist[i][j] = pts[i].dist(pts[j]);
    }
    tsp.solve(max_iter, term_cond);

    cout << "TSP Solved. Total length: " << tsp.length << "\nWriting output file..." << endl;

    ofstream fout(argv[2]);
    fout << tsp.length << '\n';
    rep(i, 0, tsp.N) fout << tsp.hamilton_path[i] << ' ';
    fout.close();

    cout << "Done." << endl;

    return 0;
}