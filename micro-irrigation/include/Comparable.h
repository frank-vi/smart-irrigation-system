#ifndef COMPARABLE_INTERFACE
#define COMPARABLE_INTERFACE

template <typename T>
class Comparable {
  virtual bool compareTo(T* object) = 0;
};

#endif