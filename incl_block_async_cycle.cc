constexpr static struct block_async_cycle_t {
private:

  constexpr static const std::size_t NUM_CORES = 16;
  constexpr static const std::size_t BASE_WORK_LOAD = 1 << 19;
    
  template < typename Iterator, typename Buffer >
  static
  void block_rotate ( Iterator first, Buffer buffer,
                      std::size_t num_blocks, std::size_t block_size,
                      std::ptrdiff_t stride ) {
    Iterator here = first;
    std::move( here, here + block_size, buffer );
    for ( std::size_t i = 0; i < num_blocks; ++ i ) {
      Iterator next = here + stride;
      std::move( next, next + block_size, here );
      here = next;
    }
    std::move( buffer, buffer + block_size, here );
  }

  template < std::size_t buffer_size, typename ValueType, typename Iterator >
  static
  void rotate_range ( Iterator from, Iterator to, std::size_t num_cores,
                      std::size_t num_blocks, std::ptrdiff_t stride ) {
    std::size_t length = to - from;
    if ( num_cores <= 1
         ||
         length <= BASE_WORK_LOAD / sizeof( ValueType ) / num_blocks ) {
      std::array< ValueType, buffer_size > buffer;
      Iterator head = from;
      Iterator next = head + buffer_size;
      while ( next < to ) {
        block_rotate( head, buffer.begin(), num_blocks, buffer_size, stride );
        head = next;
        next = head + buffer_size;
      }
      block_rotate( head, buffer.begin(), num_blocks, to - head, stride );
    } else {
      Iterator mid = from + length / 2;
      auto dummy = std::async( rotate_range< buffer_size, ValueType, Iterator >,
                               from, mid, num_cores / 2, num_blocks, stride );
      rotate_range< buffer_size, ValueType >
        ( mid, to, num_cores / 2, num_blocks, stride );
      dummy.get();
    }
  }
    
  template < std::size_t buffer_size, typename ValueType, typename Iterator >
  std::size_t rotate_left ( Iterator from, Iterator mid, Iterator to ) const {
    std::size_t l_size = mid - from;
    std::size_t r_size = to - mid;
    std::size_t num_blocks = r_size / l_size;
    rotate_range< buffer_size, ValueType >
      ( from, mid, NUM_CORES, num_blocks, l_size );
    return ( num_blocks * l_size );
  }
  
  template < std::size_t buffer_size, typename ValueType, typename Iterator >
  std::size_t rotate_right ( Iterator from, Iterator mid, Iterator to ) const {
    std::size_t l_size = mid - from;
    std::size_t r_size = to - mid;
    std::size_t num_blocks = l_size / r_size;
    rotate_range< buffer_size, ValueType >
      ( mid, to, NUM_CORES, num_blocks, - std::ptrdiff_t( r_size ) );
    return ( num_blocks * r_size );
  }
    
public:
  
  template < typename Iterator >
  Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
    std::size_t l_size = middle - from;
    std::size_t r_size = to - middle;
      
    using value_type = std::decay_t< decltype( *from ) >;
    constexpr std::size_t const buffer_size = BUFFER_SIZE;
    constexpr std::size_t const batch_size =
      std::max( std::size_t( 1 ), BATCH_SIZE / sizeof( value_type ) );
    thread_local std::array< value_type, buffer_size > buffer;

    while ( l_size > 0 && r_size > 0 ) {
 
      if ( l_size < r_size  ) {
        if ( l_size <= buffer_size ) {
          std::move( from, middle, buffer.begin() );
          std::move( middle, to, from );
          std::move( buffer.begin(), buffer.begin() + l_size, to - l_size );
          break;
        }
        std::size_t shift =
          rotate_left< batch_size, value_type >( from, middle, to );
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
        std::size_t shift =
          rotate_right< batch_size, value_type >( from, middle, to );
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
    
} block_async_cycle;
