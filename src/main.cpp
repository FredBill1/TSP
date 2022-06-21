#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include "TSP/Point.hpp"
#include "TSP/TSP.hpp"
#include "TSP/utils.hpp"
using namespace TSP;

int N;
Point* pts;

static inline void process_html(TSP_Solver& tsp, const char* input, const char* output) {
    ifstream in(input);
    string input_template((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    constexpr char magic[] = "// ~@#!-!#@~ //";
    size_t pos = input_template.find(magic);
    if (pos == string::npos) {
        cout << "Error: " << magic << " not found in " << input << endl;
        return;
    }
    ofstream out(output);
    for (size_t i = 0; i < pos; ++i) out << input_template[i];
    rep(i, 0, N) {
        int u = tsp.hamilton_path[i];
        out << '"' << pts[u].x << '|' << pts[u].y << "\",";
    }
    out << '"' << pts[0].x << '|' << pts[0].y << '"';
    for (size_t i = pos + sizeof(magic) - 1; i < input_template.size(); ++i) out << input_template[i];
    out.close();
};

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage: " << argv[0] << " <input_file> <output_file> <input_html> <output_html>" << endl;
        return 1;
    }
    cout << "Input file:  " << argv[1] << '\n';
    cout << "Output file: " << argv[2] << "\n\n";
    cout << "Start reading file..." << endl;

    ifstream fin(argv[1]);
    int max_iter;
    float term_cond;
    fin >> max_iter >> term_cond >> N;
    TSP_Solver tsp(N);

    pts = new Point[N];
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
    tsp.solve(max_iter, term_cond, true);

    cout << "TSP Solved. Total length: " << tsp.length << "\n\nWriting output file..." << endl;

    ofstream fout(argv[2]);
    fout << tsp.length << '\n';
    rep(i, 0, tsp.N) fout << tsp.hamilton_path[i] << ' ';
    fout.close();

    cout << "Processing HTML file..." << endl;
    process_html(tsp, argv[3], argv[4]);

    cout << "Done." << endl;

    delete[] pts;
    return 0;
}
