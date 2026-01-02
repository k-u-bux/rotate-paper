constexpr static struct buffering_t {
private:
    
  template < typename Rotate >
  struct worker {

    Rotate rotate;

    template < typename Iterator >
    Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
      std::size_t l_size = middle - from;
      std::size_t r_size = to - middle;
      Iterator result = from + r_size;
        
      if ( middle == from || middle == to ) {
        return ( result );
      }

      constexpr static const unsigned buffer_size = BUFFER_SIZE;
      using value_type = std::decay_t< decltype( *from ) >;
      thread_local std::array< value_type, buffer_size > buffer;
      if ( l_size <= buffer_size ) {
        std::move( from, middle, buffer.begin() );
        std::move( middle, to, from );
        std::move( buffer.begin(), buffer.begin() + l_size, to - l_size );
        return ( result );
      } else if ( r_size <= buffer_size ) {
        std::move( middle, to, buffer.begin() );
        std::move_backward( from, middle, to );
        std::move( buffer.begin(), buffer.begin() + r_size, from );
        return ( result );
      }

      return ( rotate( from, middle, to ) );
    }
      
  };

public:

  template < typename Rotate >
  constexpr worker< Rotate > operator [] ( Rotate rotate ) const {
    return { rotate };
  }
    
} buffering;
