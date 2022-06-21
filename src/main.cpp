#include <fstream>
#include <iostream>
using namespace std;

#include "TSP/Point.hpp"
#include "TSP/TSP.hpp"
#include "TSP/utils.hpp"
using namespace TSP;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    cout << "Input file:  " << argv[1] << '\n';
    cout << "Output file: " << argv[2] << "\n\n";
    cout << "Start reading file..." << endl;

    ifstream fin(argv[1]);
    int N, max_iter;
    float term_cond;
    fin >> max_iter >> term_cond >> N;
    TSP_Solver tsp(N);

    Point* pts = new Point[N];
    rep(i, 0, tsp.N) fin >> pts[i].x >> pts[i].y;
    fin.close();

    cout << "Total: " << tsp.N << " points\n";
    cout << "maximum iteration: " << max_iter << '\n';
    cout << "termination condition: " << term_cond << "\n\n";
    cout << "Solving TSP..." << endl;
    rep(i, 0, tsp.N) {
        tsp.set_dist(i, i, 0);
        rep(j, i + 1, N) tsp.set_dist(i, j, pts[i].dist(pts[j]));
    }
    delete[] pts;
    tsp.solve(max_iter, term_cond, true);

    cout << "TSP Solved. Total length: " << tsp.length << "\n\nWriting output file..." << endl;

    ofstream fout(argv[2]);
    fout << tsp.length << '\n';
    rep(i, 0, tsp.N) fout << tsp.hamilton_path[i] << ' ';
    fout.close();

    cout << "Done." << endl;

    return 0;
}
