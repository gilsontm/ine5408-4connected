#include "./linked_stack.h"

template<typename T>
structures::LinkedStack<T>::LinkedStack() {
    size_ = 0u;
    top_ = nullptr;
}

template<typename T>
structures::LinkedStack<T>::~LinkedStack() {
    clear();
}

template<typename T>
void structures::LinkedStack<T>::clear() {
    while (!empty()) {
        pop();
    }
    size_ = 0u;
}

template<typename T>
void structures::LinkedStack<T>::push(const T& data) {
    top_ = new Node(data, top_);
    size_++;
}

template<typename T>
T structures::LinkedStack<T>::pop() {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    Node* aux = top_;
    T data = top_->data();
    top_ = top_->next();
    delete aux;
    size_--;
    return data;
}

template<typename T>
T& structures::LinkedStack<T>::top() const {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    return top_->data();
}

template<typename T>
bool structures::LinkedStack<T>::empty() const {
    return size() == 0;
}

template<typename T>
std::size_t structures::LinkedStack<T>::size() const {
    return size_;
}