#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/ktime.h>

MODULE_DESCRIPTION("Module d'affichage specifique des superblocks"");
MODULE_AUTHOR("Filip BILJIC");
MODULE_LICENSE("GPL");

static char *fs_type_toShow = "";
module_param(fs_type_toShow, charp, 0);
MODULE_PARM_DESC(fs_type_toShow, "Quel type de fs a afficher");
static void iter_handler(struct super_block *sb, void *ptr)
{

	pr_info("uuid=%d type=%s time=%d \n", sb->s_dev,
		sb->s_type->name, ktime_to_ms(ktime_get()));
}	

static int __init updatesb_init(void)
{
	pr_info("[update_sb - init] Loading \n");
	struct file_system_type *fs_type_ptr;
	fs_type_ptr = get_fs_type(fs_type_toShow);
	if (fs_type_ptr == NULL) {
		pr_warn("Filesystem introuvable\n");
		return 0;
	}

	iterate_supers_type(fs_type_ptr, iter_handler, NULL);
	put_filesystem(fs_type_ptr);
	return 0;
}
module_init(updatesb_init);

static void __exit updatesb_exit(void)
{
	pr_info("Goodnight, show_sb\n");
}
module_exit(updatesb_exit);
