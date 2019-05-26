#include "assert.H"
#include "exceptions.H"
#include "console.H"
#include "paging_low.H"
#include "page_table.H"

PageTable * PageTable::current_page_table = NULL;
unsigned int PageTable::paging_enabled = 0;
ContFramePool * PageTable::kernel_mem_pool = NULL;
ContFramePool * PageTable::process_mem_pool = NULL;
unsigned long PageTable::shared_size = 0;


void PageTable::init_paging(ContFramePool * _kernel_mem_pool,
                            ContFramePool * _process_mem_pool,
                            const unsigned long _shared_size)
{
   Console::puts("Initialized Paging System\n");
   kernel_mem_pool = _kernel_mem_pool;
   process_mem_pool = _process_mem_pool;
   shared_size = _shared_size;
   
}

PageTable::PageTable()
{
   Console::puts("Constructed Page Table object\n");
   page_directory =  (unsigned long *)(kernel_mem_pool->get_frames(1) * PAGE_SIZE);
  
   /*Setting up Page TABLE*/
   unsigned long *page_table = (unsigned long *) (kernel_mem_pool->get_frames(1) * PAGE_SIZE);
   unsigned long address = 0;
   for(unsigned int i =0; i<1024; i++)
   {
	   page_table[i] = address |3;  //attribute set to: supervisor level, read/write, present(011 in binary)
	   address += PAGE_SIZE;        // 4KB
   }
   
   /*Setting up Page Directory Entries */
   page_directory[0] = (unsigned long)page_table;  
   page_directory[0] = page_directory[0] |3 ;// setting it to be supervisor, RW, PRESENT (011)
   
   for(unsigned int i = 1; i<1024; i++)
   {
	 page_directory[i]= 0|2;  
   }   
   paging_enabled = 0;
}

void PageTable::load()
{
   Console::puts("Loaded page table\n");
   current_page_table = this;
   Console::putui((unsigned long)(current_page_table->page_directory[1]));
   write_cr3((unsigned long)(current_page_table->page_directory)); // PTBR in x86
}

void PageTable::enable_paging()
{
   Console::puts("Enabled paging\n");
   write_cr0(read_cr0() | 0x80000000);
   paging_enabled = 1;
}

void PageTable::handle_fault(REGS * _r)
{
  unsigned long address = read_cr2();    // Returns the faulty address
  unsigned long* ptr_page_dir = current_page_table->page_directory;
  
  /* Extracting the first 10 bits and next 10 bits to get the page directory and page table index of the faulty address*/
  unsigned long obtained_page_dir_index = address >>22;
  unsigned long obtained_page_table_index = (address & (0x03FF << 12) ) >>12 ;
	
  unsigned long *page_table; 
  unsigned long *page_table_entry; 

/*To check if the Page directory has the obtained_page_dir_index. Check if the present bit is set or not. If not
set then the address is not present and Create an entry in page directory  */

	if((ptr_page_dir[obtained_page_dir_index] & 1) == 0) 
	{
		page_table =  (unsigned long *) (kernel_mem_pool->get_frames(1) * PAGE_SIZE); 
		ptr_page_dir[obtained_page_dir_index] = (unsigned long)page_table;
		ptr_page_dir[obtained_page_dir_index] = ptr_page_dir[obtained_page_dir_index] |3;
			
		for(unsigned long int i =0; i<ENTRIES_PER_PAGE; i++)
		{	
		page_table[i] = 2;		 // Making all the entries as not present. 010. 
		}				
	}
	   
/* Page table address would be the 	ptr_page_dir[obtained_page_dir_index] before setting the last bit. hence the bit
manipulation. Handling second level of page fault. Checking if the entry is present in the page table by checking the 
last present bit.If not, creatibg an entry */

	page_table = (unsigned long *)(ptr_page_dir[obtained_page_dir_index] & ~0xFFF);
	page_table_entry = (unsigned long*)page_table[obtained_page_table_index];	
	
	if((page_table[obtained_page_table_index] &1 ) == 0)
	{		
		page_table_entry = (unsigned long *) (process_mem_pool->get_frames(1) * PAGE_SIZE);
		page_table[obtained_page_table_index] = (unsigned long)page_table_entry;
		page_table[obtained_page_table_index] |= 3; 
			
    }
}

