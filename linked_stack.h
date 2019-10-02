//! Copyright 2019 Gilson Trombetta Magro

#include <cstdint>
#include <stdexcept>  // C++ Exceptions

namespace structures {

//! ...
template<typename T>
class LinkedStack {
 public:
    //! ...
    LinkedStack();

    //! ...
    ~LinkedStack();

    //! ...
    void clear();  // limpa pilha

    //! ...
    void push(const T& data);  // empilha

    //! ...
    T pop();  // desempilha

    //! ...
    T& top() const;  // dado no topo

    //! ...
    bool empty() const;  // pilha vazia

    //! ...
    std::size_t size() const;  // tamanho da pilha

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

    Node* top_{nullptr};  // nodo-topo
    std::size_t size_{0u};  // tamanho
};

}  // namespace structures