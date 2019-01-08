/**
 * Common data and functions
 *  
 */ 
#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <vector>

#include "pnotify.h"

#include "triangle.h"
#include "vertex.h"

template<class T> int contains_(const std::vector<T> & c, const T & t) { 
  return std::count(begin(c), end(c), t); 
}
template<class T> int index_of(const std::vector<T> & c, const T & v) { 
  return std::find(begin(c), end(c), v) - begin(c); 
} // Note: Not presently called
template<class T> T &add(std::vector<T> & c, T t) {
  c.push_back(t); 
  return c.back(); 
}
template<class T> T pop(std::vector<T> & c) {
  auto val = std::move(c.back()); 
  c.pop_back(); 
  return val; 
}
template<class T> void add_unique(std::vector<T> & c, T t) { 
  if (!contains_(c, t)) {
    c.push_back(t); 
  }
}
template<class T> void remove(std::vector<T> & c, T t) {
  auto it = std::find(begin(c), end(c), t); 
  nassertv(contains_(c, t)); 
  nassertv(it != end(c)); 
  c.erase(it); 
  nassertv(!contains_(c, t)); 
}

#endif