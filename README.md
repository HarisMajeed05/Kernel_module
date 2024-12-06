````markdown
# File Access Kernel Module on USB Plug-In

This project demonstrates a custom kernel module that triggers specific actions, such as playing a sound, upon USB plug-in or removal events. Additionally, it enables file access when a USB device is plugged in and removes that access when the USB device is unplugged.

---

## Prerequisites

### Install `sox` and required libraries:

Run the following command to install or reinstall `sox` and its associated formats:

```bash
sudo apt install --reinstall sox libsox-fmt-all
```
````

### Create a Sample `beep.wav` File:

Generate a `beep.wav` file in your `/home/<user>/` directory with the following command:

```bash
sudo sox -n beep.wav synth 2.5 sine 440
```

---

## Setup

### 1. Place the Script:

Copy the `play_usb` script to `/usr/bin/`:

```bash
sudo cp play_usb /usr/bin/
```

Make the script executable:

```bash
sudo chmod +x /usr/bin/play_usb
```

### 2. Prepare the Kernel Module:

1. Navigate to the `kernel_module` folder where the `KernelModule.c` file is located.
2. Create a `test.txt` file in the same directory:
   ```bash
   touch test.txt
   ```
3. Open the `KernelModule.c` file and **edit the path** as required in the code to ensure it matches your system configuration.

---

## Build and Load the Kernel Module

1. Open the `kernel_module` folder in a terminal.
2. Run `make` to compile the module:
   ```bash
   make
   ```
3. Insert the module:
   ```bash
   sudo insmod KernelModule.ko
   ```
4. View kernel messages for testing:
   ```bash
   sudo dmesg -w
   ```

---

## Testing

1. Plug in a USB device to test the module.
2. Remove the USB device to observe its behavior.

---

## Troubleshooting

If you encounter issues while playing sound, restart the necessary services:

```bash
systemctl --user restart pipewire pipewire-pulse wireplumber
```

---

## Contributors

- **Sana Hashim**
- **Haris Majeed Raja**

---

```

```
