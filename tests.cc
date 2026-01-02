// tests.cc (C) Kai-Uwe Bux [2024-2025]
// ====================================

#include "rotate.cc"
#include <vector>
#include <iostream>
#include <iomanip>

template < typename Alg1, typename Alg2 >
bool compare ( Alg1 alg1, Alg2 alg2, std::size_t length, std::size_t k ) {
  std::vector< unsigned > u_vect ( length );
  {
    unsigned dummy = 0;
    for ( auto & entry : u_vect ) {
      entry += dummy++;
    }
  }
  std::vector< unsigned > v_vect = u_vect;
  auto dummy1 = alg1( u_vect.begin(), u_vect.begin() + k, u_vect.end() );
  auto dummy2 = alg2( v_vect.begin(), v_vect.begin() + k, v_vect.end() );
  bool result = u_vect == v_vect;
  result &= ( dummy1 - u_vect.begin() ) == ( dummy2 - v_vect.begin() );
  if ( ! result ) {
    std::cout << k << " " << length << std::endl;
    std::exit( 0 );
  }
  return ( result );
}

template < typename Alg1, typename Alg2 >
bool compare ( Alg1 alg1, Alg2 alg2 ) {
  compare( alg1, alg2, 0, 0 );
  for ( std::size_t length = 1; length < 10000; length += 1 + length / 8 ) {
    for ( std::size_t cut = 0; cut <= length; ++ cut ) {
      compare( alg1, alg2, length, cut );
    }
  }
  return ( true );
}

template < typename Algorithm >
bool run_check ( Algorithm alg ) {
  return ( compare( alg, rotation::std_implementation ) );
}


#define CHECK(alg)                                          \
  if ( run_check( alg ) ) {                                 \
    std::cout << "passed: algorithm " << #alg << std::endl; \
  }


int main ( void ) {
  CHECK( rotation::block_cycle );
  CHECK( rotation::block_swap );
  CHECK( rotation::trinity );
  CHECK( rotation::buffering_trinity );
  CHECK( rotation::basic_triple_reverse );
  CHECK( rotation::triple_reverse );
  CHECK( rotation::buffering_triple_reverse );
  CHECK( rotation::buffering_std_implementation );
  CHECK( rotation::optimal_rotate );
}

// end of file
