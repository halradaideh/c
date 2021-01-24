```bash
gcc -c CODE.c -D__KERNEL__ -DMODULE -o OUTPUT
insmod OUTOUT.ko
mknod /dev/DEVICE_NAME [c|b|..] MAJOR MINOR
rmmod OUTPUT
```


