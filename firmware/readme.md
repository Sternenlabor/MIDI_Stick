# Configure probe for Linux
The probe doesn't use any device for upload and debug. It uses libusb so no device is created for that.
Only for UART messages a device is create /dev/ttyACMx (parameter monitor_port)

Add following rule: /etc/udev/rules.d/99-cmsis-dap.rules
```
SUBSYSTEM=="usb", ATTRS{idVendor}=="0d28", ATTRS{idProduct}=="0204", MODE="0666", GROUP="plugdev"
```
Be sure that your user is in group *plugdev*. After adding the rule restart udev
```
sudo udevadm control --reload-rules
sudo udevadm trigger
```
and connect the probe.