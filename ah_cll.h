#ifndef ah_cll_h
#define ah_cll_h

#include <stdlib.h>
#include <stdio.h>

/*                        AH Circular Linked List Class                       */
/*                             Author: Alex Hagen                             */
/* This is an implementation of a linked list class that is circular for fast
and efficient data storage.  */

template <class T> class node {
public:
  node(T,node<T>*);
  T data;
  node<T>* next;
};

template <class T> node<T>::node(T _data, node<T>* _next){
  data=_data;
  next=_next;
}

template <class T> class cll {
public:
  cll(int);
  void insert(T);
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

template <class T> void cll<T>::insert(T data)
{
        size = size + 1;
        tail->next = (node<T> *)malloc(sizeof(node<T>));
        tail = tail->next;
        tail->data = data;
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
