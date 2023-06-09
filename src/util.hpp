#pragma once 

#include <cstdio>

#include <initializer_list>
#include <ostream>

namespace edgs {

  template <typename T>
  void copy(T* start, T* end, T* target)
  {
    for (size_t i = 0; i < size_t(end - start); i++)
      target[i] = start[i];
  };

  template <typename T>
  T&& move(T& arg) { return static_cast<T&&>(arg); }

  template <typename T>
  void move(T* start, T* end, T* target)
  {
    for (size_t i = 0; i < size_t(end - start); i++)
      target[i] = move(start[i]);
  };

  template <typename T>
  void swap(T& a, T& b)
  {
    T tmp = move(a);
    a = move(b);
    b = move(tmp);
  }

  template <typename T>
  void fill(T* start, T* end, const T& value)
  {
    for (size_t i = 0; i < size_t(end - start); i++)
      start[i] = value;
  }

  template <typename T>
  void fill_from_initializer_list(const std::initializer_list<T>& c, T* data)
  {
    T* ptr = data;
    for (const T& i : c) {
      *ptr = i; ptr++;
    }
  }

  template <typename T>
  void shift_right(T* start, T* end, const size_t dif)
  {
    for (size_t i = size_t(end - start) - 1 ; i > dif - 1 ; i--)
      start[i] = start[i - dif];
  }

  struct Dummy{

    Dummy() { printf("Dummy is constructed\n"); }
    Dummy(const int a) 
      : a_{a}
    {
      printf("Dummy is constructed\n");
    }

    ~Dummy() {
      printf("Dummy is destructed\n");
    }

    friend std::ostream& operator<< (std::ostream& o, const Dummy& d)
    {
      return o << "a:" << d.a_ ;
    }

    int   a_{5};
  };
}

