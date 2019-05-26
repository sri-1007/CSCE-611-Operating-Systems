/* 
    File: machine.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 12/09/04

    LOW-LEVEL MACHINE FUNCTIONS.

*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"
#include "machine_low.H"

#include "assert.H"

/*--------------------------------------------------------------------------*/
/* INTERRUPTS */
/*--------------------------------------------------------------------------*/

bool Machine::interrupts_enabled() {
  /* We check the IF flag (INTERRUPT ENABLE) in the EFLAGS status register. */
  return get_EFLAGS() & (1 << 9);
}

void Machine::enable_interrupts() {
  assert(!interrupts_enabled());
  __asm__ __volatile__ ("sti");
}

void Machine::disable_interrupts() {
  assert(interrupts_enabled());
  __asm__ __volatile__ ("cli");
}

/*--------------------------------------------------------------------------*/
/* PORT I/O OPERATIONS  */ 
/*--------------------------------------------------------------------------*/

/* We will use this later on for reading from the I/O ports to get data
*  from devices such as the keyboard. We are using what is called
*  'inline assembly' in these routines to actually do the work */
char Machine::inportb (unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

unsigned short Machine::inportw (unsigned short _port) {
    unsigned short rv;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void Machine::outportb (unsigned short _port, char _data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void Machine::outportw (unsigned short _port, unsigned short _data) {
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
}
