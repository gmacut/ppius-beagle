/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of memory.c functions */
int mem_open(struct inode *inode, struct file *filp);
int mem_release(struct inode *inode, struct file *filp);
ssize_t mem_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t mem_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
void mem_exit(void);
int mem_init(void);

/* Structure that declares the usual file */
/* access functions */
struct file_operations mem_fops = {
  read: mem_read,
  write: mem_write,
  open: mem_open,
  release: mem_release
};


/* Declaration of the init and exit functions */
module_init(mem_init);
module_exit(mem_exit);

/* Global variables of the driver */
/* Major number */
int mem_major = 60;
/* Buffer to store data */
char *mem_buffer;




int mem_init(void) {
  int result;

  /* Registering device */
  result = register_chrdev(mem_major, "mymem", &mem_fops);
  if (result < 0) {
    printk(
      "<1>mem: cannot obtain major number %d\n", mem_major);
    return result;
  }

  /* Allocating memory for the buffer */
  mem_buffer = kmalloc(2, GFP_KERNEL); 
  if (!mem_buffer) { 
    result = -ENOMEM;
    goto fail; 
  } 
  memset(mem_buffer, 0, 2);

  printk("<1>Inserting mem module\n"); 
  return 0;

  fail: 
    mem_exit(); 
    return result;
}


void mem_exit(void) {
  /* Freeing the major number */
  unregister_chrdev(mem_major, "mymem");

  /* Freeing buffer memory */
  if (mem_buffer) {
    kfree(mem_buffer);
  }

  printk("<1>Removing mem module\n");

}



int mem_open(struct inode *inode, struct file *filp) {

  /* Success */
  return 0;
}

int mem_release(struct inode *inode, struct file *filp) {
 
  /* Success */
  return 0;
}


ssize_t mem_read(struct file *filp, char *buf, 
                    size_t count, loff_t *f_pos) { 
 
  /* Transfering data to user space */ 
  copy_to_user(buf,mem_buffer,2);


  /* Changing reading position as best suits */ 
  if (*f_pos == 0) { 
    *f_pos+=2; 
    return 2; 
  } else { 
    return 0; 
  }
}


ssize_t mem_write( struct file *filp, char *buf,
                      size_t count, loff_t *f_pos) {

  char *tmp;

  tmp=buf+count-1;
  copy_from_user(mem_buffer,tmp,1);
  copy_from_user(mem_buffer+1, tmp, 1);
  return 1;
}