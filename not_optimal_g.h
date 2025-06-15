#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#pragma warning(disable:4996)
using namespace std;

class Graph {
private:
    int step_not_optimal, n, removed_size;
    int* m_size;
    int** adj;
    int* deg_5;

    bool end = false;

public:
    Graph();
    Graph(int n, int*& m_size, int**& adj);

    int get_steps() { return step_not_optimal; }

    void dfs(int c1, int c2, int current_v, int v, int* coloring);
    void dfs_iterative(int color1, int color2, int current_v, int v, int* coloring);

    int read_from_file(const string& filename);
    int write_to_file(const string& filename, int* coloring);

    void display_in_console();

    void graph_5_coloring(int* coloring);
    int* colorGraph();
};
