constexpr static struct block_swap_t {
    
  template < typename Iterator >
  Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
    std::size_t r_size = to - middle;
    std::size_t l_size = middle - from;
    while ( true ) {
      if ( r_size == 0 ) {
        return ( from );
      }
      if ( l_size == 0 ) {
        return ( to );
      }
      if ( r_size >= l_size ) {
        std::swap_ranges( from, middle, middle );
        from = middle;
        middle += l_size;
        r_size -= l_size;
        continue;
      }
      { // 0 < r_size < l_size
        std::swap_ranges( middle, to, middle - r_size );
        to -= r_size;
        middle -= r_size;
        l_size -= r_size;
        continue;
      }
    }
  }
    
} block_swap;
