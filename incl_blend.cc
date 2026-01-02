constexpr static struct blend_t {
    
  template < typename Iterator >
  Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
    std::size_t l_size = middle - from;
    std::size_t r_size = to - middle;
      
    using value_type = std::decay_t< decltype( *from ) >;
    constexpr std::size_t const buffer_size =
      std::max( std::size_t(1), BATCH_SIZE / sizeof( value_type ) );
    std::size_t batch_size = buffer_size;
    thread_local std::array< value_type, buffer_size > buffer;

    if ( 0 < l_size && l_size <= r_size ) {
      if ( l_size < batch_size ) { batch_size = l_size; }
      std::size_t i = 0;
      for ( ; i < l_size - batch_size; i += batch_size ) {
        std::move( from + i, from + i + batch_size, buffer.begin() );
        std::size_t shift = 0;
        for ( ; shift <= r_size - l_size; shift += l_size ) {
          std::move( from + i + shift + l_size,
                     from + i + shift + l_size + batch_size,
                     from + i + shift );
        }
        std::move( buffer.begin(),
                   buffer.begin() + batch_size,
                   from + i + shift );
      }
      std::size_t n_remaining = l_size - i; /*@\label{begin.patch}@*/
      std::size_t shift = 0;
      std::move( from + i, from + i + n_remaining, buffer.begin() );
      for ( ; shift + l_size <= r_size; shift += l_size ) {
        std::move( from + i + shift + l_size,
                   from + i + shift + l_size + n_remaining,
                   from + i + shift );
      }          
      std::move( buffer.begin(),
                 buffer.begin() + n_remaining,
                 from + i + shift ); /*@\label{end.patch}@*/
      from   += shift;
      middle += shift;
      r_size -= shift;
    }
      
    return ( triple_reverse( from, middle, to ) );
  }
    
} blend;
