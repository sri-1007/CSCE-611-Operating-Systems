/* 
    File: exceptions.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 12/09/05

*/
 
/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "utils.H"
#include "console.H"
#include "idt.H"
#include "exceptions.H"

/*--------------------------------------------------------------------------*/
/* EXTERNS */
/*--------------------------------------------------------------------------*/

/* The low-level functions (defined in file 'IDT::low.s') that handle the
   32 Intel-defined CPU exceptions.
   These functions are actually merely stubs that put the error code and 
   the exception code on the stack and then call a low-level function, which
   in turn calls the exception dispatcher (defined in 'exceptions.H').
   Yes, there are more efficient ways to handle exceptions, but they require more
   code replication.
*/
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

extern "C" void lowlevel_dispatch_exception(REGS * _r) {
  ExceptionHandler::dispatch_exception(_r);
}

/*--------------------------------------------------------------------------*/
/* STATIC VARIABLES */
/*--------------------------------------------------------------------------*/

ExceptionHandler * ExceptionHandler::handler_table[ExceptionHandler::EXCEPTION_TABLE_SIZE];
  
/*--------------------------------------------------------------------------*/
/* EXPORTED EXCEPTION DISPATCHER FUNCTIONS */
/*--------------------------------------------------------------------------*/

void ExceptionHandler::init_dispatcher() {

  /* -- INITIALIZE LOW-LEVEL EXCEPTION HANDLERS */
  /*    Add any new ISRs to the IDT here using IDT::set_gate */
  IDT::set_gate( 0, (unsigned) isr0, 0x08, 0x8E);
  IDT::set_gate( 1, (unsigned) isr1, 0x08, 0x8E);
  IDT::set_gate( 2, (unsigned) isr2, 0x08, 0x8E);
  IDT::set_gate( 3, (unsigned) isr3, 0x08, 0x8E);
  IDT::set_gate( 4, (unsigned) isr4, 0x08, 0x8E);
  IDT::set_gate( 5, (unsigned) isr5, 0x08, 0x8E);
  IDT::set_gate( 6, (unsigned) isr6, 0x08, 0x8E);
  IDT::set_gate( 7, (unsigned) isr7, 0x08, 0x8E);

  IDT::set_gate( 8, (unsigned) isr8, 0x08, 0x8E);
  IDT::set_gate( 9, (unsigned) isr9, 0x08, 0x8E);
  IDT::set_gate(10, (unsigned)isr10, 0x08, 0x8E);
  IDT::set_gate(11, (unsigned)isr11, 0x08, 0x8E);
  IDT::set_gate(12, (unsigned)isr12, 0x08, 0x8E);
  IDT::set_gate(13, (unsigned)isr13, 0x08, 0x8E);
  IDT::set_gate(14, (unsigned)isr14, 0x08, 0x8E);
  IDT::set_gate(15, (unsigned)isr15, 0x08, 0x8E);

  IDT::set_gate(16, (unsigned)isr16, 0x08, 0x8E);
  IDT::set_gate(17, (unsigned)isr17, 0x08, 0x8E);
  IDT::set_gate(18, (unsigned)isr18, 0x08, 0x8E);
  IDT::set_gate(19, (unsigned)isr19, 0x08, 0x8E);
  IDT::set_gate(20, (unsigned)isr20, 0x08, 0x8E);
  IDT::set_gate(21, (unsigned)isr21, 0x08, 0x8E);
  IDT::set_gate(22, (unsigned)isr22, 0x08, 0x8E);
  IDT::set_gate(23, (unsigned)isr23, 0x08, 0x8E);

  IDT::set_gate(24, (unsigned)isr24, 0x08, 0x8E);
  IDT::set_gate(25, (unsigned)isr25, 0x08, 0x8E);
  IDT::set_gate(26, (unsigned)isr26, 0x08, 0x8E);
  IDT::set_gate(27, (unsigned)isr27, 0x08, 0x8E);
  IDT::set_gate(28, (unsigned)isr28, 0x08, 0x8E);
  IDT::set_gate(29, (unsigned)isr29, 0x08, 0x8E);
  IDT::set_gate(30, (unsigned)isr30, 0x08, 0x8E);
  IDT::set_gate(31, (unsigned)isr31, 0x08, 0x8E);

  /* -- INITIALIZE THE HIGH-LEVEL EXCEPTION HANDLER */
  int i;
  for(i = 0; i < EXCEPTION_TABLE_SIZE; i++) {
    handler_table[i] = NULL;
  }
}

void ExceptionHandler::dispatch_exception(REGS * _r) {

  /* -- EXCEPTION NUMBER */
  unsigned int exc_no = _r->int_no;

  Console::puts("EXCEPTION DISPATCHER: exc_no = ");
  Console::putui(exc_no);
  Console::puts("\n");

  assert((exc_no >= 0) && (exc_no < EXCEPTION_TABLE_SIZE));

  /* -- HAS A HANDLER BEEN REGISTERED FOR THIS EXCEPTION NO? */
  ExceptionHandler * handler = handler_table[exc_no];

  if (!handler) {
    /* --- NO HANDLER HAS BEEN REGISTERED. SIMPLY RETURN AN ERROR. */
    Console::puts("NO DEFAULT EXCEPTION HANDLER REGISTERED\n");
    abort();
  }
  else {
    /* -- HANDLE THE EXCEPTION OR INTERRUPT */
    handler->handle_exception(_r);
  }

}

void ExceptionHandler::register_handler(unsigned int       _isr_code,
                                        ExceptionHandler * _handler) {

  assert(_isr_code >= 0 && _isr_code < EXCEPTION_TABLE_SIZE);

  handler_table[_isr_code] = _handler;

  Console::puts("Installed exception handler at ISR "); 
  Console::putui(_isr_code); 
  Console::puts("\n");

}

void ExceptionHandler::deregister_handler(unsigned int    _isr_code) {
  assert(_isr_code >= 0 && _isr_code < EXCEPTION_TABLE_SIZE);

  handler_table[_isr_code] = NULL;

  Console::puts("UNINSTALLED exception handler at ISR "); 
  Console::putui(_isr_code); 
  Console::puts("\n");

}


