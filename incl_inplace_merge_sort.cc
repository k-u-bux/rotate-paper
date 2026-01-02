template < typename Iterator, typename IsLess >
void inplace_merge_sort ( Iterator from, Iterator to, IsLess is_less,
                          std::size_t bound = SOME_BOUND ) {
  std::size_t length = to - from;
  if ( length <= bound ) {
    SOME_SMALL_SORT( from, to, is_less );
  } else {
    Iterator middle = from + length / 2;
    inplace_merge_sort( from, middle, is_less, bound );
    inplace_merge_sort( middle, to, is_less, bound );
    inplace_merge[ ROTATE ]( from, middle, to, is_less );
  }
}
