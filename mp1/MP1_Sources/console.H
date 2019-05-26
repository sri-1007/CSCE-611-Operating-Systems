/* 
    File: Console.H

    Author  : R. Bettati
              Department of Computer Science
              Texas A&M University
    Date    : 09/02/12

    The class Console encapsulates the output operations ot the console 
    screen. Since the console is initialized at the very beginning of the
    boot-up of the operating system, when no memory management is available
    yet, all functions and storage for the console are static. 

    The console is initialized with an "init" function instead of a 
    constructor. (We don't want to deal with constructors that are called 
    before the "main()" function.)

    By making all functions static, we can access them across all object 
    files without having to declare a global Console object or pass pointers
    to a locally declared object.

*/

#ifndef _Console_H_                   // include file only once
#define _Console_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

/* -- (none -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef enum {
   BLACK         = 0,
   BLUE          = 1,
   GREEN         = 2,
   CYAN          = 3, 
   RED	         = 4,
   MAGENTA       = 5,
   BROWN         = 6,
   LIGHT_GREY    = 7,
   DARK_GREY     = 8,  	
   LIGHT_BLUE    = 9, 	
   LIGHT_GREEN   = 10, 	
   LIGHT_CYAN    = 11, 	
   LIGHT_RED     = 12, 	
   LIGHT_MAGENTA = 13, 	
   LIGHT_BROWN   = 14, 	
   WHITE	 = 15 	
} COLOR_CODE;


/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   C o n s o l e */
/*--------------------------------------------------------------------------*/

class Console {
private:
  static int attrib;                  /* background and foreground color */
  static int csr_x;                   /* position of cursor              */
  static int csr_y;
  static unsigned short * textmemptr; /* text pointer */
public:
  
  /* -- INITIALIZER (we have no constructor, there is no memory mgmt yet.) */
  static void init(unsigned char _fore_color = WHITE, 
                   unsigned char _back_color = BLACK);
  
  static void scroll();

  static void move_cursor();
  /* Update the hardware cursor. */

  static void cls();
  /* Clear the screen. */

  static void putch(const char _c);
  /* Put a single character on the screen. */

  static void puts(const char * _s);
  /* Display a NULL-terminated string on the screen.*/

  static void puti(const int _i);
  /* Display a integer on the screen.*/

  static void putui(const unsigned int _u);
  /* Display a unsigned integer on the screen.*/

  static void set_TextColor(unsigned char _fore_color, unsigned char _back_color);
  /* Set the color of the foreground and background. */

};


#endif


