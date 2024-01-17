#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/memblock.h>
#include <linux/dcache.h>

#define MAX_PATH_LEN 256
#define BUFFER_SIZE 4096

static char path_name[MAX_PATH_LEN];
static int mod = 0, num = 0;

static ssize_t memblock_dentry_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    struct dentry *dentry;
    struct path path;
    char buffer[BUFFER_SIZE];
    struct memblock_type *type;


    if (mod) { 
        if (kern_path(path_name, LOOKUP_FOLLOW, &path)) {
            printk(KERN_ERR "Failed to get path %s\n", path);
            return -EFAULT;
        }

        dentry = path.dentry;

        sprintf(buffer + strlen(buffer), "короткое имя файла: %s\n", dentry->d_iname);
        sprintf(buffer + strlen(buffer), "номер inode: %s\n", dentry->d_inode->i_ino);
        sprintf(buffer + strlen(buffer), "является ли объект точкой монтирования: %s\n", d_mountpoint(dentry));
        sprintf(buffer + strlen(buffer), "счетчик использования: %s\n", dentry->d_count);
    } else {
        type = &memblock.memory;

        sprintf(buffer + strlen(buffer), "количество областей памяти внутри текущего блока памяти: %s\n", type->cnt);
        sprintf(buffer + strlen(buffer), "размер всех областей памяти: %s\n", type->max);
        sprintf(buffer + strlen(buffer), "размер выделенного массива областей памяти: %s\n", type->total_size);

        for (int i = 0; i < min(num, type->cnt), i++) {
            sprintf(buffer + strlen(buffer), "адрес региона %d: %s\n", type->regions[i]->base);
        }
    }

    if (copy_to_user(buf, buffer, strlen(buffer))){
        return -EFAULT;
    }
 
    return strlen(buf);
}

static ssize_t memblock_dentry_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    pr_info("WRITING...");
    char request[BUFFER_SIZE];
    
    if (count >= BUFFER_SIZE) {
        return -EINVAL;
    }
    
    if (copy_from_user(request, buffer, count)) {
        return -EFAULT;
    }
    
    sscanf(request, "%d", &mod);

    if (mod) {
        sscanf(request, "%d %s", &mod, path_name);
    } else {
        sscanf(request, "%d %d", &mod, &num);
    }
    
    return count;
}

static const struct file_operations memblock_dentry_fops = {
    .read = memblock_dentry_read,
    .write = memblock_dentry_write,
};

static int __init memblock_dentry_info_init(void)
{
    struct dentry *dir;

    dir = debugfs_create_dir("lab", NULL);
    if (!dir) {
        printk(KERN_ERR "Failed to create debugfs directory\n");
        return -ENOMEM;
    }

    debugfs_create_file("memblock_dentry_info", 0444, dir, NULL, &memblock_dentry_fops);

    return 0;
}

static void __exit memblock_dentry_info_exit(void)
{
    debugfs_remove_recursive(NULL);
}

module_init(memblock_dentry_info_init);
module_exit(memblock_dentry_info_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Egor Nikitin");
MODULE_DESCRIPTION("Module for retrieving memblock_type and dentry information");