#include <linux/module.h>
#include <linux/kernel.h>

char modname[] = "sys_export";
unsigned long myval;

char *svctable = "TO-BE-Filled";
module_param(svctable, charp, 0444 );

unsigned long *sys_call_table;
EXPORT_SYMBOL(sys_call_table);

static int __init my_init( void )
{
	printk( "<1>\nInstalling \'%s\' module ", modname );
	printk( "with svctable=%s \n", svctable );

	myval = simple_strtoul( svctable, 0, 16 );
	if ( myval == 0 ) return -EINVAL;
	sys_call_table = (unsigned long*)myval;

	printk( "<1>  sys_call_table[] at %p \n", sys_call_table );
	return	0;  // SUCCESS
}

static void __exit my_exit( void )
{
	printk( "<1>Removing \'%s\' module\n", modname );
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");