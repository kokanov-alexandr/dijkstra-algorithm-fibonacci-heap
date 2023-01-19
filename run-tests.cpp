#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

#include "dijkstra-s_algorithm.h"
#include "generator_tests.h"

using namespace std;
using namespace std::chrono;

const int kMillion = 1e6;
const int kCountMadeCorrectnessDijTests = 23;
const int kCountAllCorrectnessDijTests = 31;
const int kCountCorrectnessFibHeapTests = 10;
const int kCountTimeTests = 10;
int CorrectDijkstraTests = 0;
int FailedDijkstraTests = 0;
int CorrectFibHeapTests = 0;
int FailedFibHeapTests = 0;

/*
    Формат тестов:
    insert a b - вставить элемент со значением a и ключом b

    remove_minimum -удалить минимум и вернуть вывести его значение
    (гарантируется, что куча не пустая)

    decrease_key a b - присвоить элементу под ключом a ключ b
    (гарантируется, что в куче есть элемент с ключом a, и только один)
*/

void RunFibHeapTests() {
    cout << "Run Fibonacci Heap tests\n";
    string ans_file_name, out_file_name, in_file_name;
    for (int i = 1; i <= kCountCorrectnessFibHeapTests; ++i) {
        in_file_name = "../tests-fib-heap/" + to_string(i) + ".in";
        ans_file_name = "../tests-fib-heap/" + to_string(i) + ".ans";
        out_file_name = "../tests-fib-heap/output.txt";

        ifstream answer(ans_file_name);
        ofstream output(out_file_name);
        ifstream input(in_file_name);

        auto fib_heap = new FibonacciHeap<int>();
        string action;
        int value, key, new_key;

        while (input >> action) {
            if (action == "insert") {
                input >> value >> key;
                fib_heap->Insert(key);
            }
            else if (action == "remove_minimum") {
                output << fib_heap->RemoveMinimum()->key << " ";
            }
            else if (action == "decrease_key") {
                input >> new_key >> key;
                auto element = fib_heap->Find(key);
                fib_heap->DecreaseKey(element, new_key);
            }
        }
        input.close();
        output.close();
        ifstream output_2(out_file_name);

        int a, b;
        bool is_test_right = true;
        while (answer >> b && output_2 >> a) {
            if (a != b) {
                FailedFibHeapTests++;
                is_test_right = false;
                break;
            }
        }
        cout << "Test " << i << ": ";
        if (is_test_right) {
            CorrectFibHeapTests++;
            cout << "ok.\n";
        }
        else {
            cout << "error.\n";
        }
        answer.close();
        output_2.close();
    }
    cout << "Correct tests: " << CorrectFibHeapTests << endl;
    cout << "Failed tests: " << FailedFibHeapTests << endl << endl;
}

bool CheckAnswer(const vector<int> &distance, const vector<int> &answers) {
    for (int i = 1; i < distance.size(); ++i) {
        if (answers[i] != distance[i]) {
            return false;
        }
    }
    return true;
}

/*
    Формат тестов:
    Первая строка: n - количество вершин, m - количество рёбер, s - стартовая вершина
    m рёбер: начало, конец, вес
*/

void RunDijkstraTests() {
    cout << "Run Dijkstra tests\n";
    pair<EDGES_LISTS, int> (*GeneratorTest[])() = {GenerateTest1, GenerateTest2, GenerateTest3, GenerateTest4,
                                                   GenerateTest5, GenerateTest6, GenerateTest7, GenerateTest8};

    for (int test_number = 1; test_number <= kCountAllCorrectnessDijTests; ++test_number) {
        int count_vertex, count_edges, first, second, length, start_vertex;
        EDGES_LISTS edges_lists;
        vector<int> answers;

        string ans_file_name = "../tests-Dijkstra/" + to_string(test_number) + ".ans";
        string out_file_name = "../tests-Dijkstra/output.txt";
        ifstream answer(ans_file_name);
        ofstream output(out_file_name);

        if (test_number > kCountMadeCorrectnessDijTests) {
            int count = test_number - kCountMadeCorrectnessDijTests - 1;
            auto graph = GeneratorTest[count]();
            edges_lists = graph.first;
            start_vertex = graph.second;

        }
        else {
            string in_file_name = "../tests-Dijkstra/" + to_string(test_number) + ".in";
            ifstream input(in_file_name);
            input >> count_vertex >> count_edges >> start_vertex;
            edges_lists = EDGES_LISTS(count_vertex + 1, vector<pair<int, int>>());
            for (int i = 0; i < count_edges; ++i) {
                input >> first >> second >> length;
                edges_lists[first].push_back({second, length});
            }
            input.close();
        }

        auto start = high_resolution_clock::now();
        auto distance = Dijkstra<FibonacciHeap>(edges_lists, start_vertex);
        auto stop = high_resolution_clock::now();

        answers = vector<int>(count_vertex + 1, 0);

        for (int i = 1; i < distance.size(); ++i) {
            output << distance[i] << " ";
        }

        if (test_number > kCountMadeCorrectnessDijTests) {
            answers = Dijkstra<PriorityQueue>(edges_lists, start_vertex);
        }
        else {
            for (int i = 1; i < answers.size(); ++i) {
                answer >> answers[i];
            }
        }

        cout << "Test " << test_number << ": ";

        if (CheckAnswer(distance, answers)) {
            CorrectDijkstraTests++;
            cout << "ok. ";
            cout << "Time: " << (double)duration_cast<microseconds>(stop - start).count() / kMillion << " sec" << endl;
        }
        else {
            FailedDijkstraTests++;
            cout << "error." << endl;
        }

        answer.close();
        output.close();
    }
    cout << "Correct tests: " << CorrectDijkstraTests << endl;
    cout << "Failed tests: " << FailedDijkstraTests << endl;

}

double GetAverage(vector<double> &operations_time) {
    double average = 0;
    for (double elem: operations_time)
        average += elem;
    return average / (double)operations_time.size();
}

void TimeComparison() {
    ofstream count_elements_f("../tests-Info/count_elements.txt");
    ofstream embedded_queue_operations_time_f("../tests-Info/embedded_priority_queue_dij.txt");
    ofstream fib_heap_operations_time_f("../tests-Info/fib_heap_dij.txt");
    ofstream naive_realization_operations_time_f("../tests-Info/naive_implement_dij.txt");

    cout << "Time comparison tests:" << endl;

    for (int i = 1; i <= kCountTimeTests; ++i) {

        int start_vertex;
        vector<double> fib_heap_operations_time;
        vector<double> naive_realization_operations_time;
        vector<double> embedded_queue_operations_time;

        int count_elements = (int)pow(10, 5) * i;
        count_elements_f << count_elements << endl;

        int iter = 0;
        while (iter < 10) {
            auto graph = GenerateRandomTest(count_elements, count_elements * 3, start_vertex);
            EDGES_LISTS edges_lists(count_elements + 1, vector<pair<int, int>>());

            for (int j = 1; j <= count_elements; ++j) {
                edges_lists[graph[j].head].push_back({graph[j].tail, graph[j].length});
            }

            auto start_1 = high_resolution_clock::now();
            auto distance = Dijkstra<FibonacciHeap>(edges_lists, start_vertex);
            auto stop_1 = high_resolution_clock::now();
            auto execution_time = duration_cast<microseconds>(stop_1 - start_1).count();
            fib_heap_operations_time.push_back(execution_time);


            auto start_2 = high_resolution_clock::now();
            distance = Dijkstra<VectorQueue>(edges_lists, start_vertex);
            auto stop_2 = high_resolution_clock::now();
            execution_time = duration_cast<microseconds>(stop_2 - start_2).count();
            naive_realization_operations_time.push_back(execution_time);

            auto start_3 = high_resolution_clock::now();
            distance = Dijkstra<PriorityQueue>(edges_lists, start_vertex);
            auto stop_3 = high_resolution_clock::now();
            execution_time = duration_cast<microseconds>(stop_3 - start_3).count();
            embedded_queue_operations_time.push_back(execution_time);

            iter++;
        }

        fib_heap_operations_time_f << GetAverage(fib_heap_operations_time) / kMillion << endl;
        naive_realization_operations_time_f << GetAverage(naive_realization_operations_time) / kMillion << endl;
        embedded_queue_operations_time_f << GetAverage(embedded_queue_operations_time) / kMillion << endl;

        cout << "test " << i << " passed\n";
    }
    count_elements_f.close();
    embedded_queue_operations_time_f.close();
    fib_heap_operations_time_f.close();
    naive_realization_operations_time_f.close();
}

int main() {
    RunFibHeapTests();
    RunDijkstraTests();
    return 0;
}