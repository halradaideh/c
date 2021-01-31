#define MODULE
#include <linux/module.h>
#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO

int sum (int x, int y) {
    return (x-y);
}

static int x=0;
static int y=0;

MODULE_PARAM(x, int, "i");
MODULE_PARAM(y, int, "i");

int init_modules() {
    printk("\n result=%d\n",sum(x,y));
    return 0;
}

void cleanup_module() {
    printk("\nModule removed....");
}