/* 
    File: gdt.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/02

    Description: Management of the Global Descriptor Table (GDT)

    The GDT describes memory access priviledges for memory segments.

    While the table is initialized by GRUB already, it may be a good idea to 
    do this again in the kernel code.

    For details see Section 5 of Brandon Friesen's Tutotial 
    on OS Kernel Development.
    URL: http://www.osdever.net/bkerndev/Docs/title.htm

*/

#ifndef _GDT_H_                   // include file only once
#define _GDT_H_

/*--------------------------------------------------------------------------*/
/* GDT */ 
/*--------------------------------------------------------------------------*/

class GDT {

private:

  /* Use this function to set up an entry in the GDT. */
  static void set_gate(int num, 
                       unsigned long base, unsigned long limit, 
                       unsigned char access, unsigned char gran);

public:

  static const unsigned int SIZE = 3;

  static void init();
  /* Initialize the GDT to have a null segment, a code segment, 
     and one data segment. */

};

#endif
