#ifndef FIB_HPP

template <class T>
Node<T>::Node(T key) {
    this->key = key;
    this->is_descendant_lost = false;
    this->parent = nullptr;
    this->child = nullptr;
    this->left = this->right = this;
    this->degree = 0;
}

template <class T>
FibonacciHeap<T>::FibonacciHeap() {
    min_ = Empty();
}
template <class T>
FibonacciHeap<T>::~FibonacciHeap() {
    if(min_) {
        DeleteAll(min_);
    }
}

template <class T>
Node<T>* FibonacciHeap<T>::Insert(T key) {
    Node<T>* elem = new Node<T>(key);
    min_ = Merge(min_, elem);
    return min_;
}

template <class T>
void FibonacciHeap<T>::Merge(FibonacciHeap& other) {
    min_= Merge(min_, other.min_);
    other.min_= Empty();
}

template <class T>
bool FibonacciHeap<T>::IsEmpty() {
    return min_ == nullptr;
}

template <class T>
void FibonacciHeap<T>::RemoveMinimum() {
    Node<T>* last_min = min_;
    min_ = RemoveMinimum(min_);
    delete last_min;
}

template <class T>
void FibonacciHeap<T>::DecreaseKey(Node<T>* n, int value) {
    min_ = DecreaseKey(min_, n, value);
}

template <class T>
Node<T>*  FibonacciHeap<T>::Find(int key) {
    return Find(min_, key);
}

template <class T>
Node<T>* FibonacciHeap<T>::Empty() {
    return nullptr;
}

template <class T>
T FibonacciHeap<T>::GetMinimum() {
    return min_->key;
}

template <class T>
Node<T>* FibonacciHeap<T>::Merge(Node<T>* first_heap, Node<T>* second_heap) {
    if (first_heap == nullptr) {
        return second_heap;
    }
    if (second_heap == nullptr) {
        return first_heap;
    }
    if (first_heap->key > second_heap->key) {
        swap(first_heap, second_heap);
    }
    Node<T>* a = first_heap->right;
    Node<T>* b = second_heap->left;
    first_heap->right = second_heap;
    second_heap->left = first_heap;
    a->left = b;
    b->right = a;
    return first_heap;
}

template <class T>
void FibonacciHeap<T>::DeleteAll(Node<T>* heap) {
    if (heap != nullptr) {
        Node<T>* start = heap;
        do {
            Node<T>* n = start;
            start = start->right;
            DeleteAll(n->child);
            delete n;
        } while(start != heap);
    }
}

template <class T>
void FibonacciHeap<T>::AddChild(Node<T>* parent, Node<T>* child) {
    child->left = child->right = child;
    child->parent = parent;
    parent->degree++;
    parent->child= Merge(parent->child, child);
}

template <class T>
void FibonacciHeap<T>::UnMarkAll(Node<T>* element) {
    if (element == nullptr) {
        return;
    }
    Node<T>* start = element;
    do {
        start->is_descendant_lost = false;
        start->parent = nullptr;
        start = start->right;
    } while (start != element);
}

template <class T>
Node<T>* FibonacciHeap<T>::Consolidate(Node<T>* heap) {
    vector<Node<T>*> trees(kMaxDegree);
    while (true) {
        if (trees[heap->degree] != nullptr) {
            Node<T>* t = trees[heap->degree];
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

template <class T>
Node<T>* FibonacciHeap<T>::RemoveMinimum(Node<T>* heap) {
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
    Node<T>* min = heap;
    do {
        if (heap->key < min->key) {
            min = heap;
        }
        heap = heap->right;
    } while (heap != min);
    return min;
}

template <class T>
Node<T>* FibonacciHeap<T>::Cut(Node<T>* heap, Node<T>* n) {
    if (n->right == n) {
        n->parent->child = nullptr;
    }
    else {
        n->right->left = n->left;
        n->left->right = n->right;
        n->parent->child = n->right;
    }
    n->right = n->left = n;
    n->is_descendant_lost = false;
    n->parent->degree--;
    return Merge(heap, n);
}

template <class T>
Node<T>* FibonacciHeap<T>::DecreaseKey(Node<T>* heap, Node<T>* element, int key) {
    if (element->key < key) {
        return heap;
    }
    element->key = key;
    Node<T>* parent = element->parent;
    if (parent != nullptr && element->key < parent->key) {
        heap = Cut(heap, element);
        element->parent = nullptr;
        while (parent != nullptr && parent->is_descendant_lost) {
            heap = Cut(heap, parent);
            element = parent;
            parent = element->parent;
            element->parent = nullptr;
        }
        if (parent != nullptr && parent->parent != nullptr) {
            parent->is_descendant_lost = true;
        }
        if (element->key < heap->key) {
            heap = element;
        }
    }
    else if (element->key < heap->key) {
        heap = element;
    }
    return heap;
}

template <class T>
Node<T>* FibonacciHeap<T>::Find(Node<T>* heap, int key) {
    auto start = heap;
    if (heap == nullptr) {
        return nullptr;
    }
    do {
        if (start->key == key) {
            return start;
        }
        Node<T>* u = Find(start->child, key);
        if (u) {
            return u;
        }
        start = start->right;
    } while (start != heap);
    return nullptr;
}
#endif