#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("Module d'affichage des superblocks");
MODULE_AUTHOR("Filip BILJIC");
MODULE_LICENSE("GPL");

void iter_handler(struct super_block *sb, void *arg)
{
    pr_info("uuid=%d type = %s\n", sb->s_dev, sb->s_type->name);
}

static int __init showsb_init(void)
{
	pr_info("Goodmorning, show_sb\n");
    iterate_supers(iter_handler, NULL);
	return 0;
}
module_init(showsb_init);

static void __exit showsb_exit(void)
{
	pr_info("Goodnight, show_sb\n");
}
module_exit(showsb_exit);
