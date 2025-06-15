#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;


class Optimal_Graph {
private:
    int steps, n;
    int* m_size;
    int** adj;

    bool end = false;

public:
    Optimal_Graph();
    Optimal_Graph(int n, int*& m_size, int**& adj);

    int get_steps() { return steps; }

    int read_from_file(const string& filename);
    int write_to_file(const string& filename, int* coloring);

    void display_in_console();

    void dfs(int c1, int c2, int current_v, int v, int* coloring);

    int* coloring_5();
};
