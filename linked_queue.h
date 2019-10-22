#include <cstdint>
#include <stdexcept>  // C++ Exceptions

namespace structures {

//! Classe fila encadeada (dinâmica).
template<typename T>
class LinkedQueue {
 public:
    //! Construtor da fila.
    LinkedQueue();

    //! Destrutor da fila.
    ~LinkedQueue();

    //! Esvazia a fila, removendo todos os elementos.
    void clear();

    //! Adiciona um elemento ao final da fila.
    void enqueue(const T& data);

    //! Remove e retorna o primeiro elemento da fila.
    T dequeue();

    //! Retorna o primeiro elemento da fila, sem removê-lo.
    T& front() const;

    //! Retorna o último elemento da fila, sem removê-lo.
    T& back() const;

    //! Retorna true se a fila estiver vazia, else caso contrário.
    bool empty() const;

    //! Retorna o tamanho da fila.
    std::size_t size() const;

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
