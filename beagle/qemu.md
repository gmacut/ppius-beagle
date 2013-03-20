Running in Qemu
===============

BeagleBoard can be emulated in QEMU. Unfortunately, emulating network access was not successfull.
It is documented [here][1] and [here][2] as possible, but wasn't successfull.


QEMU installation
-----------------

    sudo add-apt-repository ppa:linaro-maintainers/tools
    sudo apt-get update
    sudo apt-get install linaro-image-tools



Running
-------

QEMU emulates BeagleBoard hardware and we only need to give it access to the content of the SD card.
We can point it to a SD card, or we can create an image file with the required content.

In both cases, emulated BeagleBoard's serial port is connected to terminal, and we can communicate
with the board using that connection.


### Using SD card

If we have a working BeagleBoard Linux installation, we can use that SD card to boot in QEMU. First, 
we have to make sure both partitions (boot and filesystem) from SD card **ARE NOT MOUNTED** on the 
host computer. Then we have to found the SD cards device name.(not the partitions, the card itself!).
We can find it using `dmesg | tail` after inserting the card. Then we can point the QEMU to the card:

    sudo qemu-system-arm -M beagle -drive if=sd,cache=writeback,file=/dev/mmcblk0 -clock unix -serial stdio -device usb-kbd -device usb-mouse -usb -device usb-net,netdev=mynet -netdev user,id=mynet

**Remarks:**

 - [Based on this][3]
 - No networking



### Using an image file

If we want, we can point QEMU to local image file, instead of the SD cards device file. Image file 
has to contain the SD card data from both partitions (boot and filesystem). If we know the SD card's 
device name (see previous paragaph), we can copy the SD cart to the image file using:

    sudo dd if=/dev/mmcblk0 of=card.img
    sudo chown goran card.img  -- to enable running without root privileges

then we can run QEMU using:
    
    sudo qemu-system-arm -M beagle -drive if=sd,cache=writeback,file=/dev/mmcblk0 -clock unix -serial stdio -device usb-kbd -device usb-mouse -usb -device usb-net,netdev=mynet -netdev user,id=mynet

**Remarks:**

 - [Based on this][3]
 - No networking



[1]: https://blueprints.launchpad.net/qemu-linaro/+spec/add-omap3-networking    "Linaru QEMU: Add USB networking support for OMAP3"
[2]: https://bugs.launchpad.net/qemu-linaro/+bug/656530     "Unable to get networking to function"
[3]: https://wiki.linaro.org/Resources/HowTo/Qemu-beagleboard   "Booting a QEMU based BeagleBoard"