/* 
    File: simple_timer.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/19

    This is a simple example of a hardware interrupt handler.
    As an example, this implements a timer, which in turn 
    triggers a function to be called at the given frequency.
    The function is implemented in 'handle_interrupt'.

*/

#ifndef _SIMPLE_TIMER_H_
#define _SIMPLE_TIMER_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "interrupts.H"

/*--------------------------------------------------------------------------*/
/* S I M P L E   T I M E R  */
/*--------------------------------------------------------------------------*/

class SimpleTimer : public InterruptHandler {

private:

  /* How long has the system been running? */
  unsigned long seconds; 
  int           ticks;   /* ticks since last "seconds" update.    */

  /* At what frequency do we update the ticks counter? */
  int hz;                /* Actually, by defaults it is 18.22Hz.
                            In this way, a 16-bit counter wraps
                            around every hour.                    */

  void set_frequency(int _hz);
  /* Set the interrupt frequency for the simple timer. */

public :

  SimpleTimer(int _hz);
  /* Initialize the simple timer, and set its frequency. */

  virtual void handle_interrupt(REGS *_r);
  /* This must be installed as the interrupt handler for the timer 
     when the system gets initialized. (e.g. in "kernel.C")  
  */

  void current(unsigned long * _seconds, int * _ticks);
  /* Return the current "time" since the system started. */

  void wait(unsigned long _seconds);
  /* Wait for a particular time to be passed. The implementation is based 
     on busy looping! */

};

#endif
