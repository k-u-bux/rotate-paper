// rotate.cc (C) Kai-Uwe Bux [2024-2025]
// =====================================
/*
  various std::rotate implementations
*/

#ifndef INCLUDE_ONCE_ROTATE
#define INCLUDE_ONCE_ROTATE

#include <future>
#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>
#include <numeric>

namespace rotation {

  constexpr static std::size_t BUFFER_SIZE = 256;
  constexpr static std::size_t BATCH_SIZE = 32;
  constexpr static std::size_t NUM_CORES = 8;
  
  // adaptors
  // ========
  
#include "incl_buffering.cc"
#include "incl_checking.cc"

  // algorithms
  // ==========

#include "incl_block_cycle.cc"
#include "incl_block_async_cycle.cc"
#include "incl_block_swap.cc"
#include "incl_basic_triple_reverse.cc"
#include "incl_unchecked_trinity.cc"
#include "incl_checking_algorithms.cc"
#include "incl_buffering_algorithms.cc"
#include "incl_hybrid_rotate.cc"
#include "incl_optimal_rotate.cc"
  
  // Hasham-Li-Salah
  // ===============
  
#include "incl_blend.cc"
  

  // std::rotate niebloid
  // ====================

  constexpr static struct std_implementation_t {
  
    template < typename Iterator >
    Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
      return ( std::rotate( from, middle, to ) );
    }
  
  } std_implementation;

  constexpr static auto buffering_std_implementation = buffering[ std_implementation ];

} // namspace rotation


// choose a reasonable default
// ===========================

constexpr static struct rotate_t {
private:
    
  template < typename Algorithm >
  struct worker {

    Algorithm alg;

    template < typename Iterator >
    Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
      ENFORCE( from <= middle );
      ENFORCE( middle <= to );
      return ( alg( from, middle, to ) );
    }

  };

public:

  template < typename Iterator >
  Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
    return ( (*this)[ rotation::block_cycle ]( from, middle, to ) );
  }

  template < typename Algorithm >
  worker< Algorithm > operator [] ( Algorithm alg ) const {
    return { alg };
  }
    
} rotate;
  
#endif // ROTATE

// end of file
