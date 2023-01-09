#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

#include "dijkstra-s_algorithm.cpp"
#include "generator_tests.cpp"

typedef long long ll;
using namespace std;
using namespace std::chrono;

const int kMillion = 1e6;
const ll kCountMadeCorrectnessTests = 15;
const ll kCountCorrectnessTests = 30;
const ll kCountTimeTests = 10;
ll CorrectTests = 0;
ll FailedTests = 0;

bool CheckAnswer(const vector<ll>& distance, const vector<ll>& answers) {
    for (int i = 1; i < answers.size(); ++i) {
        if (answers[i] != distance[i]) {
            return false;
        }
    }
    return true;
}

void RunTests() {
    vector<int> count_elements_array;
    for (int i = 2; i <= 4; ++i) {
        for (int j = 2; j <= 10; j += 2) {
            count_elements_array.push_back((int)pow(10, i) * j);
        }
    }

    for (int test_num = 1; test_num <= kCountCorrectnessTests; ++test_num) {
        string in_file_name = "../Tests/in" + to_string(test_num) + ".txt";
        string ans_file_name = "../Tests/ans" + to_string(test_num) + ".txt";
        string out_file_name = "../Tests/output.txt";
        int count_vertex, count_edges, first, second, length, start_vertex;

        if (test_num > kCountMadeCorrectnessTests) {
            in_file_name = "../Tests/input.txt";
            int count = count_elements_array[test_num - kCountMadeCorrectnessTests - 1];
            auto graph = GenerateTest(count, count * 3,  start_vertex, false);
            ofstream output(in_file_name);
            for (auto s : graph) {
                output << s.head << " " << s.tail << " " << s.length << endl;
            }
            output.close();
        }

        ifstream input(in_file_name);
        ifstream answer(ans_file_name);
        ofstream output(out_file_name);

        input >> count_vertex >> count_edges >> start_vertex;
        vector<ll> answers(count_vertex + 1, 0);
        EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());

        for (int i = 0; i < count_edges; ++i) {
            input >> first >> second >> length;
            edges_lists[first].push_back({second, length});
        }

        auto node_lists =  DijkstraAlgorithm::IntToNode(edges_lists);
        auto start = high_resolution_clock::now();
        auto distance = DijkstraAlgorithm::DijkstraFibHeap(node_lists, start_vertex);
        auto stop = high_resolution_clock::now();

        for (auto node_list : node_lists) {
            delete node_list;
        }

        for (int i = 1; i < distance.size(); ++i) {
            output << distance[i] << " ";
        }

        for (int i = 1; i < answers.size(); ++i) {
            answer >> answers[i];
        }

        cout << "TEST " << test_num << ": ";
        if (CheckAnswer(distance, answers)) {
            CorrectTests++;
            cout << "OK. ";
            cout << "TIME: " << (double)duration_cast<microseconds>(stop - start).count() / kMillion << " sec" << endl;
        }
        else {
            FailedTests++;
            cout << "ERROR." << endl;
        }

        input.close();
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
    ofstream count_elements_f("../TestsInfo/count_elements_f.txt");
    ofstream embedded_queue_operations_time_f("../TestsInfo/embedded_priority_queue_dij.txt");
    ofstream fib_heap_operations_time_f("../TestsInfo/fib_heap_dij.txt");
    ofstream naive_realization_operations_time_f("../TestsInfo/naive_implement_dij.txt");

    cout << "Time comparison tests:" << endl;

    for (int i = 10; i <= kCountTimeTests; ++i) {

        int start_vertex;
        vector<double> fib_heap_operations_time;
        vector<double> naive_realization_operations_time;
        vector<double> embedded_queue_operations_time;

        int count_elements = (int)pow(10, 6) * i;
        count_elements_f << count_elements << endl;

        int iter = 0;
        while (iter < 10) {
            auto graph = GenerateTest(count_elements, count_elements * 3, start_vertex, true);
            EDGES_LISTS edges_lists(count_elements + 1, vector<pair<int, int>>());

            for (int j = 1; j <= count_elements; ++j) {
                edges_lists[graph[j].head].push_back({graph[j].head, graph[j].length});
            }

            auto node_lists =  DijkstraAlgorithm::IntToNode(edges_lists);
            auto start_1 = high_resolution_clock::now();
            auto distance = DijkstraAlgorithm::DijkstraFibHeap(node_lists, start_vertex);
            auto stop_1 = high_resolution_clock::now();
            auto execution_time = duration_cast<microseconds>(stop_1 - start_1).count();
            fib_heap_operations_time.push_back(execution_time);

            for (auto node_list : node_lists) {
                delete node_list;
            }

            auto start_2 = high_resolution_clock::now();
            distance = DijkstraAlgorithm::NaiveDijkstra(edges_lists, start_vertex);
            auto stop_2 = high_resolution_clock::now();
            execution_time = duration_cast<microseconds>(stop_2 - start_2).count();
            naive_realization_operations_time.push_back(execution_time);

            auto start_3 = high_resolution_clock::now();
            distance = DijkstraAlgorithm::DijkstraPriorityQueue(edges_lists, start_vertex);
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
    TimeComparison();
    return 0;
}