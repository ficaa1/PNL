#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>

MODULE_DESCRIPTION("Module pour modifier le uname");
MODULE_AUTHOR("Filip Biljic");
MODULE_LICENSE("GPL");

static char tmp[9];

static int __init hello_init(void)
{
	pr_info("Uname\n");
	strcpy(tmp, init_uts_ns.name.release);
	strcpy(init_uts_ns.name.release, "test");
	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	pr_info("Goodbye, Uname\n");
	strcpy(init_uts_ns.name.release, tmp);
}
module_exit(hello_exit);
