#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <chrono>

#include "Dijkstra-sAlgorithm.cpp"
#include "GeneratorTests.cpp"

typedef long long ll;
using namespace std;
using namespace std::chrono;

const ll CORRECTNESS_TESTS = 15;
const ll COUNT_TESTS = 30;
ll CORRECT_TESTS = 0;
ll FAILED_TESTS = 0;


bool CheckAnswer(const vector<ll>& distance, const vector<ll>& answers) {
    for (int i = 1; i < answers.size(); ++i) {
        if (answers[i] != distance[i]) {
            return false;
        }
    }
    return true;
}

void RunTests() {
    vector<int> count_elem_arr;
    for (int i = 2; i <= 4; ++i) {
        for (int j = 2; j <= 10; j += 2) {
            count_elem_arr.push_back(pow(10, i) * j);
        }
    }

    for (int test_num = 1; test_num <= COUNT_TESTS; ++test_num) {
        string in_file_name = "../Tests/in" + to_string(test_num) + ".txt";
        string ans_file_name = "../Tests/ans" + to_string(test_num) + ".txt";
        string out_file_name = "../Tests/output.txt";
        int count_vert, count_edges, first, second, length, start_vertex;

        if (test_num > CORRECTNESS_TESTS) {
            in_file_name = "../Tests/input.txt";
            int count = count_elem_arr[test_num - CORRECTNESS_TESTS - 1];
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

        input >> count_vert >> count_edges >> start_vertex;
        vector<ll> answers(count_vert + 1, 0);
        EDGES_LISTS edges_lists(count_vert + 1, vector<pair<int, int>>());

        for (int i = 0; i < count_edges; ++i) {
            input >> first >> second >> length;
            edges_lists[first].push_back({second, length});
        }
        auto node_lists =  Dijkstra_sAlgorithm::IntToNode(edges_lists);
        auto start1 = high_resolution_clock::now();
        auto distance = Dijkstra_sAlgorithm::DijkstraFibHeap(node_lists, start_vertex);
        auto stop1 = high_resolution_clock::now();

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
            CORRECT_TESTS++;
            cout << "OK. ";
            cout << "TIME: "<< duration_cast<microseconds>(stop1 - start1).count() / 1e6 << " sec" << endl;
        }
        else {
            FAILED_TESTS++;
            cout << "ERROR." << endl;
        }

        input.close();
        answer.close();
        output.close();
    }
    cout << "Correct tests: " << CORRECT_TESTS << endl;
    cout << "Failed tests: " << FAILED_TESTS << endl;

}


double GetAverage(vector<double> &operations_time) {
    double average = 0;
    for (double elem: operations_time)
        average += elem;

    return average /= (double)operations_time.size();
}

void TimeComparison() {
    ofstream count_elems("../TestsInfo/count_elements.txt");
    ofstream epq_dij_t("../TestsInfo/embedded_priority_queue_dij.txt");
    ofstream fib_heap_dij_t("../TestsInfo/fib_heap_dij.txt");
    ofstream naive_dij_t("../TestsInfo/naive_implement_dij.txt");


    cout << "Time comparison tests:" << endl;
    vector<int> count_elems_arr;
    for (int i = 1; i <= 10; ++i) {
        count_elems_arr.push_back((int)pow(10, 6) * i);
    }


    for (int i = 0; i < count_elems_arr.size(); ++i) {
        int start_vertex;
        vector<double> operations_time;
        vector<double> operations_time2;
        vector<double> operations_time3;
        int count_elem = count_elems_arr[i];
        count_elems << count_elem << endl;
        int as = 0;
        while (as < 10) {
            as++;

            auto graph = GenerateTest(count_elem, count_elem * 3,  start_vertex, true);
            EDGES_LISTS edges_lists(count_elem + 1, vector<pair<int, int>>());

            for (int i = 1; i <= count_elem; ++i) {
                edges_lists[graph[i].head].push_back({graph[i].head, graph[i].length});
            }

            auto node_lists =  Dijkstra_sAlgorithm::IntToNode(edges_lists);
            auto start1 = high_resolution_clock::now();
            auto distance = Dijkstra_sAlgorithm::DijkstraFibHeap(node_lists, start_vertex);
            auto stop1 = high_resolution_clock::now();
            for (auto node_list : node_lists) {
                delete node_list;
            }
            auto execution_time = duration_cast<microseconds>(stop1 - start1).count();
            operations_time.push_back(execution_time);
            cout << execution_time << endl;

            auto start2 = high_resolution_clock::now();
            distance = Dijkstra_sAlgorithm::NaiveDijkstra(edges_lists, start_vertex);
            auto stop2 = high_resolution_clock::now();
            execution_time = duration_cast<microseconds>(stop2 - start2).count();
            operations_time2.push_back(execution_time);

            auto start3 = high_resolution_clock::now();
            distance = Dijkstra_sAlgorithm::DijkstraPriorityQueue(edges_lists, start_vertex);
            auto stop3 = high_resolution_clock::now();
            execution_time = duration_cast<microseconds>(stop3 - start3).count();
            operations_time3.push_back(execution_time);

        }

        fib_heap_dij_t << GetAverage(operations_time) / 1e6 << endl;
        naive_dij_t << GetAverage(operations_time2) / 1e6 << endl;
        epq_dij_t << GetAverage(operations_time3) / 1e6 << endl;



    }
    count_elems.close();
    epq_dij_t.close();
    fib_heap_dij_t.close();
    naive_dij_t.close();
}

int main() {
    RunTests();

    return 0;
}