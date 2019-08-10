#include "fibonacci_heap.h"

template<typename T>
fibonacci_heap<T>::fibonacci_heap() : min(nullptr), n(0) {}

template<typename T>
fibonacci_heap<T>::~fibonacci_heap() {
    if(!empty()){
        delete_brothers_and_childs(min);
    }
}

template<typename T>
fibonacci_heap<T>::fibonacci_heap(const fibonacci_heap& h) : min(nullptr), n(0) {
    if(!h.empty()){
        insert_brothers_and_childs(h.min);
    }
}

template<typename T>
fibonacci_heap<T>& fibonacci_heap<T>::operator= (const fibonacci_heap& h) {
    clear();
    if(!h.empty()){
        insert_brothers_and_childs(h.min);
    }
}

template<typename T>
fibonacci_heap<T>::fibonacci_heap(fibonacci_heap && h) noexcept : min(h.min), n(h.n) {
    h.min = nullptr;
    h.n = 0;
}

template<typename T>
fibonacci_heap<T>& fibonacci_heap<T>::operator=(fibonacci_heap && h) noexcept {
    clear();
    std::swap(min,h.min);
    std::swap(n,h.n);
}

template<typename T>
void fibonacci_heap<T>::clear() {
    if(!empty()){
        delete_brothers_and_childs(min);
        min = nullptr;
        n = 0;
    }
}

template<typename T>
void fibonacci_heap<T>::swap(fibonacci_heap &h) {
    std::swap(min,h.min);
    std::swap(n,h.n);
}

template<typename T>
bool fibonacci_heap<T>::empty() const {
    return min == nullptr;
}

template<typename T>
typename fibonacci_heap<T>::size_type fibonacci_heap<T>::size() const {
    return n;
}

template<typename T>
const typename fibonacci_heap<T>::value_type &fibonacci_heap<T>::minimum() const {
    return min->key;
}

template<typename T>
typename fibonacci_heap<T>::handle fibonacci_heap<T>::insert(const value_type &val) {
    Node* node = new Node(val);
    if(empty()){
        min = node;
    }else{
        min->join(node);
        if(node->key < min->key){
            min = node;
        }
    }
    ++n;
    return fibonacci_heap<T>::handle(node);
}

template<typename T>
void fibonacci_heap<T>::extract_min() {
    if(!empty()){
        if(min->degree > 0){
            min->child->remove_parent();
            min->join(min->child);
        }
        Node* z = min->right;
        min->remove();
        delete min;
        if(min == z){
            min = nullptr;
        }else{
            min = z;
            consolidate();
        }
        --n;
    }
}

template<typename T>
void fibonacci_heap<T>::delete_key(fibonacci_heap<T>::handle &x) {
    fibonacci_heap<T>::Node* node_to_delete = x.n;
    fibonacci_heap<T>::Node* parent = node_to_delete->parent;
    if(parent != nullptr){
        cut(node_to_delete,parent);
        cascading_cut(parent);
    }
    min = node_to_delete;
    extract_min();
}

template<typename T>
void fibonacci_heap<T>::decrease_key(const fibonacci_heap<T>::handle &x, const value_type &val) {
    fibonacci_heap<T>::Node* decreased_node = x.n;
    assert(val < decreased_node->key);
    decreased_node->key = val;
    fibonacci_heap<T>::Node* y = decreased_node->parent;
    if(y != nullptr && decreased_node->key < y->key){
        cut(decreased_node,y);
        cascading_cut(y);
    }
    if(decreased_node->key < min->key){
        min = decreased_node;
    }
}

template<typename T>
void fibonacci_heap<T>::join(fibonacci_heap &h) {
    if(empty()){
        min = h.min;
    }else if(!h.empty()){
        min->join(h.min);
        if(h.min->key < min->key) {
            min = h.min;
        }
    }
    n += h.n;
    h.min = nullptr;
    h.n = 0;
}

template<typename T>
void fibonacci_heap<T>::delete_brothers_and_childs(fibonacci_heap::Node *x) {
    x->left->right = nullptr;
    while (x != nullptr){
        Node* next = x->right;
        if(x->child != nullptr){
            delete_brothers_and_childs(x->child);
        }
        delete x;
        x = next;
    }
}

template<typename T>
void fibonacci_heap<T>::insert_brothers_and_childs(fibonacci_heap::Node *x) {
    Node* i = x;
    do{
        if(i->child != nullptr){
            insert_brothers_and_childs(i->child);
        }
        insert(i->key);
        i = i->right;
    }while(i != x);
}

template<typename T>
void fibonacci_heap<T>::consolidate() {
    unsigned int max_degree = log2ul(n) + 1;
    std::vector<Node*> a(max_degree, nullptr);
    std::vector<fibonacci_heap::Node *> nodes_to_process = get_root_list();
    for (unsigned int j = 0; j < nodes_to_process.size(); ++j) {
        Node* x = nodes_to_process[j];
        unsigned int d = x->degree;
        while (a[d] != nullptr){
            Node* y = a[d];
            if(y->key < x->key){
                std::swap(x,y);
            }
            x->add_child(y);
            a[d] = nullptr;
            ++d;
        }
        a[d] = x;
    }
    min = nullptr;
    for (unsigned int i = 0; i < a.size(); ++i) {
        if(a[i] != nullptr && (min == nullptr || a[i]->key < min->key)){
            min = a[i];
        }
    }
}

template<typename T>
void fibonacci_heap<T>::cut(fibonacci_heap::Node *x, fibonacci_heap::Node *parent) {
    if(--parent->degree && x == parent->child){
        parent->child = x->right;
    }else if(!parent->degree){
        parent->child = nullptr;
    }
    x->remove();
    min->join(x);
    x->parent = nullptr;
    x->mark = false;
}

template<typename T>
void fibonacci_heap<T>::cascading_cut(fibonacci_heap::Node *x) {
    fibonacci_heap<T>::Node* z = x->parent;
    if(z != nullptr){
        if(x->mark){
            cut(x,z);
            cascading_cut(z);
        }else{
            x->mark = true;
        }
    }
}

template<typename T>
std::vector<typename fibonacci_heap<T>::Node *> fibonacci_heap<T>::get_root_list() {
    std::vector<fibonacci_heap::Node *> res;
    Node* i = min;
    do{
        res.push_back(i);
        i = i->right;
    }while(i != min);
    return res;
}

template<typename T>
fibonacci_heap<T>::Node::Node(const value_type &val) : parent(nullptr), child(nullptr), left(this), right(this), key(val), degree(0), mark(false) {}

template<typename T>
void fibonacci_heap<T>::Node::join(fibonacci_heap::Node *n) {
    Node* right_node = right;
    Node* right_node_other = n->right;
    std::swap(right,n->right);
    std::swap(right_node->left,right_node_other->left);
}

template<typename T>
void fibonacci_heap<T>::Node::remove() {
    left->right = right;
    right->left = left;
    left = this;
    right = this;
}

template<typename T>
void fibonacci_heap<T>::Node::remove_parent() {
    Node* i = this;
    do{
        i->parent = nullptr;
        i = i->right;
    }while(i != this);
}

template<typename T>
void fibonacci_heap<T>::Node::add_child(fibonacci_heap::Node *n) {
    n->remove();
    n->parent = this;
    if(child == nullptr){
        child = n;
    }else{
        child->join(n);
    }
    ++degree;
    n->mark = false;
}

template<typename T>
bool fibonacci_heap<T>::handle::operator==(const fibonacci_heap<T>::handle &other) const {
    return n == other.n;
}

template<typename T>
bool fibonacci_heap<T>::handle::operator!=(const fibonacci_heap<T>::handle &other) const {
    return n != other.n;
}

template<typename T>
const T &fibonacci_heap<T>::handle::operator*() const {
    return n->key;
}

template<typename T>
typename fibonacci_heap<T>::handle::pointer fibonacci_heap<T>::handle::operator->() const {
    return &n->key;
}

template<typename T>
fibonacci_heap<T>::handle::handle(fibonacci_heap<T>::Node *x) : n(x) {}
