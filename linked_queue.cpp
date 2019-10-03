#include "linked_queue.h"

template<typename T>
structures::LinkedQueue<T>::LinkedQueue() {
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0u;
}

template<typename T>
structures::LinkedQueue<T>::~LinkedQueue() {
    clear();
}

template<typename T>
void structures::LinkedQueue<T>::clear() {
    while (!empty()) {
        dequeue();
    }
    size_ = 0u;
}

template<typename T>
void structures::LinkedQueue<T>::enqueue(const T& data) {
    Node* new_node = new Node(data);

    if (size() == 0) {
        head_ = tail_ = new_node;
    } else if (size() == 1) {
        head_->next(new_node);
        tail_ = new_node;
    } else {
        tail_->next(new_node);
        tail_ = tail_->next();
    }
    size_++;
}

template<typename T>
T structures::LinkedQueue<T>::dequeue() {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    T data = head_->data();
    Node* aux = head_;
    head_ = head_->next();

    delete aux;

    size_--;
    if (size_ == 1) {
        tail_ = head_;
    }
    return data;
}

template<typename T>
T& structures::LinkedQueue<T>::front() const {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    return head_->data();
}

template<typename T>
T& structures::LinkedQueue<T>::back() const {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    return tail_->data();
}

template<typename T>
bool structures::LinkedQueue<T>::empty() const {
    return size() == 0;
}

template<typename T>
std::size_t structures::LinkedQueue<T>::size() const {
    return size_;
}
