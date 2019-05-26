/* 
    File: irq.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/05

    This code borrows heavily from Brandon Friesen's the code in Brandon
    Friesen's OS Development Tutorial.
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define IRQ_BASE 32

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"
#include "irq.H"

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS                                                   .      */
/*--------------------------------------------------------------------------*/

/* Normally, IRQs 0 to 7 are mapped to IDT entries 8 to 15.
   For a variety of reasons it is a good idea to re-map these 
   IRQs to different locations 
   We send a sequence of commands to the PICs - 8259's - in order
   to have IRQ0 to IRQ15 be remapped to IDT entries 32 to 47.
*/
static void irq_remap()
{
    Machine::outportb(0x20, 0x11);
    Machine::outportb(0xA0, 0x11);
    Machine::outportb(0x21, 0x20);
    Machine::outportb(0xA1, 0x28);
    Machine::outportb(0x21, 0x04);
    Machine::outportb(0xA1, 0x02);
    Machine::outportb(0x21, 0x01);
    Machine::outportb(0xA1, 0x01);
    Machine::outportb(0x21, 0x0);
    Machine::outportb(0xA1, 0x0);
}


/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTIONS                                                .      */
/*--------------------------------------------------------------------------*/

void IRQ::init() {

  irq_remap();


}
