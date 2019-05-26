/*
 File: scheduler.C
 
 Author:
 Date  :
 
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "scheduler.H"
#include "thread.H"
#include "console.H"
#include "utils.H"
#include "assert.H"
#include "simple_keyboard.H"
#include "machine.H"

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
/* METHODS FOR CLASS   S c h e d u l e r  */
/*--------------------------------------------------------------------------*/

Scheduler::Scheduler() {
  /* Queue object creation using new taken care by enqueue in Queue class*/
  queueSize = 0;
  Console::puts("Constructed Scheduler.\n");
}

void Scheduler::yield() {
/*Removing the first thread from the queue. Interrupts have to be disabled whenever we try to do operations in the ready queue,
particularly for RR Scheduling. As once the thread is excuting, a time quanta is finished either while yielding or adding or resuming
to another thread, then an interrupt would be generated. It is necessary to disable the interrupts and continue processing the ready queue*/
if (Machine::interrupts_enabled())	
	Machine::disable_interrupts();
  
  if(queueSize!=0)
  {
	--queueSize;
	Thread *currentThread = readyQueue.dequeue(); 	
	if(!Machine::interrupts_enabled())
		Machine::enable_interrupts();
    Thread::dispatch_to(currentThread);
  }
	
}

void Scheduler::resume(Thread * _thread) {
if (Machine::interrupts_enabled())	
	Machine::disable_interrupts();
		
  /*Adding the thread to the queue*/
  readyQueue.enqueue(_thread);
  ++queueSize;
  
  if(!Machine::interrupts_enabled())
	Machine::enable_interrupts();
}

void Scheduler::add(Thread * _thread) {
	if (Machine::interrupts_enabled())	
	Machine::disable_interrupts();
/*Adding the thread to the queue*/
  readyQueue.enqueue(_thread);
  ++queueSize; 
 
  if(!Machine::interrupts_enabled())
	Machine::enable_interrupts();
}

void Scheduler::terminate(Thread * _thread) {
  /*Dequeue each thread from the Readyqueue, check if its ID matches with the thread to be searched. If it matches set
  variable. and enqueue back the other threads. 
  eg : 1->2->3->4->5
       2->3->4->5->1
	   3->4->5->1->2
	   if 3 is the thread, beyond that 4,5, will be dequeued and enqueued back thus making the ready queue,
	   1->2->4->5
	   
  Actual method : Use next and previous pointers , find the matching thread and map prev->next = current->next.  */

  
  Console::puts("TERMINATE\n");
bool threadFound = false;
if (Machine::interrupts_enabled())	
	Machine::disable_interrupts();

	for (int i=0;i<queueSize;++i)
	{
		  Thread* temp=readyQueue.dequeue();
            if (temp->ThreadId()==_thread->ThreadId())
                threadFound=true;
            else 
                readyQueue.enqueue(temp);
    }
	if(threadFound)
		--queueSize;		
	
if(!Machine::interrupts_enabled())
	Machine::enable_interrupts();
}



/******************************************************************
RR SCHEDULER METHODS
*******************************************************************/
RRScheduler::RRScheduler()
{
queueSize = 0;
ticks = 0;
hz = 5 ; //Each tick is 10 ms, So 5 ticks -> 50 ms
InterruptHandler::register_handler(0, this); //Registering the Interrupt Handler*/
set_frequency(hz);	

}

void RRScheduler::set_frequency(int _hz) {
/* Set the interrupt frequency for the simple timer.
   Preferably set this before installing the timer handler!                 */

    hz = _hz;                            /* Remember the frequency.           */
    int divisor = 1193180 / _hz;         /* The input clock runs at 1.19MHz   */
    Machine::outportb(0x43, 0x34);                /* Set command byte to be 0x36.      */
    Machine::outportb(0x40, divisor & 0xFF);      /* Set low byte of divisor.          */
    Machine::outportb(0x40, divisor >> 8);        /* Set high byte of divisor.         */
}


void RRScheduler::handle_interrupt(REGS *_r) {
    /* Increment our "ticks" count */
    ticks++;

    /* Whenever 50ms is over, we preempt the current thread and execute the next thread */
    if (ticks >= hz )
    {
        ticks = 0;
        Console::puts("50 ms second has passed\n");
		resume(Thread::CurrentThread());               
		yield();
    }
}

void RRScheduler::yield() {
/*Removing the first thread from the queue. Interrupts have to be disabled whenever we try to do operations in the ready queue,
particularly for RR Scheduling. As once the thread is excuting, a time quanta is finished either while yielding or adding or resuming
to another thread, then an interrupt would be generated. It is necessary to disable the interrupts and continue processing the ready queue*/

Machine::outportb(0x20, 0x20); //informing that interrupt is handled by sending EOI message
if (Machine::interrupts_enabled())	
	Machine::disable_interrupts();
  
  if(queueSize!=0)
  {
	--queueSize;
	ticks = 0;
	Thread *currentThread = readyRRQueue.dequeue(); 	
	if(!Machine::interrupts_enabled())
		Machine::enable_interrupts();
    Thread::dispatch_to(currentThread);
  }
	
}

void RRScheduler::resume(Thread * _thread) {
if (Machine::interrupts_enabled())	
	Machine::disable_interrupts();
		
  /*Adding the thread to the queue*/
  readyRRQueue.enqueue(_thread);
  ++queueSize;
  
  if(!Machine::interrupts_enabled())
	Machine::enable_interrupts();
}

void RRScheduler::add(Thread * _thread) {
	if (Machine::interrupts_enabled())	
	Machine::disable_interrupts();
/*Adding the thread to the queue*/
  readyRRQueue.enqueue(_thread);
  ++queueSize; 
 
  if(!Machine::interrupts_enabled())
	Machine::enable_interrupts();
}

void RRScheduler::terminate(Thread * _thread) {
  /*Dequeue each thread from the Readyqueue, check if its ID matches with the thread to be searched. If it matches set
  variable. and enqueue back the other threads. 
  eg : 1->2->3->4->5
       2->3->4->5->1
	   3->4->5->1->2
	   if 3 is the thread, beyond that 4,5, will be dequeued and enqueued back thus making the ready queue,
	   1->2->4->5
	   
  Actual method : Use next and previous pointers , find the matching thread and map prev->next = current->next.  */

  
  Console::puts("TERMINATE\n");
bool threadFound = false;
if (Machine::interrupts_enabled())	
	Machine::disable_interrupts();

	for (int i=0;i<queueSize;++i)
	{
		  Thread* temp=readyRRQueue.dequeue();
            if (temp->ThreadId()==_thread->ThreadId())
                threadFound=true;
            else 
                readyRRQueue.enqueue(temp);
    }
	if(threadFound)
		--queueSize;		
	
if(!Machine::interrupts_enabled())
	Machine::enable_interrupts();
}


