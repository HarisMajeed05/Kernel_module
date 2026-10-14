# USB File Access Kernel Module

A Linux kernel module that reacts to USB plug-in and removal. On each event it runs a script that plays a sound, and it turns access to a file on when a USB device is plugged in and off when it is removed.

## Requirements

- Linux with kernel headers installed
- `sox` for the sound

```bash
sudo apt install --reinstall sox libsox-fmt-all
```

## Setup

### 1. Create a sound file

```bash
sox -n ~/beep.wav synth 2.5 sine 440
```

### 2. Create the `play_usb` script

The module calls `/usr/bin/play_usb` on every USB event. Create it with:

```bash
sudo tee /usr/bin/play_usb > /dev/null <<'SCRIPT'
#!/bin/bash
play /home/<user>/beep.wav
SCRIPT
sudo chmod +x /usr/bin/play_usb
```

Replace `<user>` with your username.

### 3. Set the file path

`KernelModule.c` has a `FILE_PATH` define. Change it to a file on your system, for example the `test.txt` in this folder:

```bash
touch test.txt
```

## Build and load

```bash
make
sudo insmod KernelModule.ko
sudo dmesg -w
```

Plug in a USB device and remove it to see the messages. To unload the module:

```bash
sudo rmmod KernelModule
```

## Troubleshooting

If the sound does not play, restart the audio services:

```bash
systemctl --user restart pipewire pipewire-pulse wireplumber
```

## Authors

Sana Hashim and Haris Majeed Raja
