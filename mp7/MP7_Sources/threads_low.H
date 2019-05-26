/* 
    File: threads_low.h

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/28


    Low-level context-switch operations for threads management.

*/

#ifndef _threads_low_H_                   // include file only once
#define _threads_low_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "thread.H"

/*--------------------------------------------------------------------------*/
/* LOW-LEVEL CONTEXT SWITCH ROUTINES  */
/*--------------------------------------------------------------------------*/

/* The low-level functions (defined in file 'threads_low.asm') that handle the
   low-level function context switch functions. */


extern "C" void threads_low_switch_to(Thread * _thread);
/* Switches the execution to the given thread. If the calling entity is a thread,
   the function returns after the calling thread has been switched back in.
*/

extern "C" unsigned long get_EFLAGS(); 
/* Return value of the EFLAGS status register. */

#endif

