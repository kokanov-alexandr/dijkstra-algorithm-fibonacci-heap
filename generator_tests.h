#include <random>
#include <vector>
using namespace std;
using EDGES_LISTS = vector<vector<pair<int, int>>>;

using namespace std;

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

int GetRandomNumber(int left, int right) {
    return left + rand() % (right - left + 1);
}


vector<GraphStr> GenerateRandomTest(int count_vertex, int count_edges, int &start_vertex) {
    srand(time(nullptr));
    start_vertex = rand() % count_vertex + 1;
    vector<GraphStr> graph;
    graph.push_back(GraphStr(count_vertex, count_edges, start_vertex));
    for (int i = 1; i <= count_edges; ++i) {
        graph.push_back({GetRandomNumber(1, count_vertex), GetRandomNumber(1, count_vertex), 1});
    }
    return graph;
}
//  Полный граф
pair<EDGES_LISTS, int> GenerateTest1() {
    srand(1);
    int count_vertex = 1e2;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i <= count_vertex; ++i) {
        for (int j = 1; j <= count_vertex; ++j) {
            if (i != j) {
                edges_lists[i].push_back({j, GetRandomNumber(1, 100)});
            }
        }
    }
    return {edges_lists, 1};
}

//  Цикл
pair<EDGES_LISTS, int> GenerateTest2() {
    srand(2);
    int count_vertex = 1e1;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_vertex; ++i) {
        edges_lists[i].push_back({i + 1, GetRandomNumber(1, 15000)});
    }
    edges_lists[count_vertex].push_back({1, GetRandomNumber(1, 15000)});
    return {edges_lists, 3};
}

//  Две компоненты связности
pair<EDGES_LISTS, int> GenerateTest3() {
    srand(3);
    int count_vertex = 1e5 * 2, count_edges = 1e5 * 3;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i <= count_edges / 2; ++i) {
        edges_lists[GetRandomNumber(1, count_vertex / 2)].
                push_back({GetRandomNumber(1, count_vertex / 2), GetRandomNumber(1, 15000)});
    }
    for (int i = 1; i <= count_edges / 2; ++i) {
        edges_lists[GetRandomNumber(count_vertex / 2 + 1, count_vertex)].
                push_back({GetRandomNumber(count_vertex / 2 + 1, count_vertex), GetRandomNumber(1, 15000)});
    }
    return {edges_lists, 1};
}

//  Линия
pair<EDGES_LISTS, int> GenerateTest4() {
    srand(4);
    int count_vertex = 1e5 * 5;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_vertex; ++i) {
        edges_lists[i].push_back({i + 1, GetRandomNumber(1, 1000)});
    }
    return {edges_lists, 1};
}

//  Дерево
pair<EDGES_LISTS, int> GenerateTest5() {
    int tree_height = 18;
    srand(5);
    int count_vertex = pow(2, tree_height) - 1;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i <= pow(2, tree_height - 1) - 1; ++i) {
        edges_lists[i].push_back({i + i, GetRandomNumber(1, 15000)});
        edges_lists[i].push_back({i + i + 1, GetRandomNumber(1, 15000)});
    }
    return {edges_lists, 1};
}

//  Возрастающие веса
pair<EDGES_LISTS, int> GenerateTest6() {
    srand(6);
    int count_vertex = 1e5, count_edges = 1e5 * 3, w = 1;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_edges; ++i) {
        edges_lists[GetRandomNumber(1, count_vertex)].push_back({GetRandomNumber(1, count_vertex), w++});
    }
    return {edges_lists, 1};
}

//  Убывающие веса
pair<EDGES_LISTS, int> GenerateTest7() {
    srand(6);
    int count_vertex = 1e6, count_edges = 4, w = count_edges + 1;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_edges; ++i) {
        edges_lists[GetRandomNumber(1, count_vertex)].push_back({GetRandomNumber(1, count_vertex), w--});
    }
    return {edges_lists, 1};
}


//  Все веса по 1e4
pair<EDGES_LISTS, int> GenerateTest8() {
    srand(7);
    int count_vertex = 1e5, count_edges = 1e5 * 3;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_edges; ++i) {
        edges_lists[GetRandomNumber(1, count_vertex)].push_back({GetRandomNumber(1, count_vertex),1e5});
    }
    return {edges_lists, 2};
}
