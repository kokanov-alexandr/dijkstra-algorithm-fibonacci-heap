#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

typedef long long ll;
using namespace std;
using namespace std::chrono;

enum State {
    processed, met, not_met
};

class Node;

struct Edge {
    Node* end;
    int length;

    Edge(Node* end, int length);
};

struct Node {
public:
    Node* left;
    Node* right;
    Node* child;
    Node* parent;
    State state;
    ll value;
    double key;
    int degree;
    bool marked;
    vector<Edge*> neighbours;

    void AddNeighbours(Edge * edge);

    Node(ll value, int key);
};

class FibonacciHeap {
protected:
    Node* min_;
public:

    FibonacciHeap();

    ~FibonacciHeap();

    Node * Insert(Node * elem);

    void Merge(FibonacciHeap& other);

    bool IsEmpty();

    Node * RemoveMinimum();

    void DecreaseKey(Node* n, ll value);

public:
    Node* Empty();

    Node* Merge(Node* first_heap, Node* second_heap);

    void DeleteAll(Node * heap);

    void AddChild(Node* parent, Node* child);

    void UnMarkAll(Node * element);

    Node* Consolidate(Node* heap);

    Node* RemoveMinimum(Node* heap);

    Node* Cut(Node * heap, Node * n);

    Node* DecreaseKey(Node* heap, Node* element, int key);
};