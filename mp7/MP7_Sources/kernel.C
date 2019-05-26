/*
    File: kernel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2017/05/01


    This file has the main entry point to the operating system.

    MAIN FILE FOR MACHINE PROBLEM "FILE SYSTEM"

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- COMMENT/UNCOMMENT THE FOLLOWING LINE TO EXCLUDE/INCLUDE SCHEDULER CODE */

//#define _USES_SCHEDULER_
/* This macro is defined when we want to force the code below to use 
   a scheduler.
   Otherwise, no scheduler is used, and the threads pass control to each 
   other in a co-routine fashion.
*/

#define MB * (0x1 << 20)
#define KB * (0x1 << 10)

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

#include "thread.H"         /* THREAD MANAGEMENT */

#ifdef _USES_SCHEDULER_
#include "scheduler.H"       /* WE MAY NEED A SCHEDULER IF WE USE BlockingDisk */
#endif

#include "simple_disk.H"     /* DISK DEVICE */

#include "file_system.H"     /* FILE SYSTEM */
#include "file.H"

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
/* SCHEDULER */
/*--------------------------------------------------------------------------*/

#ifdef _USES_SCHEDULER_

/* -- A POINTER TO THE SYSTEM SCHEDULER */
Scheduler * SYSTEM_SCHEDULER;

#endif

/*--------------------------------------------------------------------------*/
/* DISK */
/*--------------------------------------------------------------------------*/

/* -- A POINTER TO THE SYSTEM DISK */
SimpleDisk * SYSTEM_DISK;

#define SYSTEM_DISK_SIZE (10 MB)

/*--------------------------------------------------------------------------*/
/* FILE SYSTEM */
/*--------------------------------------------------------------------------*/

/* -- A POINTER TO THE SYSTEM FILE SYSTEM */
FileSystem * FILE_SYSTEM;

/*--------------------------------------------------------------------------*/
/* JUST AN AUXILIARY FUNCTION */
/*--------------------------------------------------------------------------*/

void pass_on_CPU(Thread * _to_thread) {

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
/* CODE TO EXERCISE THE FILE SYSTEM */
/*--------------------------------------------------------------------------*/

void exercise_file_system(FileSystem * _file_system) {
    
    const char * STRING1 = "01234567890123456789";
    const char * STRING2 = "abcdefghijabcdefghij";
    
    /* -- Create two files -- */
    
    assert(_file_system->CreateFile(1));
    assert(_file_system->CreateFile(2));
    
    /* -- "Open" the two files -- */
    
    File * file1 = _file_system->LookupFile(1);
    assert(file1 != NULL);
    
    File * file2 = _file_system->LookupFile(2);
    assert(file2 != NULL);
    
    /* -- Write into File 1 -- */
    file1->Rewrite();
    file1->Write(20, STRING1);
    
    /* -- Write into File 2 -- */
    
    file2->Rewrite();
    file2->Write(20, STRING2);
    
    /* -- "Close" files -- */
    delete file1;
    delete file2;
    
    /* -- "Open files again -- */
    file1 = _file_system->LookupFile(1);
    file2 = _file_system->LookupFile(2);
    
    /* -- Read from File 1 and check result -- */
    file1->Reset();
    char result1[30];
    assert(file1->Read(20, result1) == 20);
    for(int i = 0; i < 20; i++) {
        assert(result1[i] == STRING1[i]);
    }
    
    /* -- Read from File 2 and check result -- */
    file2->Reset();
    char result2[30];
    assert(file2->Read(20, result2) == 20);
    for(int i = 0; i < 20; i++) {
        assert(result2[i] == STRING2[i]);
    }
    
    /* -- "Close" files again -- */
    delete file1;
    delete file2;
    
    /* -- Delete both files -- */
    assert(_file_system->DeleteFile(1));
    assert(_file_system->DeleteFile(2));
    
}

/*--------------------------------------------------------------------------*/
/* A FEW THREADS (pointer to TCB's and thread functions) */
/*--------------------------------------------------------------------------*/

Thread * thread1;
Thread * thread2;
Thread * thread3;
Thread * thread4;

void fun1() {
    Console::puts("THREAD: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");

    Console::puts("FUN 1 INVOKED!\n");

    for(int j = 0;; j++) {

       Console::puts("FUN 1 IN ITERATION["); Console::puti(j); Console::puts("]\n");

        for (int i = 0; i < 10; i++) {
           Console::puts("FUN 1: TICK ["); Console::puti(i); Console::puts("]\n");
       }

        /* -- Give up the CPU */
       pass_on_CPU(thread2);
    }
}

void fun2() {
    Console::puts("THREAD: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");

    Console::puts("FUN 2 INVOKED!\n");

    for(int j = 0;; j++) {

       Console::puts("FUN 2 IN ITERATION["); Console::puti(j); Console::puts("]\n");

       for (int i = 0; i < 10; i++) {
           Console::puts("FUN 2: TICK ["); Console::puti(i); Console::puts("]\n");
       }
        
       /* -- Give up the CPU */
       pass_on_CPU(thread3);
    }
}

void fun3() {
    Console::puts("THREAD: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");

    Console::puts("FUN 3 INVOKED! <THIS THREAD EXERCISES THE FILE SYSTEM> \n");

    assert(FileSystem::Format(SYSTEM_DISK, (1 MB)));
    
    assert(FILE_SYSTEM->Mount(SYSTEM_DISK));
           
    for(int j = 0;; j++) {
        
        Console::puts("FUN 4 IN BURST["); Console::puti(j); Console::puts("]\n");
        
        exercise_file_system(FILE_SYSTEM);
        
        /* -- Give up the CPU */
        pass_on_CPU(thread4);
    }

}

void fun4() {
    Console::puts("THREAD: "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");

    for(int j = 0;; j++) {

       Console::puts("FUN 4 IN BURST["); Console::puti(j); Console::puts("]\n");

       for (int i = 0; i < 10; i++) {
           Console::puts("FUN 4: TICK ["); Console::puti(i); Console::puts("]\n");
       }

        /* -- Give up the CPU */
       pass_on_CPU(thread1);
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

    /* -- MEMORY ALLOCATOR SET UP. WE CAN NOW USE NEW/DELETE! -- */
    
    /* -- INITIALIZE THE TIMER (we use a very simple timer).-- */

    /* Question: Why do we want a timer? We have it to make sure that 
                 we enable interrupts correctly. If we forget to do it,
                 the timer "dies". */

    SimpleTimer timer(100); /* timer ticks every 10ms. */
    InterruptHandler::register_handler(0, &timer);
    /* The Timer is implemented as an interrupt handler. */

#ifdef _USES_SCHEDULER_

    /* -- SCHEDULER -- IF YOU HAVE ONE -- */
  
    SYSTEM_SCHEDULER = new Scheduler();
    
#endif

    /* -- DISK DEVICE -- */

    SYSTEM_DISK = new SimpleDisk(MASTER, SYSTEM_DISK_SIZE);
    
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
