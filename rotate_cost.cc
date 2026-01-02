// rotate_cost.cc (C) Kai-Uwe Bux [2024-2025]
// ==========================================

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>


using uhuge = unsigned long long;
using fhuge = long double;

fhuge cost_old ( fhuge n, fhuge k, fhuge b, fhuge s = 1 ) {
  k = ( k < n-k ? k : n-k );
  if ( k <= b ) {
    return ( n + k );
    // return ( 3* n );
  }
  fhuge q = std::floor( n / k );
  fhuge m = ( q - 1 ) * k;
  fhuge r = n - m;
  return ( ( q + s ) * k + cost_old( r, r-k, b ) );
}


fhuge intermediate_move_cost ( fhuge n, fhuge k, fhuge b ) {
  if ( k <= b ) {
    return ( k );
  }
  fhuge x = k / n;
  fhuge u = n / k;
  fhuge q = std::floor( u );
  fhuge m = q * k;
  fhuge k_next = n - m;
  fhuge n_next = k + k_next;
  return ( 2 * k + intermediate_move_cost( n_next, k_next, b ) );
}

fhuge cost ( fhuge n, fhuge k, fhuge b ) {
  return ( n + intermediate_move_cost( n, k, b ) );
}

int main ( int argn, char ** args ) {
  fhuge percentage = std::atof( args[ 1 ] );
  for ( fhuge k = 0.0; k <= 0.5; k += 0.5/2584 ) {
    std::cout << k << " " << cost( 1.0, k, percentage ) << "\n";
  }
}

// end of file
