/**
 * Common data and functions
 *  
 */ 
#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <vector>
#include <algorithm> 
#include <utility> 
#include <iterator>
#include <string>
#include <sstream> 

#include "pta_LVecBase3.h"
#include "pta_LVecBase4.h"


constexpr float TWOPI = 3.14159265358979f * 2.0f;
constexpr float PI = 3.14159265358979f;
constexpr float HALFPI = 3.14159265358979f / 2.0f;

int lcm(int, int);
int lcm_vect(std::vector<int>& v);
int gcd(int, int);
float ellipse_perimeter(float, float);

LQuaternionf get_vector_rotation(LVecBase3f start, LVecBase3f dest);

template<class T> int contains_(const std::vector<T> & c, const T & t) { 
  return count(begin(c), end(c), t); 
}

template<typename T> 
const bool pta_contains(PTA(T) c, T& t) { 
  if (std::find(c.begin(), c.end(), t) != c.end()) {
    return true;
  }
  return false; 
}

template < typename T>
std::pair<bool, int> find_in_pta(const PTA(T) vecOfElements, const T  & element)
{
	std::pair<bool, int> result;
 
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
 
	if (it != vecOfElements.end())
	{
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else
	{
		result.first = false;
		result.second = -1;
	}
 
	return result;
}

template<class T> int index_of(const std::vector<T> & c, const T & v) { 
  return find(begin(c), end(c), v) - begin(c); 
} // Note: Not presently called
template<class T> T &add(std::vector<T> & c, T t) {
  c.push_back(t); 
  return c.back(); 
}
template<class T> T pop(std::vector<T> & c) {
  auto val = move(c.back()); 
  c.pop_back(); 
  return val; 
}
template<class T> void add_unique(std::vector<T> & c, T t) { 
  if (!contains_(c, t)) {
    c.push_back(t); 
  }
}
template<class T> void remove(std::vector<T> & c, T t) {
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

template<class T> std::string vec_str(std::vector<T> & v) {
    std::ostringstream vts;
    vts << "(";
    copy(v.begin(), v.end() - 1, std::ostream_iterator<T>(vts, ", "));
    vts << v.back() << ")";
    return vts.str();
}

template <class T> void print_pta(T _pta, int maxp = 26) {
  int pta_size = _pta.size();
  std::cout << "[";
  for (int i = 0; i < pta_size; i++) {
    if (i < 4 || pta_size < maxp || (pta_size >= maxp && pta_size - i < 5)) {
      if (i != 0) {
        std::cout << " ";
      }
      std::cout << "[" << _pta[i] << "]";
      if (i < pta_size - 1) {
        std::cout << std::endl;
      }
    }
    else if (i == 4 && pta_size >= maxp) {
      std::cout << " ..." << std::endl << " ..." << std::endl 
        << " ..." << std::endl;
    }
  }
  std::cout << "]" << std::endl;
}

#endif