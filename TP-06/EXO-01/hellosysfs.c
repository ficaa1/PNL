#include <linux/module.h>
#include <linux/kernel.h>

MODULE_DESCRIPTION("Module pour la creation d'un kobjet");
MODULE_AUTHOR("Filip BILJIC");
MODULE_LICENSE("GPL");

static char kerText[10];

static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){

	return sprintf(buf, "Hello %s \n", kerText);

}

static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,size_t count){
	return strlcpy(kerText, buf, min(10, count + 1));
}

static struct kobj_attribute hello_kobj = __ATTR_RW(hello);

static int hellosysfs_init(void)
{
	strncpy(kerText, "sysfs", 6);
	pr_info("hellosysfs module loaded\n");
	return sysfs_create_file(kernel_kobj, &hello_kobj.attr);
}

static void hellosysfs_exit(void)
{
	sysfs_remove_file(kernel_kobj, &hello_kobj.attr);
	pr_info("hellosysfs module unloaded\n");
}

module_init(hellosysfs_init);
module_exit(hellosysfs_exit);
