
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mount.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/memblock.h>
#include <linux/dcache.h>
#include <linux/mutex.h>

#define MAX_PATH_LEN 256
#define BUFFER_SIZE 1000

static DEFINE_MUTEX(mutex);
//extern struct memblock memblock;
static struct dentry *root;
static char path_name[MAX_PATH_LEN];
static int mod = 1, num = 0;

static ssize_t memblock_dentry_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    char buffer[BUFFER_SIZE];
    int len = 0;
    //struct dentry *dentry;
    //struct path path;
    /*struct memblock_type *type;
    int i;*/

    printk(KERN_INFO "kek 21\n");
    if (mod) { 
        /*if (kern_path(path_name, LOOKUP_FOLLOW, &path)) {
            //printk(KERN_ERR "Failed to get path %s\n", path_name);
            return -EFAULT;
        }

        dentry = path.dentry;*/
        len = sprintf(buffer, "короткое имя файла:\n");
        printk(KERN_INFO "kek %lu", strlen(buffer));
        printk(KERN_INFO "kek %s", path_name);
        //sprintf(buffer + strlen(buffer), "короткое имя файла: %s\n", dentry->d_iname);
        //sprintf(buffer + strlen(buffer), "номер inode: %lu\n", dentry->d_inode->i_ino);
        //sprintf(buffer + strlen(buffer), "является ли объект точкой монтирования: %d\n", d_mountpoint(dentry));
        //sprintf(buffer + strlen(buffer), "счетчик использования: %s\n", dentry->d_count);
    } else {
        /*type = &memblock.memory;

        sprintf(buffer + strlen(buffer), "количество областей памяти внутри текущего блока памяти: %ld\n", type->cnt);
        sprintf(buffer + strlen(buffer), "размер всех областей памяти: %ld\n", type->max);
        sprintf(buffer + strlen(buffer), "размер выделенного массива областей памяти: %lld\n", type->total_size);

        for (i = 0; i < num; i++) {
            sprintf(buffer + strlen(buffer), "адрес региона %d: %lld\n", i, type->regions[i].base);
        }*/
    }

    if (copy_to_user(buf, buffer, strlen(buffer))){
        printk(KERN_INFO "kek lol");
        return -EFAULT;
    }
    printk(KERN_INFO "kek lol 2");
    return strlen(buffer);
}

static ssize_t memblock_dentry_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    char request[BUFFER_SIZE];
    pr_info("WRITING...");

    printk(KERN_INFO "kek\n");
    if (count >= BUFFER_SIZE) {
        return -EINVAL;
    }
    
    if (copy_from_user(request, buffer, count)) {
        return -EFAULT;
    }
    
    //sscanf(request, "%d", &mod);

    if (mod) {
        //sscanf(request, "%d %s", &mod, path_name);
    } else {
        sscanf(request, "%d %d", &mod, &num);
    }
    printk(KERN_INFO "kek\n");
    return count;
}

static int file_open(struct inode *inode, struct file *file) {
    mutex_lock(&mutex);
    printk(KERN_INFO "lock\n");

    return 0;
}

static int file_release(struct inode *inode, struct file *file) {
    mutex_unlock(&mutex);
    printk(KERN_INFO "unlock\n");

    return 0;
}

static const struct file_operations memblock_dentry_fops = {
    .read = memblock_dentry_read,
    .write = memblock_dentry_write,
    .open = file_open,
    .release = file_release,
};

static int __init memblock_dentry_info_init(void)
{
    printk(KERN_INFO "kek0\n");
    root = debugfs_create_dir("lab20", NULL);
    if (!root) {
        printk(KERN_ERR "Failed to create debugfs directory\n");
        return -ENOMEM;
    }

    debugfs_create_file("memblock_dentry_info", 0777, root, NULL, &memblock_dentry_fops);

    return 0;
}

static void __exit memblock_dentry_info_exit(void)
{
    debugfs_remove_recursive(root);
}

module_init(memblock_dentry_info_init);
module_exit(memblock_dentry_info_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Egor Nikitin");
MODULE_DESCRIPTION("Module for retrieving memblock_type and dentry information");