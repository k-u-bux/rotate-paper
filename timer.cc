// timer.cc (C) Kai-Uwe Bux [2024-2025]
// ====================================

#ifndef TIMER
#define TIMER

#include <chrono>

// timer stuff
// ===========

template < typename Clock = std::chrono::high_resolution_clock >
class timer {
private:
  
  typename Clock::time_point the_start;
  
public:
  
  timer ( void )
    : the_start ( Clock::now() )
  {}
  
  typename Clock::duration passed ( void ) const {
    return( Clock::now() - the_start );
  }
  
}; // timer


template < typename Clock = std::chrono::high_resolution_clock >
class accumulating_timer {
  
  typename Clock::duration & the_duration;
  timer< Clock > the_timer;
  
public:
  
  accumulating_timer ( typename Clock::duration & duration )
    : the_duration ( duration )
    , the_timer ()
  {}
  
  ~accumulating_timer ( void ) {
    the_duration += the_timer.passed();
  }
  
}; // accumulating_timer

template < typename Clock = std::chrono::high_resolution_clock >
class subtracting_timer {
  
  typename Clock::duration & the_duration;
  timer< Clock > the_timer;
  
public:
  
  subtracting_timer ( typename Clock::duration & duration )
    : the_duration ( duration )
    , the_timer ()
  {}
  
  ~subtracting_timer ( void ) {
    the_duration -= the_timer.passed();
  }
  
}; // subtracting_timer

#endif // TIMER

// end of file
