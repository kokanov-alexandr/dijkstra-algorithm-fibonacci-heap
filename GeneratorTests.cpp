#include <random>
#include <vector>
using namespace std;

random_device r;
default_random_engine e1(r());

int GetRandomNum(int l_bound, int r_bound) {
    uniform_int_distribution<int> uniform_dist(l_bound, r_bound - 1);
    return uniform_dist(e1);
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

vector<GraphStr> GenerateTest(int count_vert, int count_edges, int &start_vertex) {
    start_vertex = GetRandomNum(1, count_vert + 1);
    vector<GraphStr> graph;
    graph.push_back(GraphStr(count_vert, count_vert, start_vertex));
    for (int i = 1; i <= count_edges; ++i) {
        graph.push_back({GetRandomNum(1, count_vert + 1), GetRandomNum(1, count_vert + 1),
                         GetRandomNum(1, 15000)});
    }
    return graph;
}
