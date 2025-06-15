#pragma once
#include "graph_project.h"
#include "optimal_graph.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using namespace std;

class Test {
private:
	int n;
	int** adj;

	const int min_count = 3;
	unordered_map<int, vector<int>> min_map;
	const string key_min_map = "1-2-3";

	int node_count;
	unordered_map<int, vector<int>> map;
	unordered_map<string, vector<vector<int>>> areas;

public:
	Test(int node_count);
	~Test();

	void not_optimal_version(int**& adj, int*& m_size);
	void optimal_version(int**& adj, int*& m_size);

	vector<vector<int>> generate();

	vector<int> decode_area_to_keys(string& area);

	string define_key_for_area(int new_node_key, int node1, int node2);
};
