constexpr static struct optimal_rotate_t {
  
  template < typename Iterator >
  Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
    std::size_t length = to - from;
    std::size_t l_size = middle - from;
    std::size_t r_size = to - middle;
    Iterator result = from + r_size;
    if ( l_size == 0 || r_size == 0 ) {
      return ( result );
    }
    std::size_t count = std::gcd( l_size, r_size );
    std::size_t run_length = length / count;
    for ( std::size_t i = 0; i < count; ++ i ) {
      std::size_t target = i;
      std::size_t source = target + l_size;
      auto dummy = std::move( from[ target ] );
      for ( std::size_t k = 1; k < run_length; ++ k ) {
        from[ target ] = std::move( from[ source ] );
        target = source;
        source += l_size;
        source = ( source >= length ? source - length : source );
      }
      from[ target ] = std::move( dummy );
    }
    return ( result );
  }
  
} optimal_rotate;
  
