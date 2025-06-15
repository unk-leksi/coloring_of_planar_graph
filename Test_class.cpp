#include "Test_class.h"
#include <chrono>
using namespace std;


Test::Test(int node_count) : node_count(node_count), n(node_count)
{
    min_map[1].push_back(2); min_map[1].push_back(3);
    min_map[2].push_back(3); min_map[2].push_back(1);
    min_map[3].push_back(1); min_map[3].push_back(2);

    map = min_map; areas[key_min_map] = {};
}

Test::~Test() 
{
    if (adj) {
        for (int i = 0; i < n; ++i) {
            delete[] adj[i];
        }
        delete[] adj;
    }
}

vector<vector<int>> Test::generate()
{
    if (node_count > min_count) {
        srand(time(0));

        for (int new_node = min_count + 1; new_node <= node_count; ++new_node) {
            vector<string> keys;

            for (const auto& p : areas) keys.push_back(p.first);

            int random_index = rand() % keys.size();
            string random_area = keys[random_index];
            areas.erase(random_area);

            vector<int> list_triangle_nodes = decode_area_to_keys(random_area);
            map[map.size() + 1] = list_triangle_nodes;
            int new_node_key = map.size();

            for (int node : list_triangle_nodes)
                map[node].push_back(new_node_key);
            vector<vector<int>> variations = { {0,1},{1,2},{0,2} };

            for (vector<int>& variation : variations) {
                string new_area_key =
                    define_key_for_area(new_node_key,
                        list_triangle_nodes[variation[0]],
                        list_triangle_nodes[variation[1]]);
                vector<int> tmp1 = { new_node_key, list_triangle_nodes[variation[0]] };
                vector<int> tmp2 = { new_node_key, list_triangle_nodes[variation[1]] };
                vector<int> tmp3 = { list_triangle_nodes[variation[0]], list_triangle_nodes[variation[1]] };

                sort(tmp1.begin(), tmp1.end());
                sort(tmp2.begin(), tmp2.end());
                sort(tmp3.begin(), tmp3.end());
                areas[new_area_key] = { tmp1, tmp2, tmp3 };
            }
        }
    }
    areas[key_min_map] = { {1,2},{1,3},{2,3} };

    vector<unordered_set<int>> adj_tmp(node_count, unordered_set<int>());
    for (auto it : map) {
        int u = it.first - 1;
        for (int connected_node : it.second) {
            int v = connected_node - 1;
            adj_tmp[u].insert(v); 
            adj_tmp[v].insert(u);
        }
    }

    vector<vector<int>> adj(node_count);
    for (int i = 0; i < node_count; ++i)
        adj[i] = vector<int>(adj_tmp[i].begin(), adj_tmp[i].end());

    return adj;
}

vector<int> Test::decode_area_to_keys(string& area)
{
    vector<int> res; 
    stringstream ss(area); 
    string temp;

    while (getline(ss, temp, '-'))
        res.push_back(std::stoi(temp));

    return res;
}

string Test::define_key_for_area(int new_node_key, int node1, int node2) 
{
    vector<int> sorted_nodes = { new_node_key, node1, node2 };
    sort(sorted_nodes.begin(), sorted_nodes.end());
    ostringstream oss;

    for (size_t i = 0; i < sorted_nodes.size(); ++i) {
        oss << sorted_nodes[i];
        if (i != sorted_nodes.size() - 1) oss << "-";
    }
    return oss.str();
}


void Test::not_optimal_version(int**& adj, int*& m_size)
{ 
    Graph graph1(n, m_size, adj);

    if (n <= 0) {
        cout << "Количество вершин не натуральное число" << endl;
        return;
    }

    auto start = chrono::high_resolution_clock::now();

    int* beta_coloring = graph1.colorGraph();

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    cout << "Время выполнения не оптимальной версии: " << elapsed.count() << " мс.\n";

    if (!beta_coloring) {
        cout << "Beta_coloring не инициализирован" << endl;
        return;
    }

    for (int v = 0; v < n; ++v) {
        for (int u = 0; u < m_size[v]; ++u) {
            int k = adj[v][u];
            if (beta_coloring[k] == beta_coloring[v]) {
                cout << "error with coloring not optimal version";
                return;
            }
        }
    }

    /*for (int i = 0; i < n; ++i) {
        cout << i <<": " << beta_coloring[i] << " ";
    }*/

    delete[] beta_coloring;
}

void Test::optimal_version(int**& adj, int*& m_size)
{
    Optimal_Graph graph(n, m_size, adj);

    if (n <= 0) {
        cout << "Количество вершин не натуральное число" << endl;
        return;
    }

    auto start = chrono::high_resolution_clock::now();

    int* optimal_coloring = graph.coloring_5();

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    cout << "Время выполнения оптимальной версии: " << elapsed.count() << " мс.\n";

    if (!optimal_coloring) {
        cout << "Optimal_coloring не инициализирован" << endl;
        return;
    }

    for (int v = 0; v < n; ++v) {
        for (int u = 0; u < m_size[v]; ++u) {
            int k = adj[v][u];
            if (optimal_coloring[k] == optimal_coloring[v]) {
                cout << "error with coloring optimal version";
                return;
            }
        }
    }

    /*for (int i = 0; i < n; ++i) {
       cout << i <<": " << optimal_coloring[i] << " ";
   }*/

    delete[] optimal_coloring;
}
