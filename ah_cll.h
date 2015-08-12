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
  void* next;
  void* last;

  cll_node(T,void*,void*);
}

template <class T> cll_node::cll_node(T _object,void* _next,void* _last){
  object = _object;
  next = _next;
  last = _last;
}

template <class T> class ah_cll {
public:
  int length;
  void* first;

  ah_cll(T, int);
  void add(T);
  void clear(void);
  T pop(void);

private:

};

template <class T> ah_cll::ah_cll(T x,int length){
  // First, allocate a place for the object
  first = malloc(sizeof(cll_node))
}

template <class T> void ah_cll::ah_cll(T x){

}

template <class T> void ah_cll::clear(void){

}

template <class T> T ah_cll:pop(void){
  return x;
}

#endif
