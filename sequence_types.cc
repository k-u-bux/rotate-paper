// sequence_type.cc (C) Kai-Uwe Bux [2024-2025]
// ============================================

#ifndef INCLUDE_ONCE_SEQUENCES
#define INCLUDE_ONCE_SEQUENCES

#include <memory>
#include <vector>
#include <cstdint>

// heap allocated types

class u_ptr {

  std::unique_ptr< unsigned > data;

public:
  
  u_ptr ( void )
    : data { new unsigned ( 0 ) }
  {}

  u_ptr & operator = ( unsigned i ) {
    *data = i;
    return ( *this );
  }

  operator unsigned ( void ) const {
    return ( *data );
  }
  
};

class x_ptr {

  std::unique_ptr< unsigned > data;

public:
  
  x_ptr ( void )
    : data { new unsigned ( 0 ) }
  {}

  x_ptr & operator = ( unsigned i ) {
    *data = i;
    return ( *this );
  }

  x_ptr ( x_ptr const & other )
    : data { new unsigned ( *other.data ) }
  {} 

  x_ptr & operator = ( x_ptr && other ) {
    std::swap( data, other.data );
    return ( *this );
  }
  
  operator unsigned ( void ) const {
    return ( *data );
  }

  friend
  void swap ( x_ptr & lhs, x_ptr & rhs ) {
    std::swap( lhs.data, rhs.data );
  }
  
};

class s_ptr {

  std::shared_ptr< unsigned > data;

public:
  
  s_ptr ( void )
    : data { new unsigned ( 0 ) }
  {}

  s_ptr & operator = ( unsigned i ) {
    *data = i;
    return ( *this );
  }

  operator unsigned ( void ) const {
    return ( *data );
  }
  
};

// sequence types
// ==============

using int_vect = std::vector< int >;
using int64_vect = std::vector< std::int64_t >;
using short_vect = std::vector< short >;
using int32_vect = std::vector< std::int32_t >;
using char_vect = std::vector< char >;
using long_double_vect = std::vector< long double >;
using double_vect = std::vector< double >;
using float_vect = std::vector< float >;
using u_ptr_vect = std::vector< u_ptr >;
using x_ptr_vect = std::vector< x_ptr >;
using s_ptr_vect = std::vector< s_ptr >;

#endif // SEQUENCES


// end of file
