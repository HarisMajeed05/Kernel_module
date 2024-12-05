#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/notifier.h>
#include <linux/kmod.h> // For call_usermodehelper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Haris Majeed Raja");
MODULE_DESCRIPTION("USB Connection Logger Kernel Module with Sound on Connection");

static int usb_notify(struct notifier_block *nb, unsigned long action, void *data)
{
    struct usb_device *usb_dev = data;

    switch (action)
    {
    case USB_DEVICE_ADD:
        printk(KERN_INFO "USB device connected: Vendor ID=0x%04x, Product ID=0x%04x\n",
               usb_dev->descriptor.idVendor, usb_dev->descriptor.idProduct);

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

static int __init usb_logger_init(void)
{
    printk(KERN_INFO "USB Logger Module Loaded\n");

    // Register the USB notifier (no return value to capture)
    usb_register_notify(&usb_nb); // No need for 'ret' here

    return 0;
}

static void __exit usb_logger_exit(void)
{
    usb_unregister_notify(&usb_nb);
    printk(KERN_INFO "USB Logger Module Unloaded\n");
}

module_init(usb_logger_init);
module_exit(usb_logger_exit);
