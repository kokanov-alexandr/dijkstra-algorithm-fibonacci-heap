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

enum GraphCorrTestType {CYCLE, TWO_COMPONENTS, LINE, TREE, MULTIPLE_EDGES, STAR, LOOPS};
enum GraphTimeTestType {RANDOM, FULL};

test_tuple GenCorrTestGraph(GraphCorrTestType type, int count_vertex) {
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    auto answers = vector<int>(count_vertex + 1, 0);
    int start_vertex = 1;
    switch (type) {
        case CYCLE:
            for (int i = 1; i < count_vertex; ++i) {
                edges_lists[i].push_back({i + 1, GetRandomNum(1, kMaxWeight)});
            }
            edges_lists[count_vertex].push_back({1, GetRandomNum(1, kMaxWeight)});

            for (int i = 2; i <= count_vertex; ++i) {
                answers[i] = answers[i - 1] + edges_lists[i - 1][0].second;
            }
            break;

        case TWO_COMPONENTS:
            answers = vector<int>(count_vertex + 1, -1);
            for (int i = 1; i <= count_vertex; ++i) {
                edges_lists[i].push_back({i + 1, GetRandomNum(1, kMaxWeight)});
            }
            edges_lists[count_vertex / 2].pop_back();
            edges_lists[count_vertex / 2].push_back({1, GetRandomNum(1, kMaxWeight)});
            edges_lists[count_vertex].push_back({count_vertex / 2 + 1, GetRandomNum(1, kMaxWeight)});
            answers[1] = 0;
            for (int i = 2; i <= count_vertex / 2; ++i) {
                answers[i] = answers[i - 1] + edges_lists[i - 1][0].second;
            }
            break;

        case LINE:
            for (int i = 1; i < count_vertex; ++i) {
                edges_lists[i].push_back({i + 1, GetRandomNum(1, kMaxWeight)});
            }
            for (int i = 2; i <= count_vertex; ++i) {
                answers[i] = answers[i - 1] + edges_lists[i - 1][0].second;
            }
            break;

        case STAR:
            start_vertex = count_vertex / 3;
            for (int i = 1; i <= count_vertex; ++i) {
                edges_lists[count_vertex / 3].push_back({i, i});
                answers[i] = i;
            }
            answers[start_vertex] = 0;
            break;

        case MULTIPLE_EDGES:
            for (int i = 1; i < count_vertex; ++i) {
                for (int j = 0; j < 5; ++j) {
                    edges_lists[i].push_back({i + 1, GetRandomNum(1, kMaxWeight)});
                }
            }
            for (int i = 0; i < 5; ++i) {
                edges_lists[count_vertex].push_back({1, GetRandomNum(1, kMaxWeight)});
            }
            for (int i = 2; i <= count_vertex; ++i) {
                answers[i] = answers[i - 1] + min_element(edges_lists[i - 1].begin(), edges_lists[i - 1].end())->second;
            }
            break;

        case LOOPS:
            start_vertex = 5;
            for (int i = 1; i <= count_vertex; ++i) {
                edges_lists[i].push_back({i, GetRandomNum(1, kMaxWeight)});
            }
            answers = vector<int>(count_vertex + 1, -1);
            answers[start_vertex] = 0;
            break;

        case TREE:
            int border = count_vertex / 2 - 1;
            answers[start_vertex] = 0;
            for (int i = 1; i <= border; ++i) {
                edges_lists[i].push_back({i * 2, GetRandomNum(1, kMaxWeight)});
                edges_lists[i].push_back({i * 2 + 1, GetRandomNum(1, kMaxWeight)});
                answers[i * 2] = answers[i] + edges_lists[i][0].second;
                answers[i * 2 + 1] = answers[i] + edges_lists[i][1].second;
            }
            border++;
            edges_lists[border].push_back({border * 2, GetRandomNum(1, kMaxWeight)});
            answers[border * 2] = answers[border] + edges_lists[border][0].second;

            if (count_vertex % 2) {
                edges_lists[border].push_back({border * 2 + 1, GetRandomNum(1, kMaxWeight)});
                answers[border * 2 + 1] = answers[border] + edges_lists[border][0].second;
            }
            break;

    }
    return {edges_lists, start_vertex, answers};
}



pair<EDGES_LISTS, int> GenTimeTestGraph(GraphTimeTestType type, int count_vertex) {
    EDGES_LISTS edges_lists(count_vertex + 1, vector<pair<int, int>>());
    int start_vertex = 1;

    switch (type) {
        case RANDOM:
            for (int i = 1; i < count_vertex * 4; ++i) {
                edges_lists[GetRandomNum(1, count_vertex)].push_back({GetRandomNum(1, count_vertex), GetRandomNum(1, 15000)});
            }
            break;

        case FULL:
            for (int i = 1; i <= count_vertex; ++i) {
                for (int j = 1; j <= count_vertex; ++j) {
                    if (i != j) {
                        edges_lists[i].push_back({j, GetRandomNum(1, kMaxWeight)});
                    }
                }
            }
            break;
    }
    return {edges_lists, start_vertex};
}