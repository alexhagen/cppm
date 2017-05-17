#ifndef value_h
#define value_h
#include <stdio.h>
#include <limits>
#include "sigslot.h"

/*!
   \brief a stateful variable class - simplistically
 */
template <class T> class value : public sigslot::has_slots<>
{
  public:
    /*!
       \brief a signal that is triggered when the value changes
     */
  	sigslot::signal0<> changed;
    /*!
       \brief a signal that is triggered when someone changes the trigger level
     */
  	sigslot::signal0<> levelchanged;
    /*!
       \brief a signal triggered when the value goes from below to above the
       trigger level
     */
  	sigslot::signal0<> abovelevel;
    /*!
       \brief a signal triggered when the value goes from above to below the
       trigger level
     */
  	sigslot::signal0<> belowlevel;
    /*!
       \brief a signal triggered when the value goes from not equal to equal the
       trigger level
     */
  	sigslot::signal0<> atlevel;
    /*!
       \brief sets the variable to a certain value
       \param x\
     */
    void set(T x);
    /*!
       \brief sets the variable to a value and silences all signals (for this
       operation)
       \param x\
     */
    void silentset(T x);
    /*!
       \brief returns the value of the variable
       \returns the value of the variable
     */
    T get(void);
    /*!
       \brief sets the trigger threshold
       \param x\
     */
    void setlevel(T x);
    /*!
       \brief returns the value of the trigger threshold
       \returns the value of the trigger threshold
     */
    T getlevel(void);
    /*!
       \brief resets the states of maximum and minimum during the data-logging
       time
     */
    void resetmaxmin(void);
    /*!
       \brief returns the maximum value encountered since the start of the
       program or since the last call to resetmaxmin
       \returns maximum value during data-logging time
     */
    T getmax(void);
    /*!
       \brief returns the minimum value encountered since the start of the
       program or since the last call to resetmaxmin
       \returns minimum value during data-logging time
     */
    T getmin(void);
    /*!
       \brief returns the last value before the last call to set
       \returns value of variable before last call to set
     */
    T getold(void);
  private:
    /*!
       \brief the value of the variable
     */
  	T val;
    /*!
       \brief the trigger level
     */
  	T level;
    /*!
       \brief the current maximum
     */
    T max;
    /*!
       \brief the current minimum
     */
    T min;
    /*!
       \brief the last value
     */
    T old;
};

template <class T> void value<T>::set(T x)
{
  old = val;
  val = x;
	if(x != old){
		changed.emit();}
	if(x > level && (old <= level)){
		abovelevel.emit();}
	else if (x < level && (old >= level)){
		belowlevel.emit();}
	else if (x == level && (old != level)){
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
  old = val;
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

template <class T> T value<T>::getold(void)
{
  return old;
}

#endif
