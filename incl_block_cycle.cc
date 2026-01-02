constexpr static struct block_cycle_t {

  template < typename Iterator >
  Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
    std::size_t l_size = middle - from;
    std::size_t r_size = to - middle;
      
    using value_type = std::decay_t< decltype( *from ) >;
    constexpr std::size_t const buffer_size = BUFFER_SIZE;
    constexpr std::size_t const batch_size =
      std::max( std::size_t( 1 ), BATCH_SIZE / sizeof( value_type ) );
    static_assert( batch_size <= buffer_size );
    thread_local std::array< value_type, buffer_size > buffer;

    while ( l_size > 0 && r_size > 0 ) {
 
      if ( l_size < r_size  ) {
        if ( l_size <= buffer_size ) {
          std::move( from, middle, buffer.begin() );
          std::move( middle, to, from );
          std::move( buffer.begin(), buffer.begin() + l_size, to - l_size );
          break;
        }

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
        std::size_t n_remaining = l_size - i;
        std::size_t shift = 0;
        std::move( from + i, from + i + n_remaining, buffer.begin() );
        for ( ; shift + l_size <= r_size; shift += l_size ) {
          std::move( from + i + shift + l_size,
                     from + i + shift + l_size + n_remaining,
                     from + i + shift );
        }          
        std::move( buffer.begin(),
                   buffer.begin() + n_remaining,
                   from + i + shift );
        from   += shift;
        middle += shift;
        r_size -= shift;
        continue;
      }

      if ( r_size < l_size ) {
        if ( r_size <= buffer_size ) {
          std::move( middle, to, buffer.begin() );
          std::move_backward( from, middle, to );
          std::move( buffer.begin(), buffer.begin() + r_size, from );
          break;
        }

        std::size_t i = 0;
        for ( ; i < r_size - batch_size; i += batch_size ) {
          std::move( middle + i, middle + i + batch_size, buffer.begin() );
          std::size_t shift = 0;
          for ( ; shift <= l_size - r_size; shift += r_size ) {
            std::move( middle + i - shift - r_size,
                       middle + i -shift - r_size + batch_size,
                       middle + i - shift );
          }
          std::move( buffer.begin(),
                     buffer.begin() + batch_size,
                     middle + i - shift );
        }
        std::size_t n_remaining = r_size - i;
        std::move( middle + i, middle + i + n_remaining, buffer.begin() );
        std::size_t shift = 0;
        for ( ; shift <= l_size - r_size; shift += r_size ) {
          std::move( middle + i - shift - r_size,
                     middle + i -shift - r_size + n_remaining,
                     middle + i - shift );
        }
        std::move( buffer.begin(),
                   buffer.begin() + n_remaining,
                   middle + i - shift );
        to     -= shift;
        middle -= shift;
        l_size -= shift;
        continue;
      }

      std::swap_ranges( from, middle, middle );
      break;
    }
    return ( from + r_size );
  }
    
} block_cycle;
