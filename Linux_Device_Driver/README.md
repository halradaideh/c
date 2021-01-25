ref : 
* https://tldp.org/LDP/lkmpg/2.6/html/x569.html
* https://olegkutkov.me/2018/03/14/simple-linux-character-device-driver/

```bash
gcc -c CODE.c -D__KERNEL__ -DMODULE -o OUTPUT
insmod OUTOUT.ko
mknod /dev/DEVICE_NAME [c|b|..] MAJOR MINOR
rmmod OUTPUT
```


