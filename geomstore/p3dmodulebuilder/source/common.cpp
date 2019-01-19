/**
 * 
 * Global vars and functions
 *  
 */ 


#include "common.h"

using namespace std;


/**
 * Least Common Multiplier
 */
int lcm(int x, int y)
{
  int a;
  a = (x > y) ? x : y;
  while (true)
  {
    if (a % x == 0 && a % y == 0)
      return a;
    a++;
  }
}


/**
 * Least Common Multiplier out of a vector<int>
 */
int lcm_vect(vector<int>& v) {
  int a = *max_element(v.begin(), v.end());
  while (true) {
    bool done = true;
    for (const int& i : v) {
      if (a % i != 0) {
        done = false;
        break;
      }
    }
    if (done) {
      ostringstream vts;
      vts << "LCM of (";
      copy(v.begin(), v.end() - 1, ostream_iterator<int>(vts, ", "));
      vts << v.back() << ") is " << a;
      geomstore_cat->warning() << vts.str() << endl;
      return a;
    }
    a++;
  }
}

/**
 * Greates Common Divisor
 */
int gcd(int x, int y)
{
  int r = 0, a, b;
  a = (x > y) ? x : y;
  b = (x < y) ? x : y;

  r = b;
  while (a % b != 0)
  {
      r = a % b;
      a = b;
      b = r;
  }
  return r;
}


/**
 * Calculates the approximate perimeter of an ellipse by doing "Series 2" as 
 * described on https://www.mathsisfun.com/geometry/ellipse-perimeter.html with
 * 7 terms.
 */
float ellipse_perimeter(float a, float b) {
  float sum = 1.0f;
  float hpow = 1.0f;
  const float factors[6] = {
    0.25f, 
    0.015625f, 
    0.00390625f, 
    0.00152587890625f, 
    0.0007476806640625f, 
    0.00042057037353515625f
  };
  float term;
  float h = pow((a - b) / (a + b), 2.0f);

  for (int i = 0; i < 6; i++) {
    hpow *= h;
    term = factors[i] * hpow;
    sum += term;
  }
  sum *= PI * (a + b);
  return sum;
}