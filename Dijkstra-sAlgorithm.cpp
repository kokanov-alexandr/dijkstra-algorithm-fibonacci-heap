#include <vector>
#include <queue>
#include <string>
#include "FibonacciHeap.cpp"
#define INF LONG_LONG_MAX
#define EDGES_LISTS vector<vector<pair<int, int>>>
typedef long long ll;
using namespace std;



class Dijkstra_sAlgorithm {
public:
    static vector<ll> NaiveDijkstra(EDGES_LISTS &edges_lists, int start_vertex) {
        unsigned long long tops_count = edges_lists.size() - 1;
        vector<ll> distance(tops_count + 1, INF);
        vector<bool> used(tops_count + 1, false);
        distance[start_vertex] = 0;
        distance[0] = 0;
        used[0] = true;
        for (int i = 1; i <= tops_count; ++i) {
            int v = 1;
            ll min_length = INF;
            for (int j = 1; j <= tops_count; ++j) {
                if (!used[j] && distance[j] < min_length) {
                    v = j;
                    min_length = distance[j];
                }
            }
            if (distance[v] == INF)
                break;
            used[v] = true;
            for (auto neigh : edges_lists[v]) {
                ll new_dist = distance[v] + neigh.second;
                if (new_dist < distance[neigh.first])
                    distance[neigh.first] = new_dist;
            }
        }
        for (int i = 1; i <= tops_count; ++i) {
            if (distance[i] == INF)
                distance[i] = -1;
        }
        return distance;
    }

    static vector<ll> DijkstraPriorityQueue(EDGES_LISTS &edges_lists, int start_vertex) {
        unsigned int tops_count = edges_lists.size() - 1;
        vector<ll> distance(tops_count + 1, INF);
        vector<bool> processed(tops_count + 1, false);
        distance[start_vertex] = 0;
        processed[0] = true;
        priority_queue<pair<int, int>> q;
        q.push({0, start_vertex});
        while (!q.empty()) {
            int a = q.top().second;
            q.pop();
            if (processed[a])
                continue;
            processed[a] = true;
            for (auto u: edges_lists[a]) {
                ll b = u.first, w = u.second;
                ll new_dist = distance[a] + w;
                if (new_dist < distance[b]) {
                    distance[b] = new_dist;
                    q.push({-distance[b], b});
                }
            }
        }
        for (int i = 1; i < tops_count + 1; ++i) {
            if (distance[i] == INF)
                distance[i] = -1;
        }
        return distance;
    }

    static vector<Node<int, ll>*> IntToNode(EDGES_LISTS &edges_lists) {
        unsigned long long tops_count = edges_lists.size() - 1;
        vector<Node<int, ll>*> vertices(tops_count + 1);
        for (int i = 1; i <= tops_count; ++i) {
            vertices[i] = (new Node<int, ll>(i, INF));
        }
        for (int i = 1; i <= tops_count; ++i) {
            for (int j = 0; j < edges_lists[i].size(); ++j) {
                auto* edge = new Edge<int, ll>(vertices[edges_lists[i][j].first], edges_lists[i][j].second);
                vertices[i]->AddNeighbours(edge);
            }
        }
        return vertices;
    }

    static vector<ll> DijkstraFibHeap(vector<Node<int, ll>*> &vertices, int x) {
        vertices[x]->state = LABELED;
        auto* heap = new FibonacciHeap<int, ll>();
        vertices[x]->key = 0;
        heap->Insert(vertices[x]);

        while (!heap->IsEmpty()) {
            auto* v = heap->RemoveMinimum();
            v->state = SCANNED;
            for(int i = 0; i < v->neighbours.size(); i++) {
                auto* edge = v->neighbours[i];
                auto* u = edge->end;
                ll NewKey = v->key + edge->length;
                if (u->state == UNLABELED) {
                    u->state = LABELED;
                    u->key = v->key + edge->length;
                    heap->Insert(u);
                }
                else if (u->key > NewKey) {
                    heap->DecreaseKey(NewKey, u);
                }
            }
        }
        vector<ll> distance = vector<ll>(vertices.size() + 1);
        for (int i = 1; i < vertices.size(); ++i) {
             distance[i] = vertices[i]->key == INF ? -1 : vertices[i]->key;
        }
        delete heap;
        return distance;
    }
};

