constexpr static struct inplace_merge_t {
private:
  
  template < typename Rotate >
  struct worker {

    constexpr static std::size_t buffer_size = rotation::BUFFER_SIZE;

    constexpr static auto make_buffer = [] ( auto from, auto to ) {
      using value_type = std::decay_t< decltype( *from ) >;
      thread_local std::array< value_type, buffer_size > the_buffer;
      std::move( from, to, the_buffer.begin() );
      return ( the_buffer.begin() );
    };
    
    Rotate rotate;
    
    template < typename Iterator, typename IsLess >
    void operator () ( Iterator from, Iterator middle, Iterator to, IsLess is_less ) const {
      // ENFORCE( std::is_sorted( from, middle, is_less ) );
      // ENFORCE( std::is_sorted( middle, to, is_less ) );
      // PROMISE( std::is_sorted( from, to, is_less ) );
      std::size_t length = to - from;
      std::size_t l_size = middle - from;
      std::size_t r_size = to - middle;
      if ( from == middle || middle == to ) {
      } else if ( ! is_less( middle[ 0 ], middle[ -1 ] ) ) {
      } else if ( l_size <= buffer_size ) {
        auto buffer = make_buffer( from, middle );
        basic_merge_bottom_up( buffer, buffer + l_size, middle, to, from, is_less );
      } else if ( r_size <= buffer_size ) {
        auto buffer = make_buffer( middle, to );
        basic_merge_top_down( buffer, buffer + r_size, from, middle, to, is_less );
      } else if ( l_size >= length / 2 ) {
        Iterator left_half = from + l_size / 2;
        Iterator right_split = std::lower_bound( middle, to, *left_half, is_less );
        middle = rotate( left_half, middle, right_split );
        (*this)( from, left_half, middle, is_less );
        (*this)( middle, right_split, to, is_less );
      } else {
        Iterator right_half = middle + r_size / 2;
        Iterator left_split = std::upper_bound( from, middle, right_half[ - 1 ], is_less );
        middle = rotate( left_split, middle, right_half );
        (*this)( from, left_split, middle, is_less );
        (*this)( middle, right_half, to, is_less );
      }
    }
    
  };
  
public:
  
  template < typename Iterator, typename IsLess >
  void operator () ( Iterator from, Iterator middle, Iterator to, IsLess is_less ) const {
    (*this)[ rotation::block_cycle ]( from, middle, to, is_less );
  }
  
  template < typename Rotate >
  constexpr worker< Rotate > operator [] ( Rotate rotate ) const {
    return { rotate };
  }
  
} inplace_merge;
