#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/file.h> // For filp_open and filp_close
#include <linux/usb.h>
#include <linux/notifier.h>
#include <linux/kmod.h>  // For call_usermodehelper
#include <linux/namei.h> // For kern_path
#include <linux/stat.h>  // For umode_t

#define FILE_PATH "/home/haris-majeed-raja/Desktop/OS/Kernel_module/Kernel_module-main/test.txt"
#define PREDEFINED_STRING "Hello from the kernel module!\n"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Haris Majeed Raja");
MODULE_DESCRIPTION("Integrated Kernel Module for File Operations, USB Events, and Notifications");

// Function to write to the file
static void write_to_file(const char *str)
{
    struct file *file;
    ssize_t bytes_written;

    file = filp_open(FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (IS_ERR(file))
    {
        pr_err("Failed to open file %s\n", FILE_PATH);
        return;
    }

    bytes_written = kernel_write(file, str, strlen(str), &file->f_pos);
    if (bytes_written < 0)
        pr_err("Failed to write to file %s\n", FILE_PATH);
    else
        pr_info("Successfully wrote to the file\n");

    filp_close(file, NULL);
}

// Function to clear the file
static void clear_file(void)
{
    struct file *file;

    file = filp_open(FILE_PATH, O_WRONLY | O_TRUNC, 0644);
    if (IS_ERR(file))
    {
        pr_err("Failed to open file %s\n", FILE_PATH);
        return;
    }

    filp_close(file, NULL);
    pr_info("File cleared successfully\n");
}

// Function to set file permissions
static void set_file_permissions(const char *path, umode_t mode)
{
    struct path file_path;
    struct inode *inode;
    int ret;

    ret = kern_path(path, LOOKUP_FOLLOW, &file_path);
    if (ret)
    {
        pr_err("Failed to resolve path: %s, error: %d\n", path, ret);
        return;
    }

    inode = file_path.dentry->d_inode;

    inode_lock(inode); // Lock the inode for modifications
    inode->i_mode = (inode->i_mode & ~0777) | mode;
    inode_unlock(inode);

    pr_info("Updated permissions for %s to %o\n", path, mode);
}

// USB notification handler
static int usb_notify(struct notifier_block *nb, unsigned long action, void *data)
{
    struct usb_device *usb_dev = data;

    switch (action)
    {
    case USB_DEVICE_ADD:
        printk(KERN_INFO "USB device connected: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);

        // Write predefined string to file
        write_to_file(PREDEFINED_STRING);

        // Set file to read-write mode
        set_file_permissions(FILE_PATH, 0644);

        // Call user-space helper for connection
        printk(KERN_INFO "Calling user-space helper for connection\n");
        char *argv_add[] = {"/usr/bin/play_usb", NULL};
        int ret_add = call_usermodehelper(argv_add[0], argv_add, NULL, UMH_WAIT_PROC);
        printk(KERN_INFO "call_usermodehelper returned for connection: %d\n", ret_add);

        break;

    case USB_DEVICE_REMOVE:
        printk(KERN_INFO "USB device removed: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);

        // Clear the file
        clear_file();

        // Set file to read-only mode
        set_file_permissions(FILE_PATH, 0444);

        // Call user-space helper for disconnection
        printk(KERN_INFO "Calling user-space helper for disconnection\n");
        char *argv_remove[] = {"/usr/bin/play_usb", NULL};
        int ret_remove = call_usermodehelper(argv_remove[0], argv_remove, NULL, UMH_WAIT_PROC);
        printk(KERN_INFO "call_usermodehelper returned for disconnection: %d\n", ret_remove);

        break;
    }

    return NOTIFY_OK;
}

// USB notifier block
static struct notifier_block usb_nb = {
    .notifier_call = usb_notify,
};

// Module init function
static int __init my_module_init(void)
{
    pr_info("Integrated Kernel Module Loaded\n");

    // Write predefined string to file during initialization
    write_to_file(PREDEFINED_STRING);

    // Register USB notifier
    usb_register_notify(&usb_nb);

    return 0;
}

// Module exit function
static void __exit my_module_exit(void)
{
    // Unregister USB notifier
    usb_unregister_notify(&usb_nb);

    pr_info("Integrated Kernel Module Unloaded\n");
}

// Register init and exit functions
module_init(my_module_init);
module_exit(my_module_exit);
