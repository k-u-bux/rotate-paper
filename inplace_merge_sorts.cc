// inplace_merge_sorts.cc (C) Kai-Uwe Bux [2024-2025]
// ==================================================

#include <type_traits>
#include <array>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <future>

#include "rotate.cc"
#include "random.cc"
#include "sequence_types.cc"
#include "timer.cc"


#define LIKELY(cond)  __builtin_expect(cond,1)
#define UNLIKELY(cond)  __builtin_expect(cond,0)
#define FIFTY_FIFTY(cond)  __builtin_expect_with_probability(cond,0,0.5)

constexpr static struct {
      
  template < typename InIter1, typename InIter2, typename OutIter, typename IsLess >
  void operator () ( InIter1 from1, InIter1 to1, InIter2 from2, InIter2 to2,
                     OutIter dst, IsLess is_less ) const {
    using std::is_sorted;
    using value_type = std::decay_t< decltype( *dst ) >;
    // ENFORCE( is_sorted( from1, to1, is_less ) );
    // ENFORCE( is_sorted( from2, to2, is_less ) );
    // PROMISE( is_sorted( dst, dst + ( to1 - from1 ) + ( to2 - from2 ), is_less ) );
        
    OutIter out = dst;
    std::size_t t1 = to1 - from1;
    std::size_t f1 = 0;
    std::size_t t2 = to2 - from2;
    std::size_t f2 = 0;

    while ( LIKELY( ( f1 < t1 ) && ( f2 < t2 ) ) ) {
      *out++ = std::move( is_less( from2[ f2 ], from1[ f1 ] ) ? from2[ f2++ ] : from1[ f1++ ] );
    }

    if ( t1 <= f1 ) {
      std::move( from2 + f2, from2 + t2, out );
      return;
    }
    if ( t2 <= f2 ) {
      std::move( from1 + f1, from1 + t1, out );
      return;
    }
  }
    
} basic_merge_bottom_up;

constexpr static struct {

  template < typename InIter1, typename InIter2, typename OutIter, typename IsLess >
  void operator () ( InIter1 const from1, InIter1 const to1, InIter2 const from2, InIter2 const to2,
                     OutIter const dst, IsLess is_less ) const {
    using std::is_sorted;
    using value_type = std::decay_t< decltype( *dst ) >;
    // ENFORCE( is_sorted( from1, to1, is_less ) );
    // ENFORCE( is_sorted( from2, to2, is_less ) );
    // PROMISE( is_sorted( dst - ( to1 - from1 ) - ( to2 - from2 ), dst, is_less ) );

    // if ( done_by_reordering_blocks[ copy ]( from1, to1, from2, to2, dst, is_less ) ) { return; }
        
    OutIter out = dst;
    // PROMISE( out == dst - ( to1 - from1 ) - ( to2 - from2 ) );
    std::ptrdiff_t t1 = to1 - from1;
    std::ptrdiff_t f1 = 0;
    std::ptrdiff_t t2 = to2 - from2;
    std::ptrdiff_t f2 = 0;
    --t1;
    --t2;
        
    while ( LIKELY( ( f1 <= t1 ) && ( f2 <= t2 ) ) ) {
      *--out = std::move( is_less( from2[ t2 ], from1[ t1 ] ) ? from1[ t1-- ] : from2[ t2-- ] );
    }

    if ( t1 < f1 ) {
      std::move_backward( from2 + f2, from2 + t2 + 1, out );
      return;
    }
    if ( t2 < f2 ) {
      std::move_backward( from1 + f1, from1 + t1 + 1, out );
      return;
    }
  }
      
} basic_merge_top_down;

constexpr static struct unstable_sort2_branchless_t {

  // IMPORTANT: this one is predictable unstable
  
  template < typename ValueType, typename IsLess >
  bool operator () ( ValueType & lhs, ValueType & rhs, IsLess is_less ) const {
    bool lhs_is_less = is_less( lhs, rhs );
    ValueType temp = ( lhs_is_less ? std::move( rhs ) : std::move( lhs ) );
    lhs = ( lhs_is_less ? std::move( lhs ) : std::move( rhs ) );
    rhs = std::move( temp );
    return ( lhs_is_less );
  }
  
} unstable_sort2_branchless;

constexpr static struct stable_sort2_branchless_t {
  
  template < typename ValueType, typename IsLess >
  bool operator () ( ValueType & lhs, ValueType & rhs, IsLess is_less ) const {
    bool rhs_is_less = is_less( rhs, lhs );
    ValueType temp = ( rhs_is_less ? std::move( lhs ) : std::move( rhs ) );
    lhs = ( rhs_is_less ? std::move( rhs ) : std::move( lhs ) );
    rhs = std::move( temp );
    return ( rhs_is_less );
  }
  
} stable_sort2_branchless;

constexpr static struct stable_triad_t {

  template < typename Iterator, typename IsLess >
  void operator() ( Iterator from, Iterator to, IsLess is_less ) const {
    using std::is_sorted;
    // PROMISE( is_sorted( from, to, is_less ) );
    using std::swap;
    std::size_t length = to - from;  
    for ( std::size_t i = length; i > 2; i -= 3 ) {
      auto x = std::move( from[ 0 ] );
      auto y = std::move( from[ 1 ] );
      auto z = std::move( from[ 2 ] );
      stable_sort2_branchless( x, y, is_less );
      stable_sort2_branchless( y, z, is_less );
      stable_sort2_branchless( x, y, is_less );
      for ( std::size_t j = 3; j < i; ++j ) {
        auto t = std::move( from[ j ] );
        unstable_sort2_branchless( t, z, is_less );
        unstable_sort2_branchless( t, y, is_less );
        unstable_sort2_branchless( t, x, is_less );
        from[ j - 3 ] = std::move( t );
      }
      from[ i - 3 ] = std::move( x );
      from[ i - 2 ] = std::move( y );
      from[ i - 1 ] = std::move( z );
    }
    if ( length > 1 ) {
      stable_sort2_branchless( from[ 0 ], from[ 1 ], is_less );
    }
  }
    
} stable_triad;



#define SOME_BOUND 4
#define SOME_SMALL_SORT stable_triad

#include "incl_inplace_merge.cc"

constexpr static struct {
private:
  
  template < typename Rotate >
  struct worker {
    
    Rotate ROTATE;

#include "incl_inplace_merge_sort.cc"
    
    template < typename Iterator, typename IsLess >
    void operator () ( Iterator from, Iterator to, IsLess is_less ) const {
      const_cast< worker* >( this )->inplace_merge_sort( from, to, is_less );
    }
    
  };
  
public:
    
  template < typename Iterator, typename IsLess >
  void operator () ( Iterator from, Iterator to, IsLess is_less ) const {
    (*this)[ rotation::std_implementation ]( from, to, is_less );
  }

  template < typename Rotate >
  worker< Rotate > operator [] ( Rotate rotate ) const {
    return { rotate };
  }
    
} inplace_merge_sort;


// sorts from the standard library
// ===============================

constexpr static struct {
  
  template < typename Iterator, typename Sentinel, typename IsLess >
  void operator () ( Iterator from, Sentinel to, IsLess is_less ) const {
    std::sort( from, to, is_less );
  }
  
} std_sort;


constexpr static struct {
  
  template < typename Iterator, typename Sentinel, typename IsLess >
  void operator () ( Iterator from, Sentinel to, IsLess is_less ) const {
    std::stable_sort( from, to, is_less );
  }
  
} std_stable_sort;


constexpr static struct {
  
  template < typename Iterator, typename Sentinel, typename IsLess >
  void operator () ( Iterator from, Sentinel to, IsLess is_less ) const {
    std::partial_sort( from, to, to, is_less );
  }
  
} std_partial_sort;


// catch bugs
// ==========
constexpr static struct {
private:
  
  template < typename Algorithm >
  struct bind_default_predicate_t {
    
    Algorithm the_alg;
    
    template < typename Iterator, typename IsLess >
    void operator () ( Iterator from, Iterator to, IsLess is_less ) const {
      the_alg( from, to, is_less );
      assert( std::is_sorted( from, to, is_less ) );
    }
    
    template < typename Iterator >
    void operator () ( Iterator from, Iterator to ) const {
      (*this)( from, to, std::less{} );
    }
    
  };
  
public:
  
  template < typename ... Args >
  void operator () ( Args && ... args ) const {
    (*this)[ std_sort ]( std::forward< Args >( args ) ... );
  }
  
  template < typename Alg >
  bind_default_predicate_t< Alg > operator [] ( Alg the_alg ) const {
    return { the_alg };
  }
  
} sort;


// benchmarking
// ============

template < typename IntVector, typename Method >
double benchmark ( Method alg, unsigned n_runs, unsigned length, unsigned n_values ) {
  auto normalize = [] ( auto x ) {
    auto log_term = std::log( x ) / std::log( 2.0 );
    return ( x * log_term * log_term );
  };
  unsigned n_buckets = 1 + 10000000 / length;
  volatile unsigned optimization_blocker;
  std::chrono::system_clock::duration the_duration {};
  uniform_rng rng ( 123151 );
  IntVector seq;
  n_runs /= n_buckets;
  ++ n_runs;
  unsigned s_length = n_buckets * length;
  seq.reserve( s_length );
  for ( auto i = 0; i < n_runs; ++ i ) {
    seq.clear();
    for ( unsigned i = 0; i < s_length; ++ i ) {
      seq.push_back( rng( n_values ) );
    }
    std::random_shuffle( seq.begin(), seq.end(), rng );
    {
      accumulating_timer dummy ( the_duration );
      for ( unsigned i = 0; i < n_buckets; ++ i ) {
        sort[ alg ]( seq.begin() + i * length, seq.begin() + ( i + 1 ) * length );
        optimization_blocker += seq[ i * length ];
      }
    }
  }
  std::chrono::duration< double, std::ratio<1,1> > seconds ( the_duration );
  return ( 1000000000 * seconds.count() / n_runs / n_buckets / normalize( length ) );
}


#define SHOW(expr)                                  \
  std::cout << #expr << " = " << expr << std::endl; \


#define TYPED_BENCHMARK(name,seq_type)                            \
  SHOW( benchmark< seq_type >( name, 500000, 6, 10000000000 ) );    \
  SHOW( benchmark< seq_type >( name, 500000, 12, 10000000000 ) );   \
  SHOW( benchmark< seq_type >( name, 500000, 24, 10000000000 ) );   \
  SHOW( benchmark< seq_type >( name, 500000, 48, 10000000000 ) );   \
  SHOW( benchmark< seq_type >( name, 500000, 100, 10000000000 ) );  \
  SHOW( benchmark< seq_type >( name, 50000, 250, 10000000000 ) );   \
  SHOW( benchmark< seq_type >( name, 50000, 500, 10000000000 ) );   \
  SHOW( benchmark< seq_type >( name, 10000, 1000, 10000000000 ) );  \
  SHOW( benchmark< seq_type >( name, 1000, 10000, 10000000000 ) );  \
  SHOW( benchmark< seq_type >( name, 100, 100000, 10000000000 ) );  \
  SHOW( benchmark< seq_type >( name, 20, 1000000, 10000000000 ) );  \
  SHOW( benchmark< seq_type >( name, 20, 10000000, 10000000000 ) ); \
  SHOW( benchmark< seq_type >( name, 10, 100000000, 10000000000 ) );  \
  SHOW( benchmark< seq_type >( name, 10, 1000000000, 10000000000 ) ); \


#define BENCHMARK(alg)                          \
  TYPED_BENCHMARK( alg, long_double_vect );     \
  TYPED_BENCHMARK( alg, int64_vect );           \
  TYPED_BENCHMARK( alg, int32_vect );           \
  TYPED_BENCHMARK( alg, short_vect );           \
  TYPED_BENCHMARK( alg, char_vect );            \


int main ( void ) {
  BENCHMARK( std_sort );
  BENCHMARK( std_stable_sort );
  BENCHMARK( std_partial_sort );
  BENCHMARK( inplace_merge_sort[rotation::block_cycle] );
  BENCHMARK( inplace_merge_sort[rotation::block_async_cycle] );
  BENCHMARK( inplace_merge_sort[rotation::block_swap] );
  BENCHMARK( inplace_merge_sort[rotation::blend] );
  BENCHMARK( inplace_merge_sort[rotation::trinity] );
  BENCHMARK( inplace_merge_sort[rotation::triple_reverse] );
  BENCHMARK( inplace_merge_sort[rotation::buffering_trinity] );
  BENCHMARK( inplace_merge_sort[rotation::buffering_triple_reverse] );
  BENCHMARK( inplace_merge_sort[rotation::std_implementation] );
}

// end of file
