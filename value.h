#ifndef value_h
#define value_h
#include <stdio.h>
#include <limits>
#include "sigslot.h"

template <class T> class value : public sigslot::has_slots<>
{
  public:
  	sigslot::signal0<> changed;
  	sigslot::signal0<> levelchanged;
  	sigslot::signal0<> abovelevel;
  	sigslot::signal0<> belowlevel;
	//sigslot::signal0<> magabovelevel;
	//sigslot::signal0<> magbelowlevel;
  	sigslot::signal0<> atlevel;
	//sigslot::signal0<> magatlevel;
    void set(T x);
    void silentset(T x);
    T get(void);
    void setlevel(T x);
    T getlevel(void);
    void resetmaxmin(void);
    T getmax(void);
    T getmin(void);
  //private:
  	T val;
  	T level;
    T max;
    T min;
};

template <class T> void value<T>::set(T x)
{
  T y;
  y = val;
  val = x;
	if(x != y){
		changed.emit();}
	if(x > level && (y <= level)){
		abovelevel.emit();}
	else if (x < level && (y >= level)){
		belowlevel.emit();}
	else if (x == level && (y != level)){
		atlevel.emit();}
  if(x > max){
    max = x;
  }
  if(x < min){
    min = x;
  }
	/*
	if ((abs(x) > level) && (abs(y) <= level)){
		magabovelevel.emit();}
	else if ((abs(x) < level) && (abs(y) >= level)){
		magbelowlevel.emit();}
	else if ((abs(x) == level) && (abs(y) != level)){
		magatlevel.emit();}
	*/
}

template <class T> void value<T>::silentset(T x)
{
  val = x;
}

template <class T> T value<T>::get(void)
{
  return val;
}

template <class T> void value<T>::setlevel(T x)
{
  T y;
  y = level;
  level = x;
	if(x != y){
		levelchanged.emit();}
}

template <class T> T value<T>::getlevel(void)
{
  return level;
}

template <class T> void value<T>::resetmaxmin(void)
{
  max = std::numeric_limits<T>::min();
  min = std::numeric_limits<T>::max();
}

template <class T> T value<T>::getmax(void)
{
  return max;
}

template <class T> T value<T>::getmin(void)
{
  return min;
}

#endif
