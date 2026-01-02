constexpr static struct checking_t {
private:
    
  template < typename Rotate >
  struct worker {

    Rotate rotate;

    template < typename Iterator >
    Iterator operator () ( Iterator from, Iterator middle, Iterator to ) const {
      if ( middle == from || middle == to ) {
        return ( from + ( to - middle ) );
      }
      return ( rotate( from, middle, to ) );
    }
      
  };

public:

  template < typename Rotate >
  constexpr worker< Rotate > operator [] ( Rotate rotate ) const {
    return { rotate };
  }
    
} checking;
