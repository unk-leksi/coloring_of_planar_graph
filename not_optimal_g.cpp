#include "not_optimal_g.h"
#include <vector>
#pragma warning(disable:4996)
using namespace std;



Graph::Graph() 
{
    removed_size = 0;
    n = 0;
    step_not_optimal = 0;
    m_size = nullptr;
    end = false;
    adj = nullptr;
    deg_5 = nullptr;
}

Graph::Graph(int n, int*& m_size, int**& adj) : n(n), adj(adj), m_size(m_size)
{
    removed_size = n;
    step_not_optimal = 0;
    deg_5 = new int[n];
}


int Graph::read_from_file(const string& filename)
{
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Ошибка: не удалось открыть файл" << endl;
        return 1;
    }

    int sizeFromFile;
    inputFile >> sizeFromFile;

    if (sizeFromFile <= 0) {
        cout << "Ошибка: количество вершин должно быть больше 0" << endl;
        return 1;
    }

    n = sizeFromFile;

    vector<vector<int>> adj_tmp(n);

    inputFile.ignore();

    string line;
    while (getline(inputFile, line)) {
        if (line.empty()) continue;

        istringstream ss(line);
        int vertex;
        ss >> vertex;

        if (vertex < 0 || vertex >= n) {
            cout << "Ошибка: неверный номер вершины: " << vertex << endl;
            return 1;
        }

        int connectedVertex;
        while (ss >> connectedVertex) {
            if (connectedVertex < 0 || connectedVertex >= n) {
                cout << "Ошибка: неверный номер связанной вершины: " << connectedVertex << endl;
                return 1;
            }

            adj_tmp[vertex].push_back(connectedVertex);
        }
    }

    inputFile.close();

    adj = new int* [n];
    m_size = new int[n];
    for (int i = 0; i < n; ++i) {
        m_size[i] = adj_tmp[i].size();
        adj[i] = new int[m_size[i]];
        for (int j = 0; j < m_size[i]; ++j) {
            adj[i][j] = adj_tmp[i][j];
        }
    }

    return 0;
}

int Graph::write_to_file(const string& filename, int* coloring) 
{
    if (!m_size || !adj || !coloring) {
        cout << "Ошибка: память не выделена корректно" << endl;
        return 1;
    }

    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cout << "Ошибка: не удалось открыть файл для записи" << endl;
        return 1;
    }

    for (int v = 0; v < 8; ++v) {
        for (int u = 0; u < m_size[v]; ++u) {
            int k = adj[v][u];
            if (coloring[k] == coloring[v]) {
                cout << "error with coloring not optimal version";
                return 1;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        outputFile << i << " : " << coloring[i] << endl;
    }

    outputFile.close();
    return 0;
}


void Graph::display_in_console() 
{
    if (!m_size || !adj) {
        cout << "Ошибка: граф не инициализирован" << endl;
        return;
    }

    for (int i = 0; i < n; ++i) {
        cout << i << ": ";
        for (int j = 0; j < m_size[i]; ++j) {
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
}


void Graph::dfs(int c1, int c2, int current_v, int v, int* coloring) 
{
    if (end) return;
    coloring[current_v] = c1;

    for (int i = 0; i < m_size[current_v]; ++i) {
        ++step_not_optimal;

        int neighbor = adj[current_v][i];
        if (neighbor == v && coloring[v] == c1) {
            end = true;
        }
        else if (!end && coloring[neighbor] == c1) {
            dfs(c2, c1, neighbor, v, coloring);
        }
    }
}

void Graph::dfs_iterative(int c1, int c2, int current_v, int v, int* coloring) 
{
    int** stck = new int* [n];
    int size = 0;
    stck[size] = new int[3];
    stck[size][0] = c1;
    stck[size][1] = c2;
    stck[size][2] = current_v;
    size++;

    while (size != 0 && !end) {
        ++step_not_optimal;

        int col1 = stck[size - 1][0], col2 = stck[size - 1][1],
            u = stck[size - 1][2];

        delete stck[size - 1];
        size--;

        coloring[u] = col1;

        for (int i = 0; i < m_size[u]; ++i) {
            ++step_not_optimal;

            int neighbor = adj[u][i];

            if (neighbor == v && coloring[v] == col1) {
                end = true;
                break;
            }
            else if (!end && coloring[neighbor] == col1) {
                stck[size] = new int[3];
                stck[size][0] = col2;
                stck[size][1] = col1;
                stck[size][2] = neighbor;
                size++;
            }
            if (end) break;
        }
    }

    delete[] stck;
}


void Graph::graph_5_coloring(int* coloring) 
{
    if (removed_size <= 5) {
        int color = 0;
        for (int i = 0; i < n; ++i) {
            ++step_not_optimal;

            if (m_size[i] > 0) {
                coloring[i] = color++;
            }
        }
        return;
    }

    int v = deg_5[--removed_size];

    if (v < 0 || v >= n) {
        cout << "Ошибка: индекс вершины выходит за границы" << endl;
        return;
    }

    int* adjacent = new int[m_size[v]];
    int adj_size = m_size[v];
    for (int i = 0; i < adj_size; ++i) {
        ++step_not_optimal;

        adjacent[i] = adj[v][i];
    }

    for (int i = 0; i < adj_size; ++i) {
        ++step_not_optimal;

        int neighbor = adjacent[i];
        for (int j = 0; j < m_size[neighbor]; ++j) {
            ++step_not_optimal;

            if (adj[neighbor][j] == v) {
                adj[neighbor][j] = adj[neighbor][--m_size[neighbor]];
                if (m_size[neighbor] == 5) {
                    deg_5[removed_size++] = neighbor;
                }
                break;
            }
        }
    }
    m_size[v] = 0;

    graph_5_coloring(coloring);

    for (int i = 0; i < adj_size; ++i) {
        ++step_not_optimal;

        int neighbor = adjacent[i];
        adj[neighbor][m_size[neighbor]++] = v;
    }
    m_size[v] = adj_size;

    for (int i = 0; i < adj_size; ++i) {
        ++step_not_optimal;

        adj[v][i] = adjacent[i];
    }

    int colors[5] = { 0 };
    for (int i = 0; i < adj_size; ++i) {
        ++step_not_optimal;

        int neighbor_color = coloring[adjacent[i]];
        if (neighbor_color < 5) {
            colors[neighbor_color] = 1;
        }
    }

    int vertex_color = 0;
    while (vertex_color < 5 && colors[vertex_color]) {
        ++step_not_optimal;

        vertex_color++;
    }

    if (vertex_color < 5) {
        coloring[v] = vertex_color;
    }
    else {
        for (int c1 : {0, 4}) {
            ++step_not_optimal;

            for (int c2 : {1, 2, 3}) {
                ++step_not_optimal;

                end = false;
                int* recoloring = new int[n];
                for (int i = 0; i < n; ++i) {
                    ++step_not_optimal;

                    recoloring[i] = coloring[i];
                }
                dfs_iterative(c1, c2, v, v, recoloring);
                if (!end) {
                    for (int i = 0; i < n; ++i) {

                        ++step_not_optimal;
                        coloring[i] = recoloring[i];
                    }
                    delete[] recoloring;
                    delete[] adjacent;
                    return;
                }
                delete[] recoloring;
            }
        }
    }

    delete[] adjacent;
}



int* Graph::colorGraph() 
{
    int* coloring = new int[n];
    for (int i = 0; i < n; ++i) {
        ++step_not_optimal;
        coloring[i] = 0;
    }

    deg_5 = new int[n];
    int deg_count = 0;

    for (int i = 0; i < n; ++i) {
        ++step_not_optimal;

        if (m_size[i] <= 5) {
            deg_5[deg_count++] = i;
        }
    }

    removed_size = deg_count;

    graph_5_coloring(coloring);

    delete[] deg_5;

    cout << "Количество шагов не оптимальной версии: " << step_not_optimal << endl;

    return coloring;
}
