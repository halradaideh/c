#include <linux/module.h>	// for init_module()
#include <asm/uaccess.h>	// for copy_to/from_user()
#include <asm/unistd.h>		// for __NR_break
#include <asm/io.h>		    // for phys_to_virt()

extern unsigned long *sys_call_table;

char modname[] = "newcall";
unsigned long save_old_syscall_entry;
unsigned long save_old_pgtable_entry;
unsigned int _cr4, _cr3;
unsigned int *pgdir, dindex;
unsigned int *pgtbl, pindex;


asmlinkage long my_syscall( int __user * num )
{
	int	val;

	if ( copy_from_user( &val, num, sizeof( int ) ) ) return -EFAULT;
	++val;
	if ( copy_to_user( num, &val, sizeof( int ) ) ) return -EFAULT; 

	return	0;  // SUCCESS
}	


static void __exit my_exit( void )
{
	printk( "<1>Removing \'%s\' module\n", modname );
	sys_call_table[ __NR_break ] = save_old_syscall_entry;
	pgtbl[ pindex ] = save_old_pgtable_entry;
}



static int __init my_init( void )
{
	printk( "<1>\nInstalling \'%s\' module ", modname );
	printk( "(sys_call_table[] at %08X) \n", (int)sys_call_table );

	// get current values from control-registers CR3 and CR4
	asm(" mov %%cr4, %%eax \n mov %%eax, _cr4 " ::: "ax" );
	asm(" mov %%cr3, %%eax \n mov %%eax, _cr3 " ::: "ax" );

	// confirm that processor is using the legacy paging mechanism
	if ( (_cr4 >> 5) & 1 ) {
        printk( " processor is using Page-Address Extensions \n");
        return 	-ENOSYS;
    }

	// extract paging-table indices from 'sys_call_table[]' address 
	dindex = ((int)sys_call_table >> 22) & 0x3FF;	// pgdir-index
	pindex = ((int)sys_call_table >> 12) & 0x3FF;	// pgtbl-index

	// setup pointers to the page-directory and page-table frames
	pgdir = phys_to_virt( _cr3 & ~0xFFF );
	pgtbl = phys_to_virt( pgdir[ dindex ] & ~0xFFF );	

	// preserve page-table entry for the 'sys_call_table[]' frame
	save_old_pgtable_entry = pgtbl[ pindex ];


	printk( "\nInstalling new function for system-call %d\n", __NR_break );

	// make sure that page-frame is 'writable'
	pgtbl[ pindex ] |= 2;

	// create a backup for old entry
	save_old_syscall_entry = sys_call_table[ __NR_break ];

	// rewrite entry with new function
	sys_call_table[ __NR_break ] = (unsigned long)my_syscall;

	return	0;  // SUCCESS
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
