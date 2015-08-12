#ifndef ah_ll_h
#define ah_ll_h

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

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

template <class T> class ll {
public:
  ll();
  void insert(T);
  T pop();
  bool isempty();
  T mean();
  T stdev();
  void destroy();

private:
  node<T>* head;
  node<T>* tail;
};

template <class T> ll<T>::ll(){
  head = (node<T> *)malloc(sizeof(node<T>));
  head->data = 0;
  head->next = NULL;
  tail = head;
}

template <class T> void ll<T>::insert(T data)
{
        tail->next = (node<T> *)malloc(sizeof(node<T>));
        tail = tail->next;
        tail->data = data;
        tail->next = NULL;
}
template <class T> bool ll<T>::isempty(void) {
  if (head == NULL) {
    return true;
  } else {
    return false;
  }
}

template <class T> T ll<T>::pop(void) {
  assert(head != NULL);
  T object;
  node<T>* next = head->next;
  object = head->data;
  free(head);
  head = next;
  return(object);
}

template <class T> T ll<T>::mean(void) {
  node<T>* _next = head->next;
  T sum = 0;
  int counter = 0;
  do {
    sum = sum + _next->data;
    _next = _next->next;
    counter = counter + 1;
  } while (_next != NULL);
  return sum/(counter);
}

template <class T> T ll<T>::stdev(void) {
  node<T>* _next = head->next;
  T _mean = mean();
	T sum = 0;
  int counter = 0;
  do {
    sum = sum + pow(_next->data - _mean,2.0);
    _next = _next->next;
    counter = counter + 1;
  } while (_next != NULL);
  return sqrt((T)1.0/((T)(counter-1))*sum);
}

template <class T> void ll<T>::destroy(void) {
  while (!isempty()) {
    pop();
  }
  head = (node<T> *)malloc(sizeof(node<T>));
  head->data = 0;
  head->next = NULL;
  tail = head;
}

#endif
