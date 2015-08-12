#pragma GCC diagnostic ignored "-Wwrite-strings"
#ifndef ah_cll_h
#define ah_cll_h

/*                        AH Circular Linked List Class                       */
/*                             Author: Alex Hagen                             */
/* This is an implementation of a linked list class that is circular for fast
and efficient data storage.  */

template <class T> class cll_node {
public:
  T object;
  cll_node<T>* next;

  cll_node(T,cll_node<T>*);
}

template <class T> cll_node::cll_node(T _object,cll_node<T>* _next){
  object = _object;
  next = _next;
}

template <class T> class ah_cll {
public:
  int length;
  int size;
  cll_node<T>* head;

  ah_cll(T, int);
  ~ah_cll(void);
  void add(T);
  T pop(void);

private:

};

template <class T> ah_cll::ah_cll(int _length){
  // Set the length
  length = _length;
  // initialize the head pointer to some value
  head = NULL;
}

template <class T> ah_cll::~ah_cll(void){

}

template <class T> ah_cll::add(T _object){
  // first, allocate memory for the object
  cll_node<T>* node = (cll_node<T>*)malloc(sizeof(cll_node<T>));
  // Then, set the object and the pointer into the node
  node->object = _object;
  // then, check if our length is over size
  if (size + 1 > length){
    // find the pointer from the first node
    cll_node<T>* _next_head = head->next;
    // delete the head node
    free(head);
    // connect the list to the "nexthead"
    node->next = _next_head;
    head = _next_head;
  } else {
    size = size + 1;
    // just add the node
    // if the list is only size one, link to itself
    if (head == NULL) {
      node->next = node;
      head = node;
    } else {
      node->next = head;
    }
  }
}



template <class T> T ah_cll:pop(void){
  return x;
}

#endif
