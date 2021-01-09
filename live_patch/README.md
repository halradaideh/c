This to demonstrate how to patch a live system using LKM ( Linux Kernel Modules ), we are going to modify the sys_call_table[] to replace a existing call with our system call.

requirements:
* Liunx OS with Kernel before 2.6.22.5 ( I am going to use Ubuntu 4.10 Warty Warthog )
* Linux headers installed
* gcc , g++
* i386 based CPU
* IDE based storage ( sata is not supported at that time )
* Page-Address Extensions Disabled ( to use legacy paging mode, can be done using Virtual Box )

source : https://www.cs.usfca.edu/~cruse/cs635/