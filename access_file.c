#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/file.h>
#include <linux/usb.h>
#include <linux/notifier.h>
#include <linux/stat.h>
#include <linux/dcache.h> // For dentry
#include <linux/namei.h>  // For kern_path
#include <linux/kmod.h>   // For call_usermodehelper

#define FILE_PATH "/home/haris-majeed-raja/Desktop/OS/Kernel_module/Kernel_module-main/test.txt"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Haris Majeed Raja");
MODULE_DESCRIPTION("USB Connection Logger with File Permissions Management");

// Function to update file permissions
static void set_file_permissions(const char *path, umode_t mode)
{
    struct path file_path;
    struct inode *inode;
    int ret;

    // Resolve the file path
    ret = kern_path(path, LOOKUP_FOLLOW, &file_path);
    if (ret)
    {
        pr_err("Failed to resolve path: %s, error: %d\n", path, ret);
        return;
    }

    // Access inode
    inode = file_path.dentry->d_inode;

    // Modify inode permissions
    inode_lock(inode); // Lock the inode for safe modification
    inode->i_mode = (inode->i_mode & ~0777) | mode;
    inode_unlock(inode); // Unlock the inode

    pr_info("Updated permissions for %s to %o\n", path, mode);
}

// USB event handler
static int usb_notify(struct notifier_block *nb, unsigned long action, void *data)
{
    struct usb_device *usb_dev = data;

    switch (action)
    {
    case USB_DEVICE_ADD:
        printk(KERN_INFO "USB device connected: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);

        // Set file to read-write mode (0644)
        set_file_permissions(FILE_PATH, 0644);
        printk(KERN_INFO "File permissions set to read-write (0644)\n");
        break;

    case USB_DEVICE_REMOVE:
        printk(KERN_INFO "USB device removed: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);

        // Set file to read-only mode (0444)
        set_file_permissions(FILE_PATH, 0444);
        printk(KERN_INFO "File permissions set to read-only (0444)\n");
        break;

    default:
        pr_info("Unknown USB event occurred\n");
    }

    return NOTIFY_OK;
}

// USB notifier block
static struct notifier_block usb_nb = {
    .notifier_call = usb_notify,
};

// Module init function
static int __init usb_logger_init(void)
{
    printk(KERN_INFO "USB Logger Module Loaded\n");

    // Register USB notifier
    usb_register_notify(&usb_nb);
    return 0;
}

// Module exit function
static void __exit usb_logger_exit(void)
{
    usb_unregister_notify(&usb_nb);
    printk(KERN_INFO "USB Logger Module Unloaded\n");
}

module_init(usb_logger_init);
module_exit(usb_logger_exit);
