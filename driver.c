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

static char path_name[MAX_PATH_LEN];

static ssize_t memblock_dentry_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    struct dentry *dentry;
    struct path path;
    struct memblock_type *type;
    pgoff_t index = 0;
    struct page *page;
    int ret;


    if (kern_path(path_name, LOOKUP_FOLLOW, &path)) {
        printk(KERN_ERR "Failed to get path %s\n", path);
        return -EFAULT;
    }

    dentry = path.dentry;

    //type = memblock_get_region_type(page_to_phys(dentry->d_inode->i_mapping->pages->first), PAGE_SIZE);
    while ((page = find_get_pages(dentry->d_inode->i_mapping, index, 1, NULL)) != NULL) {
        type = memblock_get_region_type(page_to_phys(page), PAGE_SIZE);
        if (type) 
            snprintf(buf + strlen(buf), count - strlen(buf), "page: %d memblock_type: %s\n", index, type->name);
        index++;
    }

    /*if (!type) {
        printk(KERN_ERR "Failed to get memblock_type for %s\n", path);
        return -EFAULT;
    }

    ret = snprintf(buf, count, "memblock_type: %s\n", type->name);
    if (ret >= count)
        return -ENOSPC;*/

    ret = snprintf(buf + strlen(buf), count - strlen(buf), "dentry: %p\n", dentry);
    if (ret >= count - strlen(buf))
        return -ENOSPC;

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
    
    // Обработка запроса и запись результата в переменную result
    
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