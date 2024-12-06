#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/file.h> // For filp_open and filp_close

#define FILE_PATH "/home/haris-majeed-raja/Desktop/OS/Kernel_module/Kernel_module-main/test.txt"
#define PREDEFINED_STRING "Hello from the kernel module!\n"

// Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Haris Majeed Raja");
MODULE_DESCRIPTION("A simple kernel module that writes a string to a file");

// Init function: executed when the module is loaded
static int __init my_module_init(void)
{
    struct file *file;
    ssize_t bytes_written;

    // Open the file in write mode
    file = filp_open(FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (IS_ERR(file))
    {
        pr_err("Failed to open file %s\n", FILE_PATH);
        return PTR_ERR(file);
    }

    // Write the predefined string to the file using kernel_write
    bytes_written = kernel_write(file, PREDEFINED_STRING, strlen(PREDEFINED_STRING), &file->f_pos);
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
    return 0;
}

// Exit function: executed when the module is unloaded
static void __exit my_module_exit(void)
{
    pr_info("Kernel module exited\n");
}

// Register init and exit functions
module_init(my_module_init);
module_exit(my_module_exit);
