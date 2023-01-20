/*
 * Этот заголовочный файл содержит библиотеку C++ с реализацией алгоритма Дейкстры
 * с использование 3-ч различных структур даннх: vector c++, priority_queue c++,
 * FibonacciHeap из заголовочного файла "fibonacci_heap.h"
 *
 * Запуск алгоритма:
 * Dijkstra<VectorQueue>(edges_lists, start_vertex);
 * Dijkstra<PriorityQueue>(edges_lists, start_vertex);
 * Dijkstra<FibonacciHeap>(edges_lists, start_vertex);
*/

#include <vector>
#include <queue>
#include <algorithm>
#include "fibonacci_heap.h"

#define INF INT_MAX

using EDGES_LISTS = vector<vector<pair<int, int>>>;

template <class T>
struct PriorityQueue {
    priority_queue<T, vector<T>, greater<T>> my_queue;
    void Insert(T t) {
        my_queue.push(t);
    }
    T RemoveMinimum() {
        auto m = my_queue.top();
        my_queue.pop();
        return m;
    }
    bool IsEmpty() {
        return my_queue.size() == 0;
    }

    T GetMinimum() {
        return my_queue.top();
    }
};



template<template<class> class T>
vector<int> Dijkstra(EDGES_LISTS &edges_lists, int start_vertex) {
    int n = edges_lists.size();
    vector<int> distance(n, INF);
    vector<bool> used(n, false);

    T<pair<int, int> > heap;
    distance[start_vertex] = 0;
    heap.Insert(make_pair(0, start_vertex));

    while (!heap.IsEmpty()) {
        int v = heap.GetMinimum().second;
        heap.RemoveMinimum();
        if (used[v]) {
            continue;
        }
        used[v] = true;
        for (auto i: edges_lists[v]) {
            int u = i.first, d = i.second;
            int new_dist = distance[v] + d;
            if (new_dist < distance[u]) {
                distance[u] = new_dist;
                heap.Insert({distance[u], u});
            }
        }
    }
    for (int i = 0; i < distance.size(); ++i) {
        if (distance[i] == INF) {
            distance[i] = -1;
        }
    }
    return distance;
}
