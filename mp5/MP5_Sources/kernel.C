/*
    File: kernel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 17/04/03


    This file has the main entry point to the operating system.

    MAIN FILE FOR MACHINE PROBLEM "KERNEL-LEVEL THREAD MANAGEMENT"

    NOTE: REMEMBER THAT AT THE VERY BEGINNING WE DON'T HAVE A MEMORY MANAGER. 
          OBJECT THEREFORE HAVE TO BE ALLOCATED ON THE STACK. 
          THIS LEADS TO SOME RATHER CONVOLUTED CODE, WHICH WOULD BE MUCH 
          SIMPLER OTHERWISE.
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- COMMENT/UNCOMMENT THE FOLLOWING LINE TO EXCLUDE/INCLUDE SCHEDULER CODE */

#define _USES_SCHEDULER_
/* This macro is defined when we want to force the code below to use
   a scheduler.
   Otherwise, no scheduler is used, and the threads pass control to each
   other in a co-routine fashion.
*/


/* -- UNCOMMENT THE FOLLOWING LINE TO MAKE THREADS TERMINATING */

#define _TERMINATING_FUNCTIONS_
/* This macro is defined when we want the thread functions to return, and so
   terminate their thread.
   Otherwise, the thread functions don't return, and the threads run forever.
*/

//#define _RR_SCHEDULER_
/* Comment this for FIFO Scheduler and uncomment for RR Scheduler*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"         /* LOW-LEVEL STUFF   */
#include "console.H"
#include "gdt.H"
#include "idt.H"             /* EXCEPTION MGMT.   */
#include "irq.H"
#include "exceptions.H"    
#include "interrupts.H"

#include "simple_timer.H"    /* TIMER MANAGEMENT  */

#include "frame_pool.H"      /* MEMORY MANAGEMENT */
#include "mem_pool.H"

#include "thread.H"          /* THREAD MANAGEMENT */

#ifdef _USES_SCHEDULER_
#include "scheduler.H"
#endif

/*--------------------------------------------------------------------------*/
/* MEMORY MANAGEMENT */
/*--------------------------------------------------------------------------*/

/* -- A POOL OF FRAMES FOR THE SYSTEM TO USE */
FramePool * SYSTEM_FRAME_POOL;

/* -- A POOL OF CONTIGUOUS MEMORY FOR THE SYSTEM TO USE */
MemPool * MEMORY_POOL;

typedef unsigned int size_t;

//replace the operator "new"
void * operator new (size_t size) {
    unsigned long a = MEMORY_POOL->allocate((unsigned long)size);
    return (void *)a;
}

//replace the operator "new[]"
void * operator new[] (size_t size) {
    unsigned long a = MEMORY_POOL->allocate((unsigned long)size);
    return (void *)a;
}

//replace the operator "delete"
void operator delete (void * p) {
    MEMORY_POOL->release((unsigned long)p);
}

//replace the operator "delete[]"
void operator delete[] (void * p) {
    MEMORY_POOL->release((unsigned long)p);
}

/*--------------------------------------------------------------------------*/
/* SCHEDULRE and AUXILIARY HAND-OFF FUNCTION FROM CURRENT THREAD TO NEXT */
/*--------------------------------------------------------------------------*/

#ifdef _USES_SCHEDULER_
/* -- A POINTER TO THE SYSTEM SCHEDULER */
	#ifdef _RR_SCHEDULER_
	RRScheduler *SYSTEM_SCHEDULER;
   #else
    Scheduler *SYSTEM_SCHEDULER;
   #endif
#endif

void pass_on_CPU(Thread * _to_thread) {
  // Hand over CPU from current thread to _to_thread.
  
#ifndef _USES_SCHEDULER_

        /* We don't use a scheduler. Explicitely pass control to the next
           thread in a co-routine fashion. */
	Thread::dispatch_to(_to_thread);

#else

        /* We use a scheduler. Instead of dispatching to the next thread,
           we pre-empt the current thread by putting it onto the ready
           queue and yielding the CPU. */

        SYSTEM_SCHEDULER->resume(Thread::CurrentThread());
        SYSTEM_SCHEDULER->yield();
#endif
}

/*--------------------------------------------------------------------------*/
/* A FEW THREADS (pointer to TCB's and thread functions) */
/*--------------------------------------------------------------------------*/

Thread * thread1;
Thread * thread2;
Thread * thread3;
Thread * thread4;

/* -- THE 4 FUNCTIONS fun1 - fun4 ARE LARGELY IDENTICAL. */

void fun1() {
    Console::puts("Thread: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");
    Console::puts("FUN 1 INVOKED!\n");

#ifdef _TERMINATING_FUNCTIONS_
    for(int j = 0; j < 10; j++) 
#else
    for(int j = 0;; j++) 
#endif
    {	
        Console::puts("FUN 1 IN BURST["); Console::puti(j); Console::puts("]\n");
        for (int i = 0; i < 10; i++) {
            Console::puts("FUN 1: TICK ["); Console::puti(i); Console::puts("]\n");
        }
		#ifndef _RR_SCHEDULER_
        pass_on_CPU(thread2);
		#endif
    }
}


void fun2() {
    Console::puts("Thread: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");
    Console::puts("FUN 2 INVOKED!\n");

#ifdef _TERMINATING_FUNCTIONS_
    for(int j = 0; j < 10; j++) 
#else
    for(int j = 0;; j++) 
#endif  
    {		
        Console::puts("FUN 2 IN BURST["); Console::puti(j); Console::puts("]\n");
        for (int i = 0; i < 10; i++) {
            Console::puts("FUN 2: TICK ["); Console::puti(i); Console::puts("]\n");
        }
		
		#ifndef _RR_SCHEDULER_
		pass_on_CPU(thread3);
		#endif
    }
}

void fun3() {
    Console::puts("Thread: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");
    Console::puts("FUN 3 INVOKED!\n");

    for(int j = 0;; j++) {
        Console::puts("FUN 3 IN BURST["); Console::puti(j); Console::puts("]\n");
        for (int i = 0; i < 10; i++) {
	    Console::puts("FUN 3: TICK ["); Console::puti(i); Console::puts("]\n");
        }
		
		#ifndef _RR_SCHEDULER_
        pass_on_CPU(thread4);
		#endif
    }
}

void fun4() {
    Console::puts("Thread: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");
    Console::puts("FUN 4 INVOKED!\n");

    for(int j = 0;; j++) {
        Console::puts("FUN 4 IN BURST["); Console::puti(j); Console::puts("]\n");
        for (int i = 0; i < 10; i++) {
	    Console::puts("FUN 4: TICK ["); Console::puti(i); Console::puts("]\n");
        }
		
		#ifndef _RR_SCHEDULER_
        pass_on_CPU(thread1);
		#endif
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN ENTRY INTO THE OS */
/*--------------------------------------------------------------------------*/

int main() {

    GDT::init();
    Console::init();
    IDT::init();
    ExceptionHandler::init_dispatcher();
    IRQ::init();
    InterruptHandler::init_dispatcher();

    /* -- EXAMPLE OF AN EXCEPTION HANDLER -- */

    class DBZ_Handler : public ExceptionHandler {
      public:
      virtual void handle_exception(REGS * _regs) {
        Console::puts("DIVISION BY ZERO!\n");
        for(;;);
      }
    } dbz_handler;

    ExceptionHandler::register_handler(0, &dbz_handler);


    /* -- INITIALIZE MEMORY -- */
    /*    NOTE: We don't have paging enabled in this MP. */
    /*    NOTE2: This is not an exercise in memory management. The implementation
                of the memory management is accordingly *very* primitive! */

    /* ---- Initialize a frame pool; details are in its implementation */
    FramePool system_frame_pool;
    SYSTEM_FRAME_POOL = &system_frame_pool;
   
    /* ---- Create a memory pool of 256 frames. */
    MemPool memory_pool(SYSTEM_FRAME_POOL, 256);
    MEMORY_POOL = &memory_pool;

    /* -- MEMORY ALLOCATOR IS INITIALIZED. WE CAN USE new/delete! --*/

    /* -- INITIALIZE THE TIMER (we use a very simple timer).-- */

    /* Question: Why do we want a timer? We have it to make sure that 
                 we enable interrupts correctly. If we forget to do it,
                 the timer "dies". */
#ifndef _RR_SCHEDULER_
    SimpleTimer timer(100); /* timer ticks every 10ms. */
    InterruptHandler::register_handler(0, &timer);
#endif
    /* The Timer is implemented as an interrupt handler. */

#ifdef _USES_SCHEDULER_

    /* -- SCHEDULER -- IF YOU HAVE ONE -- */
    #ifdef  _RR_SCHEDULER_
	  SYSTEM_SCHEDULER = new RRScheduler();
	#else
      SYSTEM_SCHEDULER = new Scheduler();
    #endif

#endif

    /* NOTE: The timer chip starts periodically firing as
             soon as we enable interrupts.
             It is important to install a timer handler, as we
             would get a lot of uncaptured interrupts otherwise. */ 

    /* -- ENABLE INTERRUPTS -- */

    Machine::enable_interrupts();

    /* -- MOST OF WHAT WE NEED IS SETUP. THE KERNEL CAN START. */

    Console::puts("Hello World!\n");

    /* -- LET'S CREATE SOME THREADS... */

    Console::puts("CREATING THREAD 1...\n");
    char * stack1 = new char[1024];
    thread1 = new Thread(fun1, stack1, 1024);
    Console::puts("DONE\n");

    Console::puts("CREATING THREAD 2...");
    char * stack2 = new char[1024];
    thread2 = new Thread(fun2, stack2, 1024);
    Console::puts("DONE\n");

    Console::puts("CREATING THREAD 3...");
    char * stack3 = new char[1024];
    thread3 = new Thread(fun3, stack3, 1024);
    Console::puts("DONE\n");

    Console::puts("CREATING THREAD 4...");
    char * stack4 = new char[1024];
    thread4 = new Thread(fun4, stack4, 1024);
    Console::puts("DONE\n");

#ifdef _USES_SCHEDULER_

    /* WE ADD thread2 - thread4 TO THE READY QUEUE OF THE SCHEDULER. */

    SYSTEM_SCHEDULER->add(thread2);
    SYSTEM_SCHEDULER->add(thread3);
    SYSTEM_SCHEDULER->add(thread4);

#endif

    /* -- KICK-OFF THREAD1 ... */

    Console::puts("STARTING THREAD 1 ...\n");
    Thread::dispatch_to(thread1);

    /* -- AND ALL THE REST SHOULD FOLLOW ... */

    assert(false); /* WE SHOULD NEVER REACH THIS POINT. */

    /* -- WE DO THE FOLLOWING TO KEEP THE COMPILER HAPPY. */
    return 1;
}
