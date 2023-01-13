#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include "fibonacci_heap.cpp"

#define INF INT_MAX
#define EDGES_LISTS vector<vector<pair<int, int>>>

typedef long long ll;
using namespace std;

struct VectorQueue {
    vector<pair<int, int>> my_vector;
    void Insert(pair<int, int> t) {
        my_vector.push_back(t);
    }
    pair<int, int> RemoveMinimum() {
        auto m = min_element(my_vector.begin(), my_vector.end());
        auto v = *m;
        my_vector.erase(m);
        return v;
    }
    bool IsEmpty() {
        return my_vector.size() == 0;
    }
};

struct PriorityQueue {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> my_queue;
    void Insert(pair<int, int> t) {
        my_queue.push(t);
    }
    pair<int, int> RemoveMinimum() {
        auto m = my_queue.top();
        my_queue.pop();
        return m;
    }
    bool IsEmpty() {
        return my_queue.size() == 0;
    }
};

class DijkstraAlgorithm {
public:
    template <class S>
    static vector<ll> Dijkstra(S my_struct, EDGES_LISTS &edges_lists, int start_vertex) {
        unsigned int tops_count = edges_lists.size() - 1;
        vector<ll> distance(tops_count + 1, INF);
        vector<bool> processed(tops_count + 1, false);
        distance[start_vertex] = 0;
        processed[0] = true;

        my_struct.Insert({0, start_vertex});
        while (!my_struct.IsEmpty()) {
            int a = my_struct.RemoveMinimum().second;
            if (processed[a])
                continue;
            processed[a] = true;
            for (auto i: edges_lists[a]) {
                ll u = i.first, d = i.second;
                ll new_dist = distance[a] + d;
                if (new_dist < distance[u]) {
                    distance[u] = new_dist;
                    my_struct.Insert({distance[u], u});
                }
            }
        }
        for (int i = 1; i < distance.size(); ++i) {
            if (distance[i] == INF)
                distance[i] = -1;
        }
        return distance;
    }


    static vector<Node*> IntToNode(EDGES_LISTS &edges_lists) {
        unsigned long long tops_count = edges_lists.size() - 1;
        vector<Node*> vertices(tops_count + 1);
        for (int i = 1; i <= tops_count; ++i) {
            vertices[i] = (new Node(i, INF));
        }
        for (int i = 1; i <= tops_count; ++i) {
            for (int j = 0; j < edges_lists[i].size(); ++j) {
                Edge* edge = new Edge(vertices[edges_lists[i][j].first], edges_lists[i][j].second);
                vertices[i]->AddNeighbours(edge);
            }
        }
        return vertices;
    }

    static vector<ll> DijkstraFibHeap(vector<Node*> &vertices, int x) {
        vertices[x]->state = met;
        auto heap = new FibonacciHeap();
        vertices[x]->key = 0;
        heap->Insert(vertices[x]);

        while (!heap->IsEmpty()) {
            auto v = heap->RemoveMinimum();
            v->state = processed;
            for (int i = 0; i < v->neighbours.size(); i++) {
                auto edge = v->neighbours[i];
                auto u = edge->end;
                ll NewKey = v->key + edge->length;
                if (u->state == not_met) {
                    u->state = met;
                    u->key = v->key + edge->length;
                    heap->Insert(u);
                }
                else if (u->key > NewKey) {
                    heap->DecreaseKey(u, NewKey);
                }
            }
        }
        vector<ll> distance = vector<ll>(vertices.size());
        for (int i = 1; i < vertices.size(); ++i) {
             distance[i] = vertices[i]->key == INF ? -1 : vertices[i]->key;
        }
        delete heap;
        return distance;
    }
};

