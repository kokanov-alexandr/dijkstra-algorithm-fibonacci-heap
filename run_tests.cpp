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
const int kCountMadeFibHeapTests = 15;
const int kCountSawFibHeapTests = 5;
const int kCountHillFibHeapTests = 5;
const int kCountMadeCorrectnessDijTests = 24;
const int kCountGenCorrectnessDijTests = 6;
const int kCountTimeComparisonTests = 10;
int CorrectTests = 0;
int FailedTests = 0;
int TestNumber = 0;

bool CheckAnswer(const vector<int> &our_answers, const vector<int> &right_answers) {
    for (int i = 1; i < our_answers.size(); ++i) {
        if (our_answers[i] != right_answers[i]) {
            return false;
        }
    }
    return true;
}

void PrintTestResult(bool is_test_correct) {
    cout << "Test " << TestNumber << ": ";
    cout << (is_test_correct ? "ok." : "error.") << endl;
    is_test_correct ? CorrectTests++ : FailedTests++;
}


bool SawTest(int t, int k) {
    FibonacciHeap<int> fh;
    PriorityQueue<int> pq;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < t; j++) {
            int elem = GetRandomNum(1, 10000);
            fh.Insert(elem);
            pq.Insert(elem);
        }
        int a = fh.GetMinimum(), b = pq.GetMinimum();
        fh.RemoveMinimum();
        pq.RemoveMinimum();
        if (a != b) {
            return false;
        }
    }
    return true;
}

bool HillTestInt(int n) {
    FibonacciHeap<int> fh;
    PriorityQueue<int> pq;
    for (int i = 0; i < n; ++i) {
        int elem = GetRandomNum(1, 10000);
        fh.Insert(elem);
        pq.Insert(elem);
    }
    for (int i = 0; i < n; ++i) {
        int a = fh.GetMinimum(), b = pq.GetMinimum();
        if (a != b) {
            return false;
        }
        fh.RemoveMinimum();
        pq.RemoveMinimum();
    }
    return true;
}

/*
 * Формат тестов
 *
 * Тесты с фпайлов:
 * insert a - вставить элемент с ключом a
 *
 * remove_minimum -удалить минимум
 * (гарантируется, что куча не пустая)
 *
 * decrease_key a b - присвоить элементу под ключом a ключ b
 * (гарантируется, что в куче есть элемент с ключом a, и только один)
 *
 * get_minimum - вывести ключ минимума
 * (гарантируется, что куча не пустая)
 *
 * Saw тесты:
 * t раз Insert, затем RemoveMinimum, повторить k раз
 *
 * Hill tests:
 * n раз Insert, затем n раз RemoveMinimum
*/

void RunFibHeapTests() {
    CorrectTests = 0;
    FailedTests = 0;
    cout << "Run Fibonacci Heap tests\n";

    cout << "Small tests:\n";
    for (TestNumber = 1; TestNumber <= kCountMadeFibHeapTests; ++TestNumber) {
        ifstream input("../tests-fib-heap/" + to_string(TestNumber) + ".in");
        ifstream answer("../tests-fib-heap/" + to_string(TestNumber) + ".ans");

        auto fib_heap = new FibonacciHeap<int>();
        string action;
        int key, new_key;
        auto heap_answers = vector<int>();
        auto right_answers = vector<int>();

        auto start = high_resolution_clock::now();
        while (input >> action) {
            if (action == "insert") {
                input >> key;
                fib_heap->Insert(key);
            }
            else if (action == "remove_minimum") {
                 fib_heap->RemoveMinimum();
            }
            else if (action == "get_minimum") {
                heap_answers.push_back(fib_heap->GetMinimum());
            }
            else if (action == "decrease_key") {
                input >> new_key >> key;
                auto element = fib_heap->Find(key);
                fib_heap->DecreaseKey(element, new_key);
            }
        }
        auto stop = high_resolution_clock::now();
        input.close();

        int a;
        while (answer >> a) {
            right_answers.push_back(a);
        }
        PrintTestResult(CheckAnswer(heap_answers, right_answers));
        answer.close();
    }
    cout << "Saw tests:\n";
    for (TestNumber = 1; TestNumber <= kCountSawFibHeapTests; ++TestNumber) {
        PrintTestResult(SawTest(pow(2, TestNumber), pow(2, TestNumber) * 2));
    }
    cout << "Hill tests:\n";
    for (TestNumber = 1; TestNumber <= kCountHillFibHeapTests; ++TestNumber) {
        PrintTestResult(HillTestInt(pow(10, TestNumber)));
    }

    cout << "Correct tests: " << CorrectTests << endl;
    cout << "Failed tests: " << FailedTests << endl << endl;
}

/*
 * Формат тестов:
 * Первая строка: n - количество вершин, m - количество рёбер, s - стартовая вершина
 * m рёбер: начало, конец, вес
*/

void RunDijkstraTests() {
    CorrectTests = 0;
    FailedTests = 0;
    cout << "Run Dijkstra tests\n";

    int count_vertex, count_edges, first, second, length, start_vertex;
    EDGES_LISTS edges_lists;
    vector<int> answers;

    cout << "Small tests:\n";
    for (TestNumber = 1; TestNumber <= kCountMadeCorrectnessDijTests; ++TestNumber) {
        ifstream input("../tests-Dijkstra/" + to_string(TestNumber) + ".in");
        ifstream answer("../tests-Dijkstra/" + to_string(TestNumber) + ".ans");
        input >> count_vertex >> count_edges >> start_vertex;
        edges_lists = EDGES_LISTS(count_vertex + 1, vector<pair<int, int>>());
        answers = vector<int>(count_vertex + 1, 0);

        for (int i = 0; i < count_edges; ++i) {
            input >> first >> second >> length;
            edges_lists[first].push_back({second, length});
        }

        for (int i = 1; i < answers.size(); ++i) {
            answer >> answers[i];
        }
        input.close();
        answer.close();
        auto distance = Dijkstra<FibonacciHeap>(edges_lists, start_vertex);

        bool is_test_correct = CheckAnswer(distance, answers);
        PrintTestResult(is_test_correct);
    }

    cout << "Big tests:\n";
    int graph_type = CYCLE;
    for (TestNumber = 1; graph_type <= LOOPS; ++TestNumber, ++graph_type) {
        switch (graph_type) {
            case MULTIPLE_EDGES:
                count_vertex = 1e5;
                break;
            case TREE:
                count_vertex = 1e5;
                break;
            default:
                count_vertex = 1e6;
        }
        auto test = GenCorrTestGraph(static_cast<GraphCorrTestType>(graph_type), count_vertex);
        auto start = high_resolution_clock::now();
        auto distance = Dijkstra<FibonacciHeap>(get<0>(test), get<1>(test));
        auto stop = high_resolution_clock::now();
        bool is_test_correct = CheckAnswer(distance, get<2>(test));
        PrintTestResult(is_test_correct);
        if (is_test_correct) {
            cout << "Time: " << (double)duration_cast<microseconds>(stop - start).count() / kMillion << " sec" << endl;
        }
    }
    cout << endl;
    cout << "Correct tests: " << CorrectTests << endl;
    cout << "Failed tests: " << FailedTests << endl;

}

template<template<class> class T>
double GetTimeOperation(EDGES_LISTS edges_lists, int start_vertex) {
    auto start = high_resolution_clock::now();
    auto distance = Dijkstra<T>(edges_lists, start_vertex);
    auto stop = high_resolution_clock::now();
    return duration_cast<microseconds>(stop - start).count();
}

void TimeComparison() {
    ofstream counts_vertex_f("../tests-Info/count_elements.txt");
    ofstream embedded_queue_operations_time_f("../tests-Info/embedded_priority_queue_dij.txt");
    ofstream fib_heap_operations_time_f("../tests-Info/fib_heap_dij.txt");

    EDGES_LISTS edges_lists;

    cout << "Time comparison tests:" << endl;

    int count_iteration = 10;
    for (TestNumber = 1; TestNumber <= kCountTimeComparisonTests; ++TestNumber) {
        int count_vertex, count_edges, start_vertex;
        vector<double> fib_heap_operations_time;
        vector<double> embedded_queue_operations_time;

        count_vertex = (int)pow(10, 3) * TestNumber;
        count_edges = count_vertex * (count_vertex - 1);
        counts_vertex_f << count_edges << endl;

        double fib_heap_time = 0, priority_queue_time = 0;

        for (int iter = 0; iter < count_iteration + 5; ++iter) {
            auto graph = GenTimeTestGraph(FULL, count_vertex);
            edges_lists = graph.first;
            start_vertex = graph.second;
            fib_heap_time += GetTimeOperation<FibonacciHeap>(edges_lists, start_vertex);
            priority_queue_time += GetTimeOperation<PriorityQueue>(edges_lists, start_vertex);
        }

        fib_heap_time = (fib_heap_time / kMillion / count_iteration);
        priority_queue_time = (priority_queue_time / kMillion / count_iteration);

        double count_vertex_log = (int)log10(count_vertex);
        double count_edges_log = (int)log10(count_edges);
        fib_heap_operations_time_f << fib_heap_time << endl;
        embedded_queue_operations_time_f << priority_queue_time << endl;

        cout << "Test " << TestNumber << ":\n";
        cout << "n: " << (int)(count_vertex / pow(10, count_vertex_log)) << "*10^" << count_vertex_log << " ";
        cout << "m: " << (int)(count_edges/ pow(10, count_edges_log)) << "*10^" << count_edges_log << endl;
        cout << "Fib heap:       " << fib_heap_time << endl;
        cout << "Priority Queue: " << priority_queue_time << endl;
        cout << "Delay percent: " << fib_heap_time / priority_queue_time * 100 - 100 << endl;
    }
    counts_vertex_f.close();
    embedded_queue_operations_time_f.close();
    fib_heap_operations_time_f.close();
}

int main() {
    srand(time(nullptr));
    TimeComparison();
    return 0;
}