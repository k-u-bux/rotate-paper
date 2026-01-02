constexpr static struct unchecked_trinity_t {
    
  template < typename A, typename B, typename C >
  void swap3 ( A & a, B & b, C & c ) const {
    auto temp = std::move( a );
    a = std::move( b );
    b = std::move( c );
    c = std::move( temp );
  }

  template < typename A, typename B, typename C, typename D >
  void swap4 ( A & a, B & b, C & c, D & d ) const {
    auto temp = std::move( a );
    a = std::move( b );
    b = std::move( c );
    c = std::move( d );
    d = std::move( temp );
  }
    
  template < typename Iterator >
  Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
    std::size_t const l_size = middle - from;
    std::size_t const r_size = to - middle;
      
    Iterator iter_l_beg = from;
    Iterator iter_l_end = middle;
    Iterator iter_r_beg = middle;
    Iterator iter_r_end = to;

    if ( l_size >= r_size ) {
      for ( std::size_t counter = r_size / 2; counter -- > 0; ) {
        -- iter_l_end; -- iter_r_end;
        swap4( *iter_l_end, *iter_l_beg, *iter_r_beg, *iter_r_end );
        ++ iter_l_beg; ++ iter_r_beg;
      }
      for ( std::size_t counter = ( iter_l_end - iter_l_beg ) / 2; counter -- > 0; ) {
        -- iter_l_end; -- iter_r_end;
        swap3( *iter_l_end, *iter_l_beg, *iter_r_end );
        ++ iter_l_beg;
      }
    } else {
      for ( std::size_t counter = l_size / 2; counter -- > 0; ) {
        -- iter_l_end; -- iter_r_end;
        swap4( *iter_l_end, *iter_l_beg, *iter_r_beg, *iter_r_end );
        ++ iter_l_beg; ++ iter_r_beg;
      }
      for ( std::size_t counter = ( iter_r_end - iter_r_beg ) / 2; counter -- > 0; ) {
        -- iter_r_end;
        swap3( *iter_l_beg, *iter_r_beg, *iter_r_end );
        ++ iter_l_beg; ++ iter_r_beg;
      }
    }
    std::reverse( iter_l_beg, iter_r_end );
    return ( from + r_size );
  }

} unchecked_trinity;
