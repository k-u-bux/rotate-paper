// witten-sum.cc
// =============

#include <iostream>
#include <future>

long double Z3 = 1.2020569031595942853997381615114499907649862923405l;

double witten ( unsigned long from, unsigned long to ) {
  unsigned long length = to - from;
  if ( length < 100000 ) {
    long double result = 0;
    for ( long double x = to; x > from; ) {
      -- x;
      long double part = 0;
      for ( long double y = x - 1; y > 0; -- y ) {
        part += 1.0l / ( y * ( x + y ) * ( x + y ) );
      }
      result += part;
    }
    return ( result );
  }
  unsigned long mid = from + length / 2;
  auto handle = std::async( std::launch::async,
                            witten, mid, to );
  long double lower = witten( from, mid );
  return ( lower + handle.get() );
}

int main ( void ) {
  long double N = 2000000;
  long double sum = witten( 1, N );
  long double C = ( 1.0l - sum/Z3 ) / 2;
  std::cout << C << "\n";
  std::cout << 1 + 4*C << "\n";
  std::cout << 3.0l/8 + 2*C << "\n";
}

// end of file
