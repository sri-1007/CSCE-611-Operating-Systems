/* 
    File: gdt.C

    Date  : 09/03/02

    Description: Management of the Global Descriptor Table (GDT)

    The GDT describes memory access priviledges for memory segments.

    While the table is initialized by GRUB already, it may be a good idea to 
    do this again in the kernel code.

    For details see Section 5 of Brandon Friesen's Tutorial 
    on OS Kernel Development.
    URL: http://www.osdever.net/bkerndev/Docs/title.htm

*  bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk.
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

//#include "assert.H"
#include "utils.H"
#include "gdt.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* Defines a GDT entry. We use the 'packed' directive to prevent the 
   compiler from optimizing by aligning structure elements at word
   boundaries. */
struct gdt_entry {
  unsigned short limit_low;
  unsigned short base_low;
  unsigned char  base_middle;
  unsigned char  access;
  unsigned char  granularity;
  unsigned char  base_high;
} __attribute__((packed));

/* Special pointer that includes the limit: The max bytes 
   taken up by the GDT, minus 1. Again, this NEEDS to be packed. */
struct gdt_ptr {
  unsigned short limit;
  unsigned int   base;
} __attribute__((packed));

/*--------------------------------------------------------------------------*/
/* VARIABLES */ 
/*--------------------------------------------------------------------------*/

static struct gdt_entry gdt[GDT::SIZE];
struct gdt_ptr gp;

/*--------------------------------------------------------------------------*/
/* EXTERNS */ 
/*--------------------------------------------------------------------------*/

/* This function is defined in 'gdt_low.asm', which in turn is included in 
   'start.asm'. */
extern "C" void gdt_flush();

/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* Use this function to set up an entry in the GDT. */
void GDT::set_gate(int num, 
                   unsigned long base, unsigned long limit, 
                   unsigned char access, unsigned char gran) {

  /* Setup the descriptor base address */
  gdt[num].base_low    = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high   = (base >> 24) & 0xFF;

  /* Setup the descriptor limits */
  gdt[num].limit_low   = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);

  /* Finally, set up the granularity and access flags */
  gdt[num].granularity |= (gran & 0xF0);
  gdt[num].access       = access;
}


/* Installs the GDT */
void GDT::init() {

  /* Sets up the special GDT pointer. */
  gp.limit = (sizeof (struct gdt_entry) * SIZE) - 1;
  gp.base  = (unsigned int)&gdt;

  /* Our NULL descriptor */
  set_gate(0, 0, 0, 0, 0);

  /* The second entry is our Code Segment. The base address
     is 0, the limit is 4GByte, it uses 4kB granularity,
     uses 32-bit opcodes, and is a Code Segment descriptor.
     Please check the GDT section in Bran's Kernel Development
     tutorial to see exactly what each value means. */
  set_gate(1, 0, 0xFFFFFFFF, 0x9a, 0xCF);

  /* The third entry is our Data Segment. It's EXACTLY the
     same as the code segment, but the descriptor type in 
     this entry's access byte says it's a Data Segment. */
  set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

  /* Flush out the old GDT, and install the new changes. */
  gdt_flush();
}
