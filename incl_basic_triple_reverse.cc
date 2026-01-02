constexpr static struct basic_triple_reverse_t {
public:

  template < typename Iterator >
  Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
    std::size_t const r_size = to - middle;
      
    std::reverse( from, middle );
    std::reverse( middle, to );
    std::reverse( from, to );
      
    return ( from + r_size );
  }
    
} basic_triple_reverse;
