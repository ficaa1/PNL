#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_DESCRIPTION("Module \"hello world\"");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");

static char *whom = "Filip";
module_param(whom, charp, 0660);
static int howmany = 1;
module_param(howmany,int,0660);

static int __init hello_init(void)
{
	while(howmany>0){
		pr_info("Hello, %s \n", whom);
		howmany--;
	}
	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	pr_info("Goodbye, %s\n", whom);
}
module_exit(hello_exit);

