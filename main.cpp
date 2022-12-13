#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <ctime>

#include "Dijkstra-sAlgorithm.cpp"
#include "GeneratorTests.cpp"

typedef long long ll;
using namespace std;

ll CORRECTNESS_TESTS = 9;
ll COUNT_TESTS = 16;
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
    for (int test_num = 1; test_num <= COUNT_TESTS; ++test_num) {
        string in_file_name = "../Tests/in" + to_string(test_num) + ".txt";
        string ans_file_name = "../Tests/ans" + to_string(test_num) + ".txt";
        string out_file_name = "../Tests/output.txt";
        int count_vert, count_edges, first, second, length, start_vertex;
        if (test_num > CORRECTNESS_TESTS) {
            in_file_name = "../Tests/input.txt";
            int count = (int)pow(10,  test_num - CORRECTNESS_TESTS);
            auto graph = GenerateTest(count, count,  start_vertex);
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
        clock_t start = clock();
        auto distance = Dijkstra_sAlgorithm::DijkstraFibHeap(node_lists, start_vertex);
        clock_t stop = clock();

        for (int i = 1; i < distance.size(); ++i) {
            output << distance[i] << " ";
        }
        for (int i = 1; i < answers.size(); ++i) {
            answer >> answers[i];
        }

        cout << "Test " << test_num << ": ";
        if (CheckAnswer(distance, answers)) {
            CORRECT_TESTS++;
            cout << "OK. ";
            cout << "Time: "<< (double)(stop - start) / CLOCKS_PER_SEC << " sec" << endl;
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


void TimeComparison() {
    ofstream count_elems("../TestsInfo/count_elements.txt");
    ofstream epq_dij_t("../TestsInfo/embedded_priority_queue_dij.txt");
    ofstream fib_heap_dij_t("../TestsInfo/fib_heap_dij.txt");
    ofstream naive_dij_t("../TestsInfo/naive_implement_dij.txt");

    vector<int> count_elems_arr;
    for (int i = 6; i <= 7; ++i) {
        for (int j = 1; j < 10; ++j) {
            count_elems_arr.push_back((int)pow(10, i) * j);
        }
    }
    for (auto count_elem : count_elems_arr) {
        cout << count_elem << endl;
        int start_vertex;
        auto graph = GenerateTest(count_elem, count_elem,  start_vertex);

        count_elems << count_elem << endl;
        EDGES_LISTS edges_lists(count_elem + 1, vector<pair<int, int>>());
        for (int i = 1; i <= count_elem; ++i) {
            edges_lists[graph[i].head].push_back({graph[i].head, graph[i].length});
        }

        auto node_lists =  Dijkstra_sAlgorithm::IntToNode(edges_lists);
        clock_t start1 = clock();
        auto distance = Dijkstra_sAlgorithm::DijkstraFibHeap(node_lists, start_vertex);
        clock_t stop1 = clock();
        for (auto & node_list : node_lists) {
            delete node_list;
        }
        fib_heap_dij_t <<  (double)(stop1 - start1) / CLOCKS_PER_SEC  << endl;

        clock_t start2 = clock();
        distance = Dijkstra_sAlgorithm::NaiveDijkstra(edges_lists, start_vertex);
        clock_t stop2 = clock();
        naive_dij_t <<  (double)(stop2 - start2) / CLOCKS_PER_SEC  << endl;

        clock_t start3 = clock();
        distance = Dijkstra_sAlgorithm::DijkstraPriorityQueue(edges_lists, start_vertex);
        clock_t stop3 = clock();
        epq_dij_t <<  (double)(stop3 - start3) / CLOCKS_PER_SEC  << endl;


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