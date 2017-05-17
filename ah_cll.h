#ifndef ah_cll_h
#define ah_cll_h

#include <stdlib.h>
#include <stdio.h>

/*!
   \brief an implementation of a storage node for a circular linked list.
 */
template <class T> class node {
public:
  /*!
     \brief initializes a node or datum to be put into the linked list
   */
  node(T _data, node<T>* _next);
  /*!
     \brief a data type to be put into the datum
   */
  T data;
  /*!
     \brief the pointer to the next node in the circular linked list
   */
  node<T>* next;
};

template <class T> node<T>::node(T _data, node<T>* _next){
  data=_data;
  next=_next;
}

/*!
   \brief a circular linked list
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
  /*!
     \brief returns the last datum and removes it from the list
     \return last datum
   */
  T pop();
  /*!
     \brief the length of each datum
   */
  int length;
  /*!
     \brief the current size of the whole linked list
   */
  int size;

private:
  /*!
     \brief the pointer to the first datum
   */
  node<T>* head;
  /*!
     \brief the pointer to the last datum
   */
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
