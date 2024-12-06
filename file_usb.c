#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/file.h> // For filp_open and filp_close
#include <linux/usb.h>
#include <linux/notifier.h>
#include <linux/kmod.h> // For call_usermodehelper

#define FILE_PATH "/home/haris-majeed-raja/Desktop/OS/Kernel_module/Kernel_module-main/test.txt"
#define PREDEFINED_STRING "Hello from the kernel module!\n"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Haris Majeed Raja");
MODULE_DESCRIPTION("USB Connection Logger Kernel Module with File Operations");

// Function to write to the file
static void write_to_file(const char *str)
{
    struct file *file;
    ssize_t bytes_written;

    // Open the file in write mode
    file = filp_open(FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (IS_ERR(file))
    {
        pr_err("Failed to open file %s\n", FILE_PATH);
        return;
    }

    // Write the predefined string to the file using kernel_write
    bytes_written = kernel_write(file, str, strlen(str), &file->f_pos);
    if (bytes_written < 0)
    {
        pr_err("Failed to write to file %s\n", FILE_PATH);
    }
    else
    {
        pr_info("Successfully wrote to the file\n");
    }

    // Close the file
    filp_close(file, NULL);
}

// Function to clear the file
static void clear_file(void)
{
    struct file *file;
    ssize_t bytes_written;

    // Open the file in write mode (O_TRUNC will clear it)
    file = filp_open(FILE_PATH, O_WRONLY | O_TRUNC, 0644);
    if (IS_ERR(file))
    {
        pr_err("Failed to open file %s\n", FILE_PATH);
        return;
    }

    // Close the file to ensure it is cleared
    filp_close(file, NULL);
    pr_info("File cleared successfully\n");
}

// USB notification function
static int usb_notify1(struct notifier_block *nb, unsigned long action, void *data)
{
    struct usb_device *usb_dev = data;

    switch (action)
    {
    case USB_DEVICE_ADD:
        printk(KERN_INFO "USB device connected: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);

        // Write to the file when USB is plugged in
        write_to_file(PREDEFINED_STRING);

        // Optionally call user-space helper
        char *argv_add[] = {"/usr/bin/play_usb", NULL};
        int ret_add = call_usermodehelper(argv_add[0], argv_add, NULL, UMH_WAIT_PROC);
        printk(KERN_INFO "call_usermodehelper returned for connection: %d\n", ret_add);
        break;

    case USB_DEVICE_REMOVE:
        printk(KERN_INFO "USB device removed: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);

        // Clear the file when USB is removed
        clear_file();

        // Optionally call user-space helper
        char *argv_remove[] = {"/usr/bin/play_usb", NULL};
        int ret_remove = call_usermodehelper(argv_remove[0], argv_remove, NULL, UMH_WAIT_PROC);
        printk(KERN_INFO "call_usermodehelper returned for disconnection: %d\n", ret_remove);
        break;
    }

    return NOTIFY_OK;
}
static int usb_notify(struct notifier_block *nb, unsigned long action, void *data)
{
    struct usb_device *usb_dev = data;

    switch (action)
    {
    case USB_DEVICE_ADD:
        printk(KERN_INFO "USB device connected: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);
        write_to_file(PREDEFINED_STRING);

        // Log before calling the user-space helper
        printk(KERN_INFO "Attempting to call user-space helper for connection\n");

        // Full path and command to execute the script for USB connection
        char *argv_add[] = {"/usr/bin/play_usb", NULL};
        int ret_add = call_usermodehelper(argv_add[0], argv_add, NULL, UMH_WAIT_PROC);

        // Log the return value of call_usermodehelper for connection
        printk(KERN_INFO "call_usermodehelper returned for connection: %d\n", ret_add);
        break;

    case USB_DEVICE_REMOVE:
        printk(KERN_INFO "USB device removed: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);
        clear_file();

        // Log before calling the user-space helper for disconnection
        printk(KERN_INFO "Attempting to call user-space helper for disconnection\n");

        // Full path and command to execute the script for USB disconnection
        char *argv_remove[] = {"/usr/bin/play_usb", NULL};
        int ret_remove = call_usermodehelper(argv_remove[0], argv_remove, NULL, UMH_WAIT_PROC);

        // Log the return value of call_usermodehelper for disconnection
        printk(KERN_INFO "call_usermodehelper returned for disconnection: %d\n", ret_remove);
        break;
    }

    return NOTIFY_OK;
}

// Correct notifier block structure
static struct notifier_block usb_nb = {
    .notifier_call = usb_notify};

// Init function
static int __init usb_logger_init(void)
{
    printk(KERN_INFO "USB Logger Module Loaded\n");

    // Register the USB notifier
    usb_register_notify(&usb_nb);

    return 0;
}

// Exit function
static void __exit usb_logger_exit(void)
{
    usb_unregister_notify(&usb_nb);
    printk(KERN_INFO "USB Logger Module Unloaded\n");
}

module_init(usb_logger_init);
module_exit(usb_logger_exit);
