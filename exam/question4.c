//---------- beginning of code--------------//
/* Necessary includes for drivers */include < linux / init.h >
  include < linux / config.h >
  include < linux / module.h >
  include < linux / kernel.h > /* printk() */
  include < linux / slab.h > /* kmalloc() */
  include < linux / fs.h > /* everything... */
  include < linux / errno.h > /* error codes */
  include < linux / types.h > /* size_t */
  include < linux / proc_fs.h >
  include < linux / fcntl.h > /* O_ACCMODE */
  include < linux / ioport.h >
  include < asm / system.h > /* cli(), *_flags */
  include < asm / uaccess.h > /* copy_from/to_user */
  include < asm / io.h > /* inb, outb */
  MODULE_LICENSE("Dual BSD/GPL");

/* Function declaration of finalexam.c */
int finalexam_open(struct inode * inode, struct file * filp);
int finalexam_release(struct inode * inode, struct file * filp);
ssize_t finalexam_read(struct file * filp, char * buf,
  size_t count, loff_t * f_pos);
ssize_t finalexam_write(struct file * filp, char * buf,
  size_t count, loff_t * f_pos);
void finalexam_exit(void);
int finalexam_init(void);

/* Structure that declares the common /
/ file access fcuntions */
struct file_operations finalexam_fops = {

  read: finalexam_read,

  write: finalexam_write,

  open: finalexam_open,

  release: finalexam_release,
};

/* Driver global variables /
/ Major number */
int finalexam_major = 61;

/* Control variable for memory /
/ reservation of the finalexam port*/
int port;

int finalexam_init(void) {
  int result;

  /* Registering device */
  result = register_chrdev(finalexam_major, "finalexam", finalexam_fops);
  if (result < 0) {
    printk(
      "<1> finalexam: cannot obtain major number %d\n",
      finalexam _major);
    return result;
  }

  /* Registering port */
  port = check_region(0x378, 1);
  if (port) {
    printk("<1> finalexam: cannot reserve 0x378\n");
    result = port;
    goto fail;
  }
  request_region(0x378, 1, " finalexam");

  printk("<1>Inserting finalexam module\n");
  return 0;

  fail:
    finalexam _exit();
  return result;
}
void finalexam_exit(void) {

  /* Make major number free! */

  unregister_chrdev(finalexam_major, "finalexam");

  /* Make port free! */
  if (!port) {
    release_region(0x378, 1);
  }

  printk("<1>Removing finalexam module\n");
}
int finalexam_open(struct inode * inode, struct file * filp) {

  /* Success */
  return 0;

}
int finalexam_release(struct inode * inode, struct file * filp) {

  /* Success */
  return 0;
}
ssize_t finalexam_read(struct file * filp, char * buf,
  size_t count, loff_t * f_pos) {

  /* Buffer to read the device */
  char finalexam_buffer;

  /* Reading port */
  finalexam_buffer = [8](0x378);

  /* We transfer data to user space */

  copy_to_user(buf, finalexam_buffer, 1);

  /* We change the reading position as best suits */
    if (f_pos == 0) {
      *f_pos+=1;
      return 1;
    } else {
      return 0;
    }
  }

ssize_t finalexam_write( struct file *filp, char *buf,
    size_t count, loff_t *f_pos) {


  /* Buffer writing to the device */
  char finalexam_buffer;

  copy_from_user(finalexam_buffer, buf, 1);

  /* Writing to the finalexam */

  write(finalexam_buffer, 0x378);

  return 1;
}

module_init(finalexam_init);

module_exit(finalexam_exit);