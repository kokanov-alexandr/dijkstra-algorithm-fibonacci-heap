#include <fstream>
#include <vector>
#include <iostream>

using namespace std;
const int MAX_DEGREE = 64;

template <class V, class P> class FibonacciHeap;
template <class V, class P> class Node;

enum State {
    LABELED, UNLABELED, SCANNED
};

template <class V, class P>
struct Edge {
    Node<V, P>* end;
    int length;

    Edge(Node<V, P>* end, P length) {
        this->end = end;
        this->length = length;
    }
};

template <class V, class P>
struct Node {
    V value;
    P key;
    int degree;
    bool mark;
    Node<V, P>* parent;
    Node<V, P>* left;
    Node<V, P>* right;
    Node<V, P>* child;
    State state;
    vector<Edge<V, P>*> neighbours;

    Node(V value, P key = 0) {
        this->value = value;
        this->key = key;
        this->mark = false;
        this->parent = nullptr;
        this->child = nullptr;
        this->left = this->right = this;
        this->degree = 0;
        this->state = UNLABELED;
    }

    void AddNeighbours(Edge<V, P>* edge) {
        neighbours.push_back(edge);
    }
};

template <class V, class P> class FibonacciHeap {
private:
    Node<V, P>* min_;
public:
    FibonacciHeap() {
        min_ = Empty();
    }

    ~FibonacciHeap() {
        DeleteAll(min_);
    }

    void Insert(Node<V, P>* elem) {
        min_ = Merge(min_, elem);
    }

    void Merge(FibonacciHeap& other) {
        min_ = Merge(min_, other.min_);
        other.min_ = Empty();
    }

    bool IsEmpty() {
        return min_ == nullptr;
    }

    V GetMin() {
        return min_->value;
    }

    Node<V, P>* RemoveMinimum() {
        Node<V, P>* old_min = min_;
        min_ = RemoveMinimum(min_);
        return old_min;
    }

    void DecreaseKey(P key, Node<V, P>* element) {
        min_ = DecreaseKey(min_, element, key);
    }

    void DeleteAll(Node<V, P>* heap) {
        if (heap != nullptr) {
            Node<V, P>* local_heap = heap;
            do {
                auto n = local_heap;
                local_heap = local_heap->right;
                DeleteAll(n->child);
                delete n;
            } while (local_heap != heap);
        }
    }

private:
    Node<V, P>* Empty() {
        return nullptr;
    }

    Node<V, P>* Merge(Node<V, P>* first_heap, Node<V, P>* second_heap) {
        if (first_heap == nullptr)
            return second_heap;
        if (second_heap == nullptr)
            return first_heap;
        if (first_heap->key > second_heap->key)
            swap(first_heap, second_heap);
        Node<V, P>* a = first_heap->right;
        Node<V, P>* b = second_heap->left;
        first_heap->right = second_heap;
        second_heap->left = first_heap;
        a->left = b;
        b->right = a;
        return first_heap;
    }

    void AddChild(Node<V, P>* parent, Node<V, P>* child) {
        child->left = child->right = child;
        child->parent = parent;
        parent->degree++;
        parent->child = Merge(parent->child, child);
    }

    void UnMarkAll(Node<V, P>* element) {
        if (element == nullptr)
            return;
        Node<V, P>* local_element = element;
        do {
            local_element->mark = false;
            local_element->parent = nullptr;
            local_element = local_element->right;
        } while (local_element != element);
    }


    Node<V, P>* RemoveMinimum(Node<V, P>* heap) {
        UnMarkAll(heap->child);
        if (heap->right == heap)
            heap = heap->child;
        else {
            heap->right->left = heap->left;
            heap->left->right = heap->right;
            heap = Merge(heap->right, heap->child);
        }
        if (heap == nullptr)
            return heap;

        heap = Consolidate(heap);
        Node<V, P>* min = heap;
        Node<V, P>* start = heap;
        do {
            if (heap->key < min->key)
                min = heap;
            heap = heap->right;
        } while (heap != start);
        return min;
    }

    Node<V, P>* Consolidate(Node<V, P>* heap) {
        vector<Node<V, P>*> trees(MAX_DEGREE);
        while (true) {
            if (trees[heap->degree] != nullptr) {
                Node<V, P>* t = trees[heap->degree];
                if (t == heap)
                    break;
                trees[heap->degree] = nullptr;
                if (heap->key < t->key) {
                    t->left->right = t->right;
                    t->right->left = t->left;
                    AddChild(heap, t);
                }
                else {
                    t->left->right = t->right;
                    t->right->left = t->left;
                    if (heap->right == heap) {
                        t->right = t->left = t;
                        AddChild(t, heap);
                        heap = t;
                    }
                    else {
                        heap->left->right = t;
                        heap->right->left = t;
                        t->right = heap->right;
                        t->left = heap->left;
                        AddChild(t, heap);
                        heap = t;
                    }
                }
                continue;
            }
            else
                trees[heap->degree] = heap;
            heap = heap->right;
        }
        return heap;
    }

    Node<V, P>* Cut(Node<V, P>* heap, Node<V, P>* element) {
        if (element->right == element)
            element->parent->child = nullptr;
        else {
            element->right->left = element->left;
            element->left->right = element->right;
            element->parent->child = element->right;
        }
        element->right = element->left = element;
        element->mark  = false;
        return Merge(heap, element);
    }

    Node<V, P>* DecreaseKey(Node<V, P>* heap, Node<V, P>* element, int key) {
        element->key = key;
        if (element->parent) {
            if (element->key < element->parent->key) {
                heap = Cut(heap, element);
                auto* parent = element->parent;
                element->parent = nullptr;
                while (parent != nullptr && parent->mark) {
                    heap = Cut(heap, parent);
                    element = parent;
                    parent = element->parent;
                    element->parent = nullptr;
                }
                if (parent != nullptr && parent->parent != nullptr)
                    parent->mark = true;
            }
        }
        else if (element->key < heap->key)
            heap = element;
        return heap;
    }
};
