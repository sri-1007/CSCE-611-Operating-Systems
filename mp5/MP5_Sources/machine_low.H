/* 
    File: machine_low.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 12/09/03


    Low-level CPU operations.

*/

#ifndef _machine_low_H_                   // include file only once
#define _machine_low_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

 /* (none) */

/*--------------------------------------------------------------------------*/
/* LOW-LEVER CPU OPERATIONS */
/*--------------------------------------------------------------------------*/

extern "C" unsigned long get_EFLAGS(); 
/* Return value of the EFLAGS status register. */

#endif

