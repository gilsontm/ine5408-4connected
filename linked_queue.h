//! Copyright 2019 Gilson Trombetta Magro

#include <cstdint>
#include <stdexcept>  // C++ Exceptions

namespace structures {

//! ...
template<typename T>
class LinkedQueue {
 public:
    //! ...
    LinkedQueue();

    //! ...
    ~LinkedQueue();

    //! ...
    void clear();  // limpar

    //! ...
    void enqueue(const T& data);  // enfilerar

    //! ...
    T dequeue();  // desenfilerar

    //! ...
    T& front() const;  // primeiro dado

    //! ...
    T& back() const;  // último dado

    //! ...
    bool empty() const;  // fila vazia

    //! ...
    std::size_t size() const;  // tamanho

 private:
    class Node {
     public:
        explicit Node(const T& data):
            data_{data}
        {}

        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}

        T& data() {  // getter: dado
            return data_;
        }

        const T& data() const {  // getter const: dado
            return data_;
        }

        Node* next() {  // getter: próximo
            return next_;
        }

        const Node* next() const {  // getter const: próximo
            return next_;
        }

        void next(Node* node) {  // setter: próximo
            next_ = node;
        }

     private:
        T data_;
        Node* next_;
    };

    Node* head_{nullptr};  // nodo-cabeça
    Node* tail_{nullptr};  // nodo-fim
    std::size_t size_{0u};  // tamanho
};
}  // namespace structures
