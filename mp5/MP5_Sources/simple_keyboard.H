/* 
    File: simple_keyboard.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2017/02/17

    Implements an interrupt handler for the keyboard.
    The function is implemented in 'handle_interrupt'.

*/

#ifndef _SIMPLE_KEYBOARD_H_
#define _SIMPLE_KEYBOARD_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "interrupts.H"

/*--------------------------------------------------------------------------*/
/* S I M P L E   K E Y B O A R D */
/*--------------------------------------------------------------------------*/

class SimpleKeyboard : public InterruptHandler {

public :

  SimpleKeyboard();
  /* Initialize the simple keyboard. */

  virtual void handle_interrupt(REGS *_r);
  /* This must be installed as the interrupt handler for the keyboard
     when the system gets initialized. (e.g. in "kernel.C")  
  */

  static void init();

  static void wait();
  /* Wait until keyboard is pressed. The implementation is based
     on busy looping! */

  static char read();
  /* Wait until keyboard is pressed and return the keycode. 
     Note: The keycode is not the same as the character! 
     Note2: This is a very "approximate" implementation. Not complete,
           and likely not correct. Use only under duress!
     The implementation is based on busy looping! */

private:
  bool key_pressed;
  char key_code;
  static SimpleKeyboard kb;    

  static const unsigned short STATUS_PORT = 0x64;
  static const unsigned short DATA_PORT   = 0x60;

};

#endif
