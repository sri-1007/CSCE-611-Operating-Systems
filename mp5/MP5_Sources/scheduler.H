/* 
    Author: R. Bettati, Joshua Capehart
            Department of Computer Science
            Texas A&M University
			
	    A thread scheduler.

*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */
/*REFERENCE TO UTILS.H . Defined to make NULL available to Class Queue*/
#ifndef NULL
#define NULL 0
#endif
/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "thread.H"
#include "interrupts.H"

/*--------------------------------------------------------------------------*/
/* !!! IMPLEMENTATION HINT !!! */
/*--------------------------------------------------------------------------*/
/*
    One way to proceed is to implement the FIFO scheduling policy inside
    class 'Scheduler'. 

    If you plan to implement a Round-Robin Scheduler, derive it from class
    'Scheduler', say as class 'RRScheduler'. The class 'RRScheduler' is 
    really just a FIFO scheduler with THREE MODIFICATIONS:
    1. It manages a timer, which fires at the end-of-quantum (EOQ). 
    (For details on how to set up a timer and how to handle timer interrupts 
    see the 1-second timer in 'kernel.C'.)  The timer is set up in the
    constructor.
    2. It uses an additional function, the EOQ handler. This function gets
    called whenever an EOQ timer event fires. The EOQ handler forces the 
    current thread to call the scheduler's 'yield' function.
    3. The 'yield' function must be modified to account for unused quantum
    time. If a thread voluntarily yields, the EOQ timer must be reset in order
    to not penalize the next thread.
 
    (Note that this qualifies as programming at about the level of a baboon.
     Much better woudl be to have the abstract class 'Scheduler' implement 
     the basic scheduling MECHANISMS and provide abstract funtions to define
     the queue management POLICIES in derived classes, 
     such as 'FIFOScheduler'.)
    
 */

/*--------------------------------------------------------------------------*/
/* SCHEDULER */
/*--------------------------------------------------------------------------*/

/* Creating a class Queue to ensure easy enqueue, Dequeue operations . The ready queue would be of this class type.*/

class Queue{
    private:
      Thread* threadObj;
      Queue* next;
    public:
      Queue()
	  {
        threadObj = NULL;
        next = NULL;
      }
      Queue(Thread* t)
	  {
        threadObj = t;
        next = NULL;
      }
      void enqueue(Thread* t)
	  {
        if (!threadObj)  
            threadObj = t;  //if threadobject is NULL, assign threadObj as incoming thread :HEAD
        else
            if (!next)
                next = new Queue(t);
            else
                next->enqueue(t);
      }
      Thread* dequeue()
	  {
        if (!threadObj)
            return NULL;
        if (next)
		{
            Thread* removedThread = threadObj;
            threadObj = next->threadObj;
            Queue* consumed = next;
            next = next->next;
            delete consumed;
            return removedThread; 
        }
        
          Thread* temp = threadObj;
          threadObj = NULL;
          return temp;
      }
};

/*Default Scheduler is FIFO Type */
class Scheduler {

  /* The scheduler may need private members... */
  Queue readyQueue; // creates Queue object
  int queueSize;
  
public:

   Scheduler();
   /* Setup the scheduler. This sets up the ready queue, for example.
      If the scheduler implements some sort of round-robin scheme, then the 
      end_of_quantum handler is installed in the constructor as well. */

   /* NOTE: We are making all functions virtual. This may come in handy when
            you want to derive RRScheduler from this class. */
  
   virtual void yield();
   /* Called by the currently running thread in order to give up the CPU. 
      The scheduler selects the next thread from the ready queue to load onto 
      the CPU, and calls the dispatcher function defined in 'Thread.H' to
      do the context switch. */

   virtual void resume(Thread * _thread);
   /* Add the given thread to the ready queue of the scheduler. This is called
      for threads that were waiting for an event to happen, or that have 
      to give up the CPU in response to a preemption. */

   virtual void add(Thread * _thread);
   /* Make the given thread runnable by the scheduler. This function is called
      after thread creation. Depending on implementation, this function may 
      just add the thread to the ready queue, using 'resume'. */

   virtual void terminate(Thread * _thread);
   /* Remove the given thread from the scheduler in preparation for destruction
      of the thread. 
      Graciously handle the case where the thread wants to terminate itself.*/
  
}; 

/*Creating RR scheduler by inheriting from Scheduler class and Interrupt Handler */
class RRScheduler : public Scheduler, public InterruptHandler
{	
	Queue readyRRQueue;
	int queueSize;
    
    int ticks;   /* ticks since last "seconds" update.    */
   /* At what frequency do we update the ticks counter? */
    int hz;                /* Actually, by defaults it is 18.22Hz.
                            In this way, a 16-bit counter wraps
                            around every hour.                    */

    void set_frequency(int _hz);
  /* Set the interrupt frequency for the RR timer. */
  
  public: 
  RRScheduler();
  virtual void yield();
  virtual void resume(Thread * _thread);  
  virtual void add(Thread * _thread);
  virtual void terminate(Thread * _thread);	
  
  virtual void handle_interrupt(REGS *_r);
	
};
#endif
