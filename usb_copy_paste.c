#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Haris Majeed Raja");
MODULE_DESCRIPTION("USB Connection and Activity Logger Kernel Module");

// Function to log read/write activity
static int usb_block_read_write(struct inode *inode, struct file *file, unsigned int cmd)
{
    struct usb_device *usb_dev = file->private_data;
    if (usb_dev)
    {
        switch (cmd)
        {
        case 0: // Simulated read action for illustrative purposes
            printk(KERN_INFO "USB Device Read Action Detected: Vendor ID=0x%04x, Product ID=0x%04x\n",
                   usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);
            break;
        case 1: // Simulated write action for illustrative purposes
            printk(KERN_INFO "USB Device Write Action Detected: Vendor ID=0x%04x, Product ID=0x%04x\n",
                   usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);
            break;
        }
    }
    return 0;
}

// Notifier function that gets called on USB events
static int usb_notify(struct notifier_block *nb, unsigned long action, void *data)
{
    struct usb_device *usb_dev = data;

    switch (action)
    {
    case USB_DEVICE_ADD:
        printk(KERN_INFO "USB device connected: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);
        break;

    case USB_DEVICE_REMOVE:
        printk(KERN_INFO "USB device removed: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);
        break;
    }
    return NOTIFY_OK;
}

// Notifier block structure
static struct notifier_block usb_nb = {
    .notifier_call = usb_notify};

// Register the file operations for the USB block device
static int __init usb_logger_init(void)
{
    printk(KERN_INFO "USB Logger Module Loaded\n");
    usb_register_notify(&usb_nb);

    // Here, we would hook into the file operations for each USB block device
    // For example:
    // register_blkdev(major_num, "usb_device");

    return 0;
}

static void __exit usb_logger_exit(void)
{
    usb_unregister_notify(&usb_nb);
    printk(KERN_INFO "USB Logger Module Unloaded\n");
}

module_init(usb_logger_init);
module_exit(usb_logger_exit);
