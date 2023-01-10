#include <random>
#include <vector>
using namespace std;

random_device rd;
default_random_engine e(rd());
static std::mt19937 gen{
        static_cast<unsigned int>(
                std::chrono::system_clock::now().time_since_epoch().count())};


int GetNumber(int left_bound, int right_bound) {
    uniform_int_distribution<int> dis(left_bound, right_bound - 1);
    return dis(e);
}

int GetRandomNumber(int left_bound, int right_bound) {
    uniform_int_distribution<int> dis(left_bound, right_bound - 1);
    return dis(gen);
}

struct GraphStr {
    int head;
    int tail;
    int length;
    GraphStr(int head, int tail, int length) {
        this->head = head;
        this->tail = tail;
        this->length = length;
    }
};

vector<GraphStr> GenerateTest(int count_vert, int count_edges, int &start_vertex, bool is_test_random) {
    int (*GetterNum)(int, int);
    if (is_test_random)
        GetterNum = GetRandomNumber;
    else GetterNum = GetNumber;

    start_vertex = GetterNum(1, count_vert + 1);
    vector<GraphStr> graph;
    graph.push_back(GraphStr(count_vert, count_edges, start_vertex));
    for (int i = 1; i <= count_edges; ++i) {
        graph.push_back({GetterNum(1, count_vert + 1), GetterNum(1, count_vert + 1),
                         GetterNum(1, 15000)});
    }
    return graph;
}



