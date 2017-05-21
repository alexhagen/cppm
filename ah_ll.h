#ifndef ah_ll_h
#define ah_ll_h

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

/*!
   \brief an implementation of a storage node for a linked list.
 */
template <class T> class node {
public:
  /*!
     \brief initializes a node or datum to be put into the linked list
   */
  node(T,node<T>*);
  /*!
     \brief a data type to be put into the datum
   */
  T datum;
  /*!
     \brief the pointer to the next node in the linked list
   */
  node<T>* next;
};

template <class T> node<T>::node(T _datum, node<T>* _next){
  datum=_datum;
  next=_next;
}

/*!
   \brief a linked list
 */
template <class T> class ll {
public:
  /*!
   * Initializes a linked list
   * initializes the linked list with data object type of length _length
   */
  ll();
  /*!
      \brief Inserts a datum into the linked list
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
    \brief returns whether the linked list includes any data
    \return boolean True if empty, False if not
   */
  bool isempty();
  /*!
    \brief the mean of any data in the linked list that can be summed
    \return the mean of the data
   */
  T mean();
  /*!
    \brief the standard deviation of any data in the linekd list that can be
    summed
    \return the standard deviation of the data
   */
  T stdev();
  /*!
    \brief removes all pointers to the linked list and destroys the instance
   */
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
