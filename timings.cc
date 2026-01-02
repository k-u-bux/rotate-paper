// timings.cc (C) Kai-Uwe Bux [2024-2025]
// ======================================

#include "rotate.cc"
#include "random.cc"
#include "sequence_types.cc"
#include "timer.cc"

#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <type_traits> 

// benchmarking
// ============

using clock_type = std::chrono::high_resolution_clock;

template < typename IntVector, typename Method >
double benchmark ( Method alg, std::uint64_t n_runs, std::uint64_t length ) {
  volatile std::uint64_t optimization_blocker;
  volatile std::size_t i;
  volatile std::size_t j;
  using value_type = std::decay_t< decltype( IntVector{}[0] ) >;
  length += sizeof( value_type ) - 1;
  length /= sizeof( value_type );
  clock_type::duration the_duration {};
  IntVector seq ( length );
  for ( std::uint64_t i = 0; i < length; ++ i ) {
    seq[ i ] = i;
  }
  std::uint64_t n_total = 0;
  {
    uniform_rng rng ( 123151 );
    accumulating_timer< clock_type > dummy ( the_duration );
    for( std::uint64_t k = 0; k < n_runs; ++k ) {
      j = length - rng( length / 2 );
      i = rng( j + 1 );
      n_total += j;
      alg( seq.begin(), seq.begin() + i, seq.begin() + j );
      alg( seq.begin(), seq.begin() + i, seq.begin() + j );        
    }
  }
  optimization_blocker += seq.front() + n_total;
  std::uint64_t n_dummy = 0;
  {
    uniform_rng rng ( 123151 );
    subtracting_timer< clock_type > dummy ( the_duration );
    for( std::uint64_t k = 0; k < n_runs; ++k ) {
      j = length - rng( length / 2 );
      i = rng( j + 1 );
      n_dummy += j;
      alg( seq.begin(), seq.begin() + i, seq.begin() + j );
    }
  }
  optimization_blocker += seq.front() + n_dummy;
  std::chrono::duration< double, std::ratio<1,1> > seconds ( the_duration );
  return ( 1000000000 * seconds.count() / n_total / sizeof( value_type ) );
}

template < typename IntVector, typename Method >
double benchmark_b ( Method alg, std::uint64_t n_runs, std::uint64_t length ) {
  volatile std::uint64_t optimization_blocker;
  using value_type = std::decay_t< decltype( IntVector{}[0] ) >;
  length += sizeof( value_type ) - 1;
  length /= sizeof( value_type );
  clock_type::duration the_duration {};
  uniform_rng rng ( 123151 );
  IntVector seq ( length );
  for ( std::uint64_t i = 0; i < length; ++ i ) {
    seq[ i ] = i;
  }
  std::uint64_t n_total = 0;
  std::uint64_t unroll = 1 << 16;
  {
    std::uint64_t k = 0;
    for( ; k + unroll <= n_runs; k += unroll ) {
      std::vector< std::uint64_t > past ( unroll );
      std::vector< std::uint64_t > middle ( unroll );
      for ( std::uint64_t i = 0; i < unroll; ++ i ) {
        past[ i ] = length - rng( length / 2 );
        middle[ i ] = rng( past[ i ] );
      }
      n_total = std::accumulate( past.begin(), past.end(), n_total );
      {
        accumulating_timer< clock_type > dummy ( the_duration );
        for ( auto i = 0; i < unroll; ++ i ) {
          auto iter = alg( seq.begin(), seq.begin() + middle[ i ], seq.begin() + past[ i ] );
        }
      }
    }
    optimization_blocker += seq.front();
    {
      std::vector< std::uint64_t > past ( unroll );
      std::vector< std::uint64_t > middle ( unroll );
      for ( std::uint64_t i = 0; i < unroll; ++ i ) {
        past[ i ] = rng( length );
        middle[ i ] = rng( 1 + past[ i ] );
      }
      n_total += std::accumulate( past.begin(), past.begin() + ( n_runs - k ), n_total );
      {
        accumulating_timer< clock_type > dummy ( the_duration );
        for ( auto i = 0; i < n_runs - k; ++ i ) {
          auto iter = alg( seq.begin(), seq.begin() + middle[ i ], seq.begin() + past[ i ] );
        }
      }
    }
    optimization_blocker += seq.front();
  }
  std::chrono::duration< double, std::ratio<1,1> > seconds ( the_duration );
  return ( 1000000000 * seconds.count() / n_total / sizeof( value_type ) );
}

#define SHOW(expr)                                  \
  std::cout << #expr << " = " << expr << std::endl; \


#define BENCHMARK_VECT(alg,vect)                      \
  SHOW( benchmark< vect >( alg, 500000000, 32 ) );      \
  SHOW( benchmark< vect >( alg, 300000000, 64 ) );      \
  SHOW( benchmark< vect >( alg, 90000000, 128 ) );      \
  SHOW( benchmark< vect >( alg, 60000000, 300 ) );      \
  SHOW( benchmark< vect >( alg, 30000000, 1000 ) );     \
  SHOW( benchmark< vect >( alg, 10000000, 3000 ) );     \
  SHOW( benchmark< vect >( alg, 3000000, 10000 ) );    \
  SHOW( benchmark< vect >( alg, 1000000, 30000 ) );    \
  SHOW( benchmark< vect >( alg, 300000, 100000 ) );    \
  SHOW( benchmark< vect >( alg, 100000, 300000 ) );    \
  SHOW( benchmark< vect >( alg, 30000, 1000000 ) );    \
  SHOW( benchmark< vect >( alg, 10000, 3000000 ) );    \
  SHOW( benchmark< vect >( alg, 3000, 10000000 ) );    \
  SHOW( benchmark< vect >( alg, 1000, 30000000 ) );    \
  SHOW( benchmark< vect >( alg, 300, 100000000 ) );    \
  SHOW( benchmark< vect >( alg, 100, 300000000 ) );    \
  SHOW( benchmark< vect >( alg, 50, 1000000000 ) );   \
  SHOW( benchmark< vect >( alg, 50, 3000000000 ) );   \
  SHOW( benchmark< vect >( alg, 50, 10000000000 ) );  \


#define BENCHMARK(alg)                          \
  BENCHMARK_VECT( alg, u_ptr_vect );            \
  BENCHMARK_VECT( alg, x_ptr_vect );            \
  BENCHMARK_VECT( alg, s_ptr_vect );            \
  BENCHMARK_VECT( alg, int64_vect );            \
  BENCHMARK_VECT( alg, int32_vect );            \
  BENCHMARK_VECT( alg, short_vect );            \
  BENCHMARK_VECT( alg, char_vect );             \
  BENCHMARK_VECT( alg, long_double_vect );      \


int main ( void ) {
  BENCHMARK( rotation::std_implementation );
  BENCHMARK( rotation::block_cycle );
  // BENCHMARK( rotation::block_async_cycle );
  BENCHMARK( rotation::block_swap );
  // BENCHMARK( rotation::blend );
  BENCHMARK( rotation::trinity );
  BENCHMARK( rotation::buffering_trinity );
  BENCHMARK( rotation::triple_reverse );
  BENCHMARK( rotation::buffering_triple_reverse );
  // BENCHMARK( rotation::optimal_rotate );
}

// end of file
