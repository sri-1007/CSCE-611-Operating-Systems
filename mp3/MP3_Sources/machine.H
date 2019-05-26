/* 
    File: machine.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 12/09/05

    Description: Low-level definitions for x86 architecture.

        - Register context
        - Data and code segment for kernel. 
        - Interrupt enable/disable.

*/

#ifndef _machine_H_                   // include file only once
#define _machine_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* REGISTER CONTEXT ON THE STACK WHEN EXCEPTION/INTERRUPT REACHES EXCEPTION
   DISPATCHER OR INTERRUPT DISPATCHER. 
   THIS IS ALSO USED IN THREAD SCHEDULING (for context switching) */
typedef struct regs {

    /* segment descriptors */
    unsigned int gs;
    unsigned int fs;
    unsigned int es;
    unsigned int ds;

    /* General purpose registers, managed by pusha/popa in x86. */
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;

    /* In the low-level handlers (i.e. before we call the 
       exception or interrupt dispatcher) we push number
       of the interrupt and error code onto the stack. */
    unsigned int int_no;
    unsigned int err_code;

    /* These registers are pushed on the stack when 
       the exception occurs. */
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;

    /* These registers are pushed on the stack only when
       the exception occured in user mode.
       As long as you are in kernel mode, the register context
       is 68 bytes long, and not 76! */
    unsigned int useresp;
    unsigned int ss;    
} REGS;
 

/*--------------------------------------------------------------------------*/
/* CLASS   M a c h i n e */
/*--------------------------------------------------------------------------*/

class Machine {

public:
  
/*---------------------------------------------------------------*/
/* MEMORY MANAGEMENT */
/*---------------------------------------------------------------*/

  static const unsigned int PAGE_SIZE = 4096;
  static const unsigned int PT_ENTRIES_PER_PAGE = 1024;

/*---------------------------------------------------------------*/
/* INTERRUPTS */
/*---------------------------------------------------------------*/

  /*
   This is done by checking the Interrupt Enabled flag in the 
   EFLAG status register and by issuing STI/CLI instructions.
   (NOTE: If we have multiple threads, each has its own status register.)
  */

  static bool interrupts_enabled();
  /* Returns whether interrupts are enabled. */

  static void enable_interrupts();
  static void disable_interrupts();
  /* Issue CLI/STI instructions. */

/*---------------------------------------------------------------*/
/* PORT I/O OPERATIONS */
/*---------------------------------------------------------------*/

  static char inportb  (unsigned short _port);
  static unsigned short inportw (unsigned short _port);
  /* Read data from input port _port.*/

  static void outportb (unsigned short _port, char _data);
  static void outportw (unsigned short _port, unsigned short _data);
  /* Write _data to output port _port.*/

};
#endif
