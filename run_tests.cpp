#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

#include "dijkstra-s_algorithm.h"
#include "generator_tests.h"

using namespace std;

const int kMillion = 1e6;
const int kCountMadeCorrectnessDijTests = 23;
const int kCountGenCorrectnessDijTests = 8;
const int kCountCorrectnessFibHeapTests = 10;
const int kCountTimeTests = 10;
int CorrectDijkstraTests = 0;
int FailedDijkstraTests = 0;
int CorrectFibHeapTests = 0;
int FailedFibHeapTests = 0;
int TestNumber = 0;

bool CheckAnswer(const vector<int> &our_answers, const vector<int> &right_answers) {
    for (int i = 1; i < right_answers.size(); ++i) {
        if (our_answers[i] != right_answers[i]) {
            return false;
        }
    }
    return true;
}

/*
    Формат тестов:
    insert a b - вставить элемент со значением a и ключом b

    remove_minimum -удалить минимум
    (гарантируется, что куча не пустая)

    decrease_key a b - присвоить элементу под ключом a ключ b
    (гарантируется, что в куче есть элемент с ключом a, и только один)

    get_minimum - вывести ключ минимума
    (гарантируется, что куча не пустая)
*/

void RunFibHeapTests() {
    cout << "Run Fibonacci Heap tests\n";
    for (TestNumber = 1; TestNumber <= kCountCorrectnessFibHeapTests; ++TestNumber) {
        ifstream input("../tests-fib-heap/" + to_string(TestNumber) + ".in");
        ifstream answer("../tests-fib-heap/" + to_string(TestNumber) + ".ans");

        auto fib_heap = new FibonacciHeap<int>();
        string action;
        int value, key, new_key;
        auto heap_answers = vector<int>();
        auto right_answers = vector<int>();

        while (input >> action) {
            if (action == "insert") {
                input >> value >> key;
                fib_heap->Insert(key);
            }
            else if (action == "remove_minimum") {
                 fib_heap->GetMinimum();
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
        input.close();

        int a;
        while (answer >> a) {
            right_answers.push_back(a);
        }

        cout << "Test " << TestNumber << ": ";
        if (CheckAnswer(right_answers, heap_answers)) {
            CorrectFibHeapTests++;
            cout << "ok.\n";
        }
        else {
            cout << "error.\n";
        }
        answer.close();
    }
    cout << "Correct tests: " << CorrectFibHeapTests << endl;
    cout << "Failed tests: " << FailedFibHeapTests << endl << endl;
}

/*
    Формат тестов:
    Первая строка: n - количество вершин, m - количество рёбер, s - стартовая вершина
    m рёбер: начало, конец, вес
*/

void RunDijkstraTest(EDGES_LISTS edges_lists, int start_vertex, vector<int> answers) {
    auto start = high_resolution_clock::now();
    auto distance = Dijkstra<FibonacciHeap>(edges_lists, start_vertex);
    auto stop = high_resolution_clock::now();

    cout << "Test " << TestNumber << ": ";

    if (CheckAnswer(distance, answers)) {
        CorrectDijkstraTests++;
        cout << "ok. ";
        cout << "Time: " << (double)duration_cast<microseconds>(stop - start).count() / kMillion << " sec" << endl;
    }
    else {
        FailedDijkstraTests++;
        cout << "error." << endl;
    }
}

void RunDijkstraTests() {
    cout << "Run Dijkstra tests\n";
    test_tuple (*GeneratorTest[])(int) = {GenTestFullGraph, GenTestCycleGraph, GenTestTwoComponentsGraph,GenTestLineGraph,
                                          GenTestTree, GenTestIncWeightsStarGraph, GenTestSameWeightsStarGraph,GenTestRandomGraph};

    int count_vertex, count_edges, first, second, length, start_vertex;
    EDGES_LISTS edges_lists;
    vector<int> answers;

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
        RunDijkstraTest(edges_lists, start_vertex, answers);
    }

    for (TestNumber = kCountMadeCorrectnessDijTests + 1; TestNumber <= kCountMadeCorrectnessDijTests + kCountGenCorrectnessDijTests; ++TestNumber) {
        int index = TestNumber - kCountMadeCorrectnessDijTests - 1;
        switch (index) {
            case 0:
                count_vertex = 1e3 * 5;
                break;
            case 4:
                count_vertex = 1e5;
                break;
            default:
                count_vertex = 1e6;
        }
        auto test = GeneratorTest[index](count_vertex);
        RunDijkstraTest(get<0>(test), get<1>(test),  get<2>(test));
    }
    cout << endl;
    cout << "Correct tests: " << CorrectDijkstraTests << endl;
    cout << "Failed tests: " << FailedDijkstraTests << endl;

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

    int count_iteration = 15;
    for (TestNumber = 1; TestNumber <= kCountTimeTests; ++TestNumber) {
        int count_vertex, count_edges, start_vertex;
        vector<double> fib_heap_operations_time;
        vector<double> embedded_queue_operations_time;

        count_vertex = (int)pow(10, 6) * TestNumber;
        count_edges = count_vertex;
        counts_vertex_f << count_edges << endl;

        double fib_heap_time = 0, priority_queue_time = 0;

        for (int iter = 0; iter < count_iteration; ++iter) {
            auto graph = GenTestCycleGraph(count_vertex);
            edges_lists = get<0>(graph);
            start_vertex = get<1>(graph);
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
    RunFibHeapTests();
    RunDijkstraTests();
    return 0;
}