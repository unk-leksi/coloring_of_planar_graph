#include "optimal_graph.h"

using namespace std;



Optimal_Graph::Optimal_Graph() 
{
    n = 0;
    steps = 0;
    adj = nullptr;
    m_size = nullptr;
}

Optimal_Graph::Optimal_Graph(int n, int*& m_size, int**& adj) : n(n), m_size(m_size), adj(adj) 
{
    steps = 0;
}


int Optimal_Graph::read_from_file(const string& filename)
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

int Optimal_Graph::write_to_file(const string& filename, int* coloring) 
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
                cout << "error with coloring optimal version";
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


void Optimal_Graph::display_in_console() 
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


void Optimal_Graph::dfs(int c1, int c2, int current_v, int v, int* coloring)
{
    int** stck = new int* [n];
    int size = 0;
    stck[size] = new int[3];
    stck[size][0] = c1;
    stck[size][1] = c2;
    stck[size][2] = current_v;
    size++;

    while (size != 0 && !end) {
        ++steps;

        int col1 = stck[size - 1][0], col2 = stck[size - 1][1],
            u = stck[size - 1][2];

        delete stck[size - 1];
        size--;

        coloring[u] = col1;

        for (int i = 0; i < m_size[u]; ++i) {

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

int* Optimal_Graph::coloring_5()
{
    int* coloring = new int[n];

    for (int i = 0; i < n; ++i) {
        coloring[i] = 0;
    }

    bool* used_colors = new bool[5];
    for (int v = 0; v < n; ++v) {
        ++steps;

        for (int i = 0; i < 5; ++i) {
            ++steps;
            used_colors[i] = 0;
        }

        for (int i = 0; i < m_size[v]; ++i) {
            ++steps;

            int u = adj[v][i];
            if (coloring[u] != 0)
                used_colors[coloring[u]] = 1;
        }

        int cur_color = 0;
        while (coloring[v] == 0) {
            ++cur_color;
        
            if (cur_color > 5) {
                int flag = 0;
                for (int c1 : {1, 5}) {
                    ++steps;

                    for (int c2 : {2, 3, 4}) {
                        ++steps;

                        end = false;
                        int* recoloring = new int[n];
                        for (int i = 0; i < n; ++i) {
                            ++steps;

                            recoloring[i] = coloring[i];
                        }
                        dfs(c1, c2, v, v, recoloring);
                        if (!end) {
                            for (int i = 0; i < n; ++i) {
                                ++steps;

                                coloring[i] = recoloring[i];
                            }
                            delete[] recoloring;
                            flag = 1;
                        }
                        if (flag == 1) {
                            break;
                        }
                        delete[] recoloring;
                    }
                    if (flag == 1) {
                        break;
                    }
                }
            }

            if (used_colors[cur_color] == 0 && cur_color < 6) {
                coloring[v] = cur_color;
                break;
            }
        }
    }
    delete[] used_colors;

    cout << "Количество шагов оптимальной версии: " << steps << endl;
    return coloring;
}
