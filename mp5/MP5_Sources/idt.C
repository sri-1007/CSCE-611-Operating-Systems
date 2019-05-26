/* 
    File: idt.C

    Date  : 09/03/02

*/

/* Based largely on 
*  bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk. */


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

//#include "assert.H"
#include "utils.H"
#include "idt.H"
#include "console.H"

/*--------------------------------------------------------------------------*/
/* EXTERNS */ 
/*--------------------------------------------------------------------------*/

/* Used to load our IDT, defined in 'idt_low.s' */
extern "C" void idt_load();

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));


/*--------------------------------------------------------------------------*/
/* VARIABLES */
/*--------------------------------------------------------------------------*/


/* Declare an IDT of IDT_SIZE entries. We use only the first 32
   entries. If any undefined IDT entry is hit, it normally
   cause an "Unhandled Interrupt" exception. Any descriptor
   for which the 'presence' bit is cleared will generate an
   "Unhandled Interrupt" exception. */
struct idt_entry idt[IDT::SIZE];
struct idt_ptr idtp;

/*--------------------------------------------------------------------------*/
/* HOOKING UP THE LOW-LEVEL EXCEPTION HANDLER TO EXCEPTIONDISPATCHER.       */
/*--------------------------------------------------------------------------*/

/* Use this function to set an entry in the IDT. */
void IDT::set_gate(unsigned char num, unsigned long base, 
                   unsigned short sel, unsigned char flags) {

    Console::puts("Installing handler in IDT position ");
    Console::puti((int)num);
    Console::puts("\n");

    /* The interrupt routine's base address */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;

}

/* Installs the IDT */
void IDT::init() {

  /* Sets the special IDT pointer up. */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

  /* Clear out the entire IDT, initializing it to zeros. */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);


  /* Points the processor's internal register to the new IDT */
  idt_load();
}
