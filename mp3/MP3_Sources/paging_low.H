/* 
    File: paging_low.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/28


    Low-level register operations for x86 paging subsystem.

*/

#ifndef _paging_low_H_                   // include file only once
#define _paging_low_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- none -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* LOW-LEVEL PAGING ROUTINES  */
/*--------------------------------------------------------------------------*/

/* The low-level functions (defined in file 'paging_low.asm') that handle the
   low-level function to manage the page tables. */


/* -- CR0 -- */
extern "C" unsigned long read_cr0();
extern "C" void write_cr0(unsigned long _val);

/* -- CR2 -- */
extern "C" unsigned long read_cr2();

/* -- CR3 -- */
extern "C" unsigned long read_cr3();
extern "C" void write_cr3(unsigned long _val);


#endif


