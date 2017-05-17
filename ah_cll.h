#ifndef ah_cll_h
#define ah_cll_h

#include <stdlib.h>
#include <stdio.h>

/*!
 * Node is an implementation of a storage node for a circular linked list.
 */
template <class T> class node {
public:
  /** Initializes a node
   * @_data the data that will be added to the node
   * @_next the pointer of the next node
   *
   * initializes the node with the pointer to the next node
   */
  node(T _data,node<T>* _next);
  T data; /*!< Detailed description after the member */
  node<T>* next; /*!< Detailed description after the member */
};

template <class T> node<T>::node(T _data, node<T>* _next){
  data=_data;
  next=_next;
}

/** cll class
 *
 * cll is a circular linked list
 */
template <class T> class cll {
public:
  /*!
   * Initializes a circular linked list
   * initializes the linked list with data object type of length _length
   */
  cll(int _length);
  /*! 
      \brief Inserts a datum into the linked list
      \see cll()
      \param datum\ - the datum
      \return none
   */
  void insert(T datum);
  T pop();
  int length;
  int size;

private:
  node<T>* head;
  node<T>* tail;
};

template <class T> cll<T>::cll(int _length){
  length = _length;
  size = 1;
  head = (node<T> *)malloc(sizeof(node<T>));
  head->data = 0;
  tail = head;
  tail->next = head;
}

template <class T> void cll<T>::insert(T datum){
        size = size + 1;
        tail->next = (node<T> *)malloc(sizeof(node<T>));
        tail = tail->next;
        tail->data = datum;
        tail->next = head;
        if (size > length) {
          tail->next = head->next;
          free(head);
          head = tail->next;
          size = size - 1;
        }
}

template <class T> T cll<T>::pop(void) {
  T object;
  if (size > 0){
    object = head->data;
    tail->next = head->next;
    free(head);
    head = tail->next;
    size = size - 1;
  } else {
    exit(-1);
  }
  return(object);
}

#endif
