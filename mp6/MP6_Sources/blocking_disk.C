/*
     File        : blocking_disk.c

     Author      : 
     Modified    : 

     Description : 

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
#include "blocking_disk.H"

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR */
/*--------------------------------------------------------------------------*/
BlockingDisk::BlockingDisk(DISK_ID _disk_id, unsigned int _size, Scheduler *_scheduler) 
  : SimpleDisk(_disk_id, _size) {
	  this->scheduler = _scheduler; //passing the SYSTEM scheduler object and assigning to the class pointer to access scheduler class
	  this->scheduler->addDiskobj(this); // Exposing the SYSTEMDISK object to the scheduler class
	  queue_size = 0;
	  this->block_queue = new Queue();
}

void BlockingDisk::wait_until_ready()
{
/* This checks if the disk is ready and if not, adds the thread to the Blocking queue and yields the nesxt thread*/	
	if(!is_ready())
	{
			Thread *t = Thread::CurrentThread();
			block_queue->enqueue(t);//add thread to queue
			++queue_size;
			
			this->scheduler->yield();		
	}
}

void BlockingDisk::pop_from_queue()
{
/* This pops the thread from the Blocking disk . If Disk Operation is not issued the 
thread is added back to the front of the ready queue to maintain FIFO order , 
otherwise if disk operation is issued and disk is ready , it is added to the back of the
queue. This is done, to ensure a THREAD SAFE SYSTEM*/

  if(queue_size!=0)
 {
	Thread* t = this->block_queue->dequeue();
	if(scheduler->IssueDiskOperationFlag == false)
		this->scheduler->enqueueFront(t);
	else
	   this->scheduler->resume(t);
	--queue_size;
 }	
		
}

bool BlockingDisk::is_ready()
{
	/* This returns if the disk is ready or not.*/
return SimpleDisk::is_ready();		
}	

/*--------------------------------------------------------------------------*/
/* SIMPLE_DISK FUNCTIONS */
/*--------------------------------------------------------------------------*/

void BlockingDisk::read(unsigned long _block_no, unsigned char * _buf) {
/* It is first added to the blocking queue, where it yields the thread. It is necessary to maintain a flag for thread 
safe operation and also necessary to  add to the blocking queue before the issue operation is performed as it is 
noticed that once issue operation is performed the disk returns true always for isready() function.
If issue operation is not performed , the thread resumes back, it is removed from blocking queue 
 and calls issue operation. Once issued, the flag is set to true and is added back to the blocking queue.
 This is why wait_until_ready is called twice*/
 
 wait_until_ready();
 SimpleDisk::issue_operation(READ, _block_no);
 scheduler->IssueDiskOperationFlag = true;
 wait_until_ready();
 int i;
  unsigned short tmpw;
  for (i = 0; i < 256; i++) {
    tmpw = Machine::inportw(0x1F0);
    _buf[i*2]   = (unsigned char)tmpw;
    _buf[i*2+1] = (unsigned char)(tmpw >> 8);
  }
  
  scheduler->IssueDiskOperationFlag = false;
 

}


void BlockingDisk::write(unsigned long _block_no, unsigned char * _buf) { 
 /* same order as READ*/
 wait_until_ready();
 SimpleDisk::issue_operation(WRITE, _block_no);
  scheduler->IssueDiskOperationFlag = true;
 wait_until_ready();
 
 int i; 
  unsigned short tmpw;
  for (i = 0; i < 256; i++) {
    tmpw = _buf[2*i] | (_buf[2*i+1] << 8);
    Machine::outportw(0x1F0, tmpw);
  }

 scheduler->IssueDiskOperationFlag = false;
}
