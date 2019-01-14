/**
 * Common data and functions
 *  
 */ 
#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm> 
#include <sstream> 
#include <iterator> 

#include "pnotify.h"

#include "config_module.h"
#include "triangle.h"
#include "vertex.h"

using namespace std;

constexpr float PI = 3.14159265358979f;
constexpr float HALFPI = 3.14159265358979f / 2.0f;

int lcm(int, int);
int lcm_vect(vector<int>& v);
int gcd(int, int);
float ellipse_perimeter(float, float);

template<class T> int contains_(const vector<T> & c, const T & t) { 
  return count(begin(c), end(c), t); 
}
template<class T> int index_of(const vector<T> & c, const T & v) { 
  return find(begin(c), end(c), v) - begin(c); 
} // Note: Not presently called
template<class T> T &add(vector<T> & c, T t) {
  c.push_back(t); 
  return c.back(); 
}
template<class T> T pop(vector<T> & c) {
  auto val = move(c.back()); 
  c.pop_back(); 
  return val; 
}
template<class T> void add_unique(vector<T> & c, T t) { 
  if (!contains_(c, t)) {
    c.push_back(t); 
  }
}
template<class T> void remove(vector<T> & c, T t) {
  auto it = find(begin(c), end(c), t); 
  nassertv(contains_(c, t)); 
  nassertv(it != end(c)); 
  c.erase(it); 
  nassertv(!contains_(c, t)); 
}

template<class T> bool vec_almost_equal(T a, T b) {
  T it = a - b;
  T it_max =  (a.compare_to(b) > -1) ? a : b;
  float sum_it = 0.0f;
  float sum_max = 0.0f;
  for (int i = 0; i < it.size(); i++) {
    sum_it += (float) abs(it[i]);
    sum_max += (float) abs(it_max[i]);
  }
  if (sum_it < 0.001f * sum_max) {
    return true;
  }
  return false;
}

template<class T> string vec_str(vector<T> & v) {
    ostringstream vts;
    vts << "(";
    copy(v.begin(), v.end() - 1, ostream_iterator<T>(vts, ", "));
    vts << v.back() << ")";
    return vts.str();
}

#endif