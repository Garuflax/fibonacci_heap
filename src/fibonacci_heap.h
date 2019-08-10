#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
#include "utils.h"

/**
 * Implementación de min_priority_queue<T> sobre min_fibonacci_heap.
 * Asume de T:
 * - tiene constructor por copia (con complejidad copy(T))
 * - tiene operador < (con complejidad cmp(T)) que define una relación de orden débil
 * - se asume que copy(T), cmp(T), delete(T) tienen complejidad \O(1) para facilitar análisis de complejidad pero no hay problema con que cuesten más
 */
template < typename T >
class fibonacci_heap {
public:
    using value_type = T;
    using size_type = size_t;

    class handle;

    /**
     * @brief Construye heap vacio
     * \complexity{\O(1)}
     */
    fibonacci_heap();

    /**
     * @brief Destructor
     * \complexity{\O(n)}
     *
     *
     */
    ~fibonacci_heap();

    /**
     * @brief Constructor por copia
     * \complexity{\O(n)}
     *
     *
     */
    fibonacci_heap (const fibonacci_heap&);

    /**
     * @brief Operador de asignacion
     * \complexity{\O(n)}
     *
     *
     */
    fibonacci_heap& operator= (const fibonacci_heap&);

    /**
     * @brief Operdador de movimiento
     * \complexity{\O(1)}
     */
    fibonacci_heap (fibonacci_heap&&) noexcept ;

    /**
     * @brief Operdador de asignacion de movimiento
     * \complexity{\O(n)}
     *
     *
     */
    fibonacci_heap& operator= (fibonacci_heap&&) noexcept ;

    /**
     * @brief Remueve todos los elementos
     *
     * \complexity{\O(n)}
     *
     */
    void clear();

    /**
     * @brief Intercambia los elementos de 2 heaps
     * @param h heap a intercambiar
     *
     * \complexity{\O(1)}
     */
    void swap (fibonacci_heap& h);

    /**
     * @brief Indica si el heap esta vacio
     *
     * @returns true \IFF  size() == 0
     *
     * \complexity{\O(1)}
     */
    bool empty() const;

    /**
     * @brief Devuelve cantidad de elementos
     *
     * @returns n
     *
     * \complexity{\O(1)}
     */
    size_type size() const;

    /**
     * @brief Acceso al minimo elemento
     *
     * @returns referencia constante al minimo
     *
     * \complexity{\O(1)}
     */
    const value_type& minimum() const;

    /**
     * @brief Inserción
     *
     * @param val elemento a insertar
     *
     * @returns handle que apunta al elemento insertado
     *
     * \complexity{\O(1)}
     *
     */
    handle insert(const value_type& val);

    /**
     * @brief Remover minimo
     * \complexity{\O(log(n) amortizado)}
     *
     */
    void extract_min();

    /**
     * @brief Eliminar elemento
     * @param x handle que apunta al elemento a eliminar
     *
     * \complexity{\O(log(n) amortizado)}
     *
     */
    void delete_key(handle& x);

    /**
     * @brief Decrementar elemento
     * @param x handle que apunta al elemento a decrementar
     * @param val nuevo valor del elemento
     * \pre  \P{val} < *\P{x}
     * \post *\P{x} == \P{val} \AND el resto de la estructura no cambia
     *
     * \complexity{\O(1) amortizado}
     *
     */
    void decrease_key(const handle &x, const value_type &val);

    /**
     * @brief Une 2 heaps quedando todos los elementos en uno solo
     * @param h heap a unir que queda vacio
     *
     * \complexity{\O(1)}
     *
     */
    void join(fibonacci_heap& h);

private:

    /**
     * Nodo de la estructura:
     * - Apunta al padre (null si no tiene)
     * - Apunta a la lista de hijos
     * - Las listas son doblemente enlazadas
     * - Se guarda un elemento
     * - Se tiene la cantidad de hijos
     * - Se marca si pierde un hijo desde que el nodo se hizo hijo de otro nodo
     */
    struct Node{

        /**
         * @brief crear nodo que representa heap de un elemento
         * @param val clave del nodo a crear
         *
         * \complexity{\O(1)}
         */
        Node(const value_type& val);

        /**
         * @brief unir listas
         * @param n puntero a nodo a unir
         *
         * \complexity{\O(1)}
         */
        void join(Node* n);

        /**
         * @brief Sacar nodo de la lista enlazada en la que esta sin romperla y unirse a si mismo
         *
         * \complexity{\O(1)}
         */
        void remove();

        /**
         * @brief Sacar padre de una lista enlazada
         *
         * \complexity{\O(log(n))}
         */
        void remove_parent();

        /**
         * @brief Agrega hijo al nodo
         * @param n nodo a agregar que es removido de la lista enlazada en la que está
         *
         * \complexity{\O(1)}
         */
        void add_child(Node* n);

        /** @{ */
        Node* parent;
        Node* child;
        Node* left;
        Node* right;
        value_type key;
        unsigned int degree;
        bool mark;
        /** @} */
    };

    /**
     * @brief eliminar todos los nodos de una lista circular y cada uno de sus hijos
     * Deja al heap en un estado inconsistente
     * @param x puntero al nodo donde se empieza
     *
     * \complexity{\O(n)}
     */
    void delete_brothers_and_childs(Node* x);

    /**
     * @brief agregar todos los nodos de una lista circular y cada uno de sus hijos
     * @param x puntero al nodo donde se empieza
     *
     * \complexity{\O(n)}
     */
    void insert_brothers_and_childs(Node* x);

    /**
     * @brief Dejar a la lista de raíces sin raíces con misma cantidad de hijos
     *
     * \complexity{\O(log(n) amortizado)}
     */
    void consolidate();

    /**
     * @brief Poner nodo en la lista de raíces
     * @param x Nodo a mover
     * @param parent Padre del nodo a mover
     *
     * \complexity{\O(1)}
     */
    void cut(Node* x,Node* parent);

    /**
     * @brief Mantener estructura para que no haya nodos que hayan perdido más de 1 nodo
     * @param x Nodo que acaba de perder un hijo
     *
     * \complexity{\O(c)} con c cantidad de llamadas recursivas
     *
     */
    void cascading_cut(Node* x);

    /**
     * @brief Obtener lista de raíces de la estructura
     * @returns Lista de raíces en vector
     *
     * \complexity{\O(t)} con t cantidad de árboles en la lista de raíces
     */
    std::vector<Node*> get_root_list();

    /** @{ */
    Node* min;
    size_type n;
    /** @} */
};

template<typename T>
class fibonacci_heap<T>::handle {
public:
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;

    /**
     * @brief Comparacion entre handles
     *
     * @param other handle a comparar
     *
     * @returns true \IFF apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator==(const handle &other) const;

    /**
     * @brief Comparacion entre handles
     *
     * @param other handle a comparar
     *
     * @returns false \IFF apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator!=(const handle &other) const;

    /**
     * @brief Desreferencia el puntero
     * El valor devuelto tiene aliasing dentro de la coleccion.
     * \pre El handle debe estar apuntando a un elemento.
     * \post El valor resultado es una referencia constante al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    reference operator*() const;

    /**
     * @brief Operador flechita
     *
     * El valor devuelvo tiene aliasing dentro de la coleccion.
     *
     * \pre El handle debe estar apuntando a un elemento.
     * \post El valor resultado es un puntero al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    pointer operator->() const;

private:

    friend class fibonacci_heap;

    /**
     * @brief Constructor
     * @param x puntero al nodo que el handle estará ligado
     *
     * Cuando el elemento sea eliminado no se debe desreferenciar a este handle
     */
    handle(fibonacci_heap<T>::Node* x);

    /** @{ */
    fibonacci_heap<T>::Node* n;
    /** @} */
};

#include "fibonacci_heap.hpp"

#endif //FIBONACCI_HEAP_H
