#include <iostream>
#include "not_optimal_g.h"
#include "optimal_g.h"
#include "Test_class.h"
#include <chrono>
#pragma warning(disable:4996)
using namespace std;



int main() {
	setlocale(LC_ALL, "Russian");


    while (true) {
        cout << endl;
        cout << "==============================================" << endl;
        cout << "         Раскраска планарного графа:     " << endl;
        cout << "==============================================" << endl;
        cout << "1. Запустить тестирование базового алгоритма" << endl;
        cout << "2. Запустить тестирование 2-ого алгоритма" << endl;
        cout << "3. Запустить тестирование обоих алгоритмов" << endl;
        cout << "4. Выход" << endl;
        cout << "Введите ваш выбор: ";
        int choice;
        cin >> choice;

        if (choice == 4) {
            cout << "Выход из программы." << endl;
            return 0;
        }

        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Пожалуйста, введите число от 1 до 5.\n" << endl;
            continue;
        }

        int n;
        cout << "Введите количество вершин: ";
        cin >> n;

        if (n <= 0) {
            cout << "Пожалуйста, введите число натуральное число.\n" << endl;
            break;
        }

        Test test_graph(n);
        vector<vector<int>> adj_tmp = test_graph.generate();

        int** adj = new int* [n];
        int* m_size = new int[n];
        for (int i = 0; i < n; ++i) {
            m_size[i] = adj_tmp[i].size();
            adj[i] = new int[m_size[i]];
            for (int j = 0; j < m_size[i]; ++j) adj[i][j] = adj_tmp[i][j];
        }
        
        for (int i = 0; i < n; ++i) {
            cout << i << ": ";
            for (int j = 0; j < m_size[i]; ++j) {
                cout << adj[i][j] << " ";
            }
            cout << endl;
        }
      
        cout << endl;
        switch (choice) {
        case 1:
            test_graph.not_optimal_version(adj, m_size);
            break;
        case 2:
            test_graph.optimal_version(adj, m_size);
            break;
        case 3:
            test_graph.not_optimal_version(adj, m_size);
            cout << endl;
            test_graph.optimal_version(adj, m_size);
            break;
        default:
            std::cout << "Неверный выбор. Пожалуйста, попробуйте снова.\n" << std::endl;
            break;
        }
    }


	return 0;
}
