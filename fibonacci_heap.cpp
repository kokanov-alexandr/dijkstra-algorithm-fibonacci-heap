#include <vector>
#include "fibonacci_heap.h"

typedef long long ll;
using namespace std;
using namespace std::chrono;
const int kMaxDegree = 64;

Edge::Edge(Node* end, int length) {
    this->end = end;
    this->length = length;
}


Node::Node(ll value, int key) {
    this->value = value;
    this->key = key;
    this->marked = false;
    this->parent = nullptr;
    this->child = nullptr;
    this->left = this->right = this;
    this->degree = 0;
    this->state = not_met;
}

void Node::AddNeighbours(Edge * edge) {
    neighbours.push_back(edge);
}

FibonacciHeap::FibonacciHeap() {
    min_ = Empty();
}

FibonacciHeap::~FibonacciHeap() {
    if(min_) {
        DeleteAll(min_);
    }
}

Node* FibonacciHeap::Insert(Node * elem) {
    min_ = Merge(min_, elem);
    return min_;
}

void FibonacciHeap::Merge(FibonacciHeap& other) {
    min_= Merge(min_, other.min_);
    other.min_= Empty();
}

bool FibonacciHeap::IsEmpty() {
    return min_ == nullptr;
}


Node* FibonacciHeap::RemoveMinimum() {
    Node* old = min_;
    min_ = RemoveMinimum(min_);
    return old;
}

void FibonacciHeap::DecreaseKey(Node* n, ll value) {
    min_ = DecreaseKey(min_, n, value);
}

Node* FibonacciHeap::Empty() {
    return nullptr;
}

Node* FibonacciHeap::Merge(Node* first_heap, Node* second_heap) {
    if (first_heap == nullptr) {
        return second_heap;
    }
    if (second_heap == nullptr) {
        return first_heap;
    }
    if (first_heap->key > second_heap->key) {
        swap(first_heap, second_heap);
    }
    Node* a = first_heap->right;
    Node* b = second_heap->left;
    first_heap->right = second_heap;
    second_heap->left = first_heap;
    a->left = b;
    b->right = a;
    return first_heap;
}

void FibonacciHeap::DeleteAll(Node * heap) {
    if (heap != nullptr) {
        Node* start = heap;
        do {
            Node* n = start;
            start = start->right;
            DeleteAll(n->child);
            delete n;
        } while(start != heap);
    }
}

void FibonacciHeap::AddChild(Node* parent, Node* child) {
    child->left = child->right = child;
    child->parent = parent;
    parent->degree++;
    parent->child= Merge(parent->child, child);
}

void FibonacciHeap::UnMarkAll(Node * element) {
    if (element == nullptr) {
        return;
    }
    Node* start = element;
    do {
        start->marked = false;
        start->parent = nullptr;
        start = start->right;
    } while (start != element);
}

Node* FibonacciHeap::Consolidate(Node* heap) {
    vector<Node*> trees(kMaxDegree);
    while (true) {
        if (trees[heap->degree] != nullptr) {
            Node * t = trees[heap->degree];
            if (t == heap) {
                break;
            }
            trees[heap->degree] = nullptr;
            t->left->right = t->right;
            t->right->left = t->left;
            if (heap->key < t->key) {
                AddChild(heap, t);
            }
            else {
                if (heap->right == heap) {
                    t->right = t->left=t;
                }
                else {
                    heap->left->right = t;
                    heap->right->left = t;
                    t->right = heap->right;
                    t->left = heap->left;
                }
                AddChild(t, heap);
                heap = t;
            }
            continue;
        } else {
            trees[heap->degree] = heap;
        }
        heap = heap->right;
    }
    return heap;
}

Node* FibonacciHeap::RemoveMinimum(Node* heap) {
    UnMarkAll(heap->child);
    if (heap->right == heap) {
        heap = heap->child;
    }
    else {
        heap->right->left = heap->left;
        heap->left->right = heap->right;
        heap = Merge(heap->right, heap->child);
    }
    if (heap == nullptr) {
        return heap;
    }

    heap = Consolidate(heap);
    Node * min = heap;
    do {
        if (heap->key < min->key) {
            min = heap;
        }
        heap = heap->right;
    } while (heap != min);
    return min;
}

Node* FibonacciHeap::Cut(Node * heap, Node * n) {
    if (n->right == n) {
        n->parent->child = nullptr;
    }
    else {
        n->right->left = n->left;
        n->left->right = n->right;
        n->parent->child = n->right;
    }
    n->right = n->left = n;
    n->marked = false;
    n->parent->degree--;
    return Merge(heap, n);
}

Node* FibonacciHeap::DecreaseKey(Node* heap, Node* element, int key) {
    if (element->key < key) {
        return heap;
    }
    element->key = key;
    Node* parent = element->parent;
    if (parent != nullptr && element->key < parent->key) {
        heap = Cut(heap, element);
        element->parent = nullptr;
        while (parent != nullptr && parent->marked) {
            heap = Cut(heap, parent);
            element = parent;
            parent = element->parent;
            element->parent = nullptr;
        }
        if (parent != nullptr && parent->parent != nullptr) {
            parent->marked = true;
        }
        if (element->key < heap->key) {
            heap = element;
        }
    }
    return heap;
}