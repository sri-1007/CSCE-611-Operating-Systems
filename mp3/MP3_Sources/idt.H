/* 
    File: idt.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/02

    Description: the Interrupt Description Table (IDT) 

    The IDT contains pointer to low-level exception and interrupt handlers.
    The way the exception handling is set up, all low-level handlers route
    the exception to a single exception dispatcher, which in turn 
    calls a high-level exception dispatcher (see file 'exceptions.H').

    For details see Section 6 of Brandon Friesen's Tutorial 
    on OS Kernel Development.
    URL: http://www.osdever.net/bkerndev/Docs/title.htm 

*/

#ifndef _IDT_H_                   // include file only once
#define _IDT_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

//#define IDT_SIZE 256

/*--------------------------------------------------------------------------*/
/* Class IDT */ 
/*--------------------------------------------------------------------------*/

class IDT {

  public:

  static const int SIZE = 256;

  static void init();
  /* Initialize the IDT, and fill the 32 first entries with pointers to handle
     the 32 Intel-defined exceptions. After initializing the IDT, these exceptions
     are routed to the exception dispatcher (see 'exceptions.H'). At this point,
     no exception handlers are installed yet.
  */

  static void set_gate(unsigned char  num, unsigned long base, 
                       unsigned short sel, unsigned char flags);
  /* Used to install a low-level exception handler in the IDT. For high-level
     exception handlers, use the exception management framework defined in 
     file 'exceptions.H'.
  */

};

#endif
