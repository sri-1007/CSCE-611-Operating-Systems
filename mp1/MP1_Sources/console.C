/* 
    File: Console.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/02/2009

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define CONSOLE_START_ADDRESS (unsigned short *)0xB8000

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "console.H"

#include "utils.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   C o n s o l e */
/*--------------------------------------------------------------------------*/

/* -- GLOBAL VARIABLES -- */

 int Console::attrib;                  /* background and foreground color */
 int Console::csr_x;                   /* position of cursor              */
 int Console::csr_y;
 unsigned short * Console::textmemptr; /* text pointer */

/* -- CONSTRUCTOR -- */

void Console::init(unsigned char _fore_color,
                   unsigned char _back_color) {
    set_TextColor(_fore_color, _back_color);
    csr_x  = 0;
    csr_y  = 0;
    textmemptr = CONSOLE_START_ADDRESS;
    cls();
}


void Console::scroll() {

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    unsigned blank = 0x20 | (attrib << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(csr_y >= 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        unsigned temp = csr_y - 25 + 1;
        memcpy ((char*)textmemptr, (char*)textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}


void Console::move_cursor() {
    
    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    unsigned temp = csr_y * 80 + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  Console Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, (char)14);
    //outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    //outportb(0x3D5, (char)temp);
}

/* Clear the screen */
void Console::cls() {

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    unsigned blank = 0x20 | (attrib << 8);

    /* Sets the entire screen to spaces in our current
    *  color */
    for(int i = 0; i < 25; i++) 
        memsetw (textmemptr + i * 80, blank, 80);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    move_cursor();
}

/* Puts a single character on the screen */
void Console::putch(const char _c){
 

    /* Handle a backspace, by moving the cursor back one space */
    if(_c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(_c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(_c == '\r')
    {
        csr_x = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(_c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(_c >= ' ')
    {
        unsigned short * where = textmemptr + (csr_y * 80 + csr_x);
        *where = _c | (attrib << 8);	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_cursor();
}

/* Uses the above routine to output a string... */
void Console::puts(const char * _s) {

    for (int i = 0; i < strlen(_s); i++) {
        putch(_s[i]);
    }
}

void Console::puti(const int _n) {
  char foostr[15];

  int2str(_n, foostr);
  puts(foostr);
}

void Console::putui(const unsigned int _n) {
  char foostr[15];

  uint2str(_n, foostr);
  putch('<');
  puts(foostr);
  putch('>');
}


/* -- COLOR CONTROL -- */
void Console::set_TextColor(const unsigned char _forecolor, 
                            const unsigned char _backcolor) {
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (_backcolor << 4) | (_forecolor & 0x0F);
}

