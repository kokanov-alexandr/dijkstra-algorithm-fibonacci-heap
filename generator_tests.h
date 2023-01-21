#include <random>
#include <vector>
#include <tuple>

using namespace std;
using EDGES_LISTS = vector<vector<pair<int, int>>>;
using test_tuple = tuple<EDGES_LISTS, int, vector<int>>;

int kMaxWeight = 1000;
int GetRandomNum(int left, int right) {
    return left + rand() % (right - left + 1);
}

test_tuple GenTestRandomGraph(int count_vertex) {
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_vertex; ++i) {
        edges_lists[i].push_back({i + 1, GetRandomNum(1, 15000)});
    }
    edges_lists[count_vertex].push_back({1, GetRandomNum(1, 15000)});

    return {edges_lists, 1, Dijkstra<PriorityQueue>(edges_lists, 1)};
}

test_tuple GenTestFullGraph(int count_vertex) {
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i <= count_vertex; ++i) {
        for (int j = 1; j <= count_vertex; ++j) {
            if (i != j) {
                edges_lists[i].push_back({j, GetRandomNum(1, kMaxWeight)});
            }
        }
    }
    return {edges_lists, 1, Dijkstra<PriorityQueue>(edges_lists, 1)};
}

test_tuple GenTestCycleGraph(int count_vertex) {
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_vertex; ++i) {
        edges_lists[i].push_back({i + 1, GetRandomNum(1, kMaxWeight)});
    }
    edges_lists[count_vertex].push_back({1, GetRandomNum(1, kMaxWeight)});

    auto answers = vector<int>(count_vertex + 1, 0);
    for (int i = 2; i <= count_vertex; ++i) {
        answers[i] = answers[i - 1] + edges_lists[i - 1][0].second;
    }
    return {edges_lists, 1, answers};
}

test_tuple GenTestTwoComponentsGraph(int count_vertex) {
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_vertex / 2; ++i) {
        edges_lists[i].push_back({i + 1, GetRandomNum(1, kMaxWeight)});
    }
    edges_lists[count_vertex / 2].push_back({1, GetRandomNum(1, kMaxWeight)});

    for (int i = count_vertex / 2 + 1; i <= count_vertex ; ++i) {
        edges_lists[i].push_back({i + 1, GetRandomNum(1, kMaxWeight)});
    }
    edges_lists[count_vertex].push_back({count_vertex / 2 + 1, GetRandomNum(1, kMaxWeight)});

    auto answers = vector<int>(count_vertex + 1, -1);
    answers[1] = 0;
    for (int i = 2; i <= count_vertex / 2; ++i) {
        answers[i] = answers[i - 1] + edges_lists[i - 1][0].second;
    }
    return {edges_lists, 1, answers};
}

test_tuple GenTestLineGraph(int count_vertex) {
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i < count_vertex; ++i) {
        edges_lists[i].push_back({i + 1, GetRandomNum(1, kMaxWeight)});
    }
    auto answers = vector<int>(count_vertex + 1, 0);
    for (int i = 2; i < count_vertex; ++i) {
        answers[i] = answers[i - 1] + edges_lists[i][0].second;
    }

    return {edges_lists, 1, Dijkstra<PriorityQueue>(edges_lists, 1)};
}

test_tuple GenTestTree(int count_vertex) {
    int border = count_vertex / 2;
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());

    for (int i = 1; i <= border; ++i) {
        edges_lists[i].push_back({i * 2, GetRandomNum(1, kMaxWeight)});
        edges_lists[i].push_back({i * 2 + 1, GetRandomNum(1, kMaxWeight)});
    }
    auto answers = vector<int>(count_vertex + 1, -1);
    answers[1] = 0;
    for (int i = 2; i <= border; ++i) {
        answers[i] = (i % 2 == 0) ? (answers[i / 2] + edges_lists[i / 2][0].second) : (answers[i / 2] + edges_lists[i / 2][1].second);
    }
    return {edges_lists, 1, Dijkstra<PriorityQueue>(edges_lists, 1)};
}

test_tuple GenTestIncWeightsStarGraph(int count_vertex) {
    int start_vertex = count_vertex / 3, w = 1;
    vector<int> answers = vector<int>(count_vertex + 1, 0);
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i <= count_vertex; ++i) {
        edges_lists[start_vertex].push_back({i, w});
        answers[i] = w++;
    }
    answers[start_vertex] = 0;
    return {edges_lists, start_vertex, answers};
}

test_tuple GenTestSameWeightsStarGraph(int count_vertex) {
    int start_vertex = 1;
    vector<int> answers = vector<int>(count_vertex + 1, 0);
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    for (int i = 1; i <= count_vertex; ++i) {
        edges_lists[start_vertex].push_back({i, 1e4});
        answers[i] = 1e4;
    }
    answers[start_vertex] = 0;
    return {edges_lists, start_vertex, answers};
}