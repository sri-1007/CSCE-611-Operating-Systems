/* 
    File: irq.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/02

    Description: Interrupt Table (IRQ) 

    For details see Section 7 of Brandon Friesen's Tutorial 
    on OS Kernel Development.
    URL: http://www.osdever.net/bkerndev/Docs/title.htm 

*/

#ifndef _IRQ_H_                   // include file only once
#define _IRQ_H_


/*--------------------------------------------------------------------------*/
/*  */ 
/*--------------------------------------------------------------------------*/

class IRQ {

public:

  static void init();
  /* Initialize the IRQ handlers, i.e. fill 16 entries with pointers to handle
     the PIC generated interrupts. These interrupts are routed to the exception 
     dispatcher (see 'exceptions.H'). At this point, no exception handlers are 
     installed yet.
  */

};

#endif
