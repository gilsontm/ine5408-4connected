#include <cstdint>
#include <stdexcept>  // C++ Exceptions

namespace structures {

//! Classe pilha encadeada (dinâmica).
template<typename T>
class LinkedStack {
 public:
    //! Construtor da pilha.
    LinkedStack();

    //! Destrutor da pilha.
    ~LinkedStack();

    //! Esvazia a pilha, removendo todos os elementos.
    void clear();

    //! Adiciona um elemento ao topo da pilha.
    void push(const T& data);

    //! Remove e retorna (desempilha) o elemento no topo da pilha.
    T pop();

    //! Retorna o elemento no topo da pilha, sem removê-lo.
    T& top() const;

    //! Retorna true se a pilha estiver vazia, else caso contrário.
    bool empty() const;

    //! Retorna o tamanho da pilha.
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

    Node* top_{nullptr};  // nodo-topo
    std::size_t size_{0u};  // tamanho
};

}  // namespace structures