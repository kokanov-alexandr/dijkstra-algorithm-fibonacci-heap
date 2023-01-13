#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <random>
#include <chrono>

#include "dijkstra-s_algorithm.cpp"
#include "generator_tests.h"

typedef long long ll;
using namespace std;
using namespace std::chrono;

const int kMillion = 1e6;
const int kCountMadeCorrectnessTests = 23;
const int kCountCorrectnessTests = 31;
const int kCountTimeTests = 10;
int CorrectTests = 0;
int FailedTests = 0;

bool CheckAnswer(const vector<ll>& distance, const vector<ll>& answers) {
    for (int i = 1; i < distance.size(); ++i) {
        if (answers[i] != distance[i]) {
            return false;
        }
    }
    return true;
}

void RunTests() {
    pair<EDGES_LISTS, int> (*GeneratorTest[])() = {GenerateTest1, GenerateTest2, GenerateTest3, GenerateTest4,
                                                   GenerateTest5, GenerateTest6, GenerateTest7, GenerateTest8};

    for (int test_number = 1; test_number <= kCountCorrectnessTests; ++test_number) {
        int count_vertex = 0, count_edges = 0, first = 0, second = 0, length = 0, start_vertex = 0;
        EDGES_LISTS edges_lists;
        vector<ll> answers;

        string ans_file_name = "../Tests/" + to_string(test_number) + ".ans";
        string out_file_name = "../Tests/output.txt";
        ifstream answer(ans_file_name);
        ofstream output(out_file_name);

        if (test_number > kCountMadeCorrectnessTests) {
            int count = test_number - kCountMadeCorrectnessTests - 1;
            auto graph = GeneratorTest[count]();
            edges_lists = graph.first;
            start_vertex = graph.second;

        }
        else {
            string in_file_name = "../Tests/" + to_string(test_number) + ".in";
            ifstream input(in_file_name);
            input >> count_vertex >> count_edges >> start_vertex;
            edges_lists = EDGES_LISTS(count_vertex + 1, vector<pair<int, int>>());
            for (int i = 0; i < count_edges; ++i) {
                input >> first >> second >> length;
                edges_lists[first].push_back({second, length});
            }
            input.close();
        }

        auto node_lists =  DijkstraAlgorithm::IntToNode(edges_lists);
        auto start = high_resolution_clock::now();
        auto distance = DijkstraAlgorithm::DijkstraFibHeap(node_lists, start_vertex);
        auto stop = high_resolution_clock::now();

        for (auto node_list : node_lists) {
            delete node_list;
        }
        answers = vector<ll>(count_vertex + 1, 0);

        for (int i = 1; i < distance.size(); ++i) {
            output << distance[i] << " ";
        }

        if (test_number > kCountMadeCorrectnessTests) {
            answers = GenerateAnswer(edges_lists, start_vertex);
        }
        else {
            for (int i = 1; i < answers.size(); ++i) {
                answer >> answers[i];
            }
        }

        cout << "TEST " << test_number << ": ";
        if (CheckAnswer(distance, answers)) {
            CorrectTests++;
            cout << "OK. ";
            cout << "TIME: " << (double)duration_cast<microseconds>(stop - start).count() / kMillion << " sec" << endl;
        }
        else {
            FailedTests++;
            cout << "ERROR." << endl;
        }

        answer.close();
        output.close();
    }
    cout << "Correct tests: " << CorrectTests << endl;
    cout << "Failed tests: " << FailedTests << endl;

}

double GetAverage(vector<double> &operations_time) {
    double average = 0;
    for (double elem: operations_time)
        average += elem;
    return average / (double)operations_time.size();
}


void TimeComparison() {
    ofstream count_elements_f("../TestsInfo/count_elements.txt");
    ofstream embedded_queue_operations_time_f("../TestsInfo/embedded_priority_queue_dij.txt");
    ofstream fib_heap_operations_time_f("../TestsInfo/fib_heap_dij.txt");
    ofstream naive_realization_operations_time_f("../TestsInfo/naive_implement_dij.txt");

    cout << "Time comparison tests:" << endl;

    for (int i = 1; i <= kCountTimeTests; ++i) {

        int start_vertex;
        vector<double> fib_heap_operations_time;
        vector<double> naive_realization_operations_time;
        vector<double> embedded_queue_operations_time;

        int count_elements = (int)pow(10, 6) * i;
        count_elements_f << count_elements << endl;

        int iter = 0;
        while (iter < 5) {
            auto graph = GenerateRandomTest(count_elements, count_elements * 3, start_vertex);
            EDGES_LISTS edges_lists(count_elements + 1, vector<pair<int, int>>());

            for (int j = 1; j <= count_elements; ++j) {
                edges_lists[graph[j].head].push_back({graph[j].head, graph[j].length});
            }

            auto Node_lists =  DijkstraAlgorithm::IntToNode(edges_lists);
            auto start_1 = high_resolution_clock::now();
            auto distance = DijkstraAlgorithm::DijkstraFibHeap(Node_lists, start_vertex);
            auto stop_1 = high_resolution_clock::now();
            auto execution_time = duration_cast<microseconds>(stop_1 - start_1).count();
            fib_heap_operations_time.push_back(execution_time);

            for (auto Node_list : Node_lists) {
                delete Node_list;
            }

            auto start_2 = high_resolution_clock::now();
            distance = DijkstraAlgorithm::Dijkstra(VectorQueue(), edges_lists, start_vertex);
            auto stop_2 = high_resolution_clock::now();
            execution_time = duration_cast<microseconds>(stop_2 - start_2).count();
            naive_realization_operations_time.push_back(execution_time);

            auto start_3 = high_resolution_clock::now();
            distance = DijkstraAlgorithm::Dijkstra(PriorityQueue(), edges_lists, start_vertex);
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

bool Compare(pair<int, int> a, pair<int, int> b) {
    return a.second > b.second;
}

int main() {
    RunTests();
    return 0;
}