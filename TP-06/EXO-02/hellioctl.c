#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "hellioctl.h"


MODULE_DESCRIPTION("Module pour tester les ioctl");
MODULE_AUTHOR("Filip BILJIC");
MODULE_LICENSE("GPL");

static int major;
static char msg[MSG_SIZE];
static char adaptmsg[ADAPT_SIZE] = "ioctl";

static long hellioctl(struct file *file, unsigned int cmd, unsigned long arg){
	switch(cmd){
	case HELLO:
		pr_info("HELLO command called");
		sprintf(msg, "Hello %s", adaptmsg);
		return copy_to_user((char*) arg, msg, strlen(msg));
	case WHO:
		pr_info("WHO command called");
		return copy_from_user(adaptmsg, (char*) arg, strlen(adaptmsg));
		
	default:
		return -ENOTTY;
	}
}


static struct file_operations hello_fops = {
	.unlocked_ioctl=hellioctl
};

static int __init helloioctl_init(void)
{
	major = register_chrdev(0, DEV_NAME , &hello_fops);
	pr_info("helloioctl module loaded with major number : %d\n", major);
	pr_info("IO_MAGIC : %d\n", IO_MAGIC);

	return 0;
}

static void __exit helloioctl_exit(void)
{
	unregister_chrdev(major, DEV_NAME);
	pr_info("helloioctl module unloaded\n");
}

module_init(helloioctl_init);
module_exit(helloioctl_exit);
