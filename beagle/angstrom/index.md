Beagleboard* with Angstrom distribution
======================================

\* revision C4



## 1. Software

### 1.1 Booting the BeagleBoard

BeagleBoard uses Das uBoot bootloader. Booting is well documented in the 
[demo image official documentation][1] Default setting should be fine. Basically, only the 
default Serial output has to be configured to boot Linux. [Demo image][1] kernel currently 
uses `tty02` as output, some other images use `ttyS2`.

To configure console enter uBoot environment and then:
 
 * In the uBoot environment: `bootargs` and `console` parameters have to have the right console
 output, it can be checked using `printenv`, and set using `setenv`. See the example bellow for 
 a valid uBoot environment.

Settings can be changed from the uBoot prompt with setenv command, for example:

    setenv console 'ttyO2,115200n8'

##### 1.1.1 uBoot environment that works with [BeagleBoard Angstrom demo download (including instructions)][1]

    OMAP3 beagleboard.org # printenv
    baudrate=115200
    beaglerev=C4
    bootargs=console=ttyO2,115200n8 root=/dev/mmcblk0p2 rw rootwait omapfb.mode=dvi:hd720 single g_ether.host_addr=16:0F:15:5A:E1:21  g_ether.dev_addr=16:0F:15:5A:E1:20
    bootcmd=mmc rescan;fatload mmc 0 0x80300000 uImage;bootm 0x80300000
    bootdelay=2
    bootenv=uEnv.txt
    bootfile=uImage
    buddy=unknown
    buddy2=unknown
    camera=none
    console=ttyO2,115200n8
    defaultdisplay=dvi
    dieid#=5f84000400000000040373050e005009
    dvimode=640x480MR-16@60
    filesize=30C25C
    importbootenv=echo Importing environment from mmc ...; env import -t $loadaddr $filesize
    loadaddr=0x80200000
    loadbootenv=fatload mmc ${mmcdev} ${loadaddr} ${bootenv}
    loadramdisk=fatload mmc ${mmcdev} ${rdaddr} ramdisk.gz
    loaduimage=ext2load mmc ${mmcdev}:2 ${loadaddr} /boot/uImage
    loaduimagefat=fatload mmc ${mmcdev} ${loadaddr} uImage
    mmcargs=setenv bootargs console=${console} ${optargs} mpurate=${mpurate} buddy=${buddy} buddy2=${buddy2} camera=${camera} vram=${vram} omapfb.mode=dvi:${dvimode} omapdss.def_disp=${defaultdisplay} root=${mmcroot} rootfstype=${mmcrootfstype}
    mmcboot=echo Booting from mmc ...; run mmcargs; bootm ${loadaddr}
    mmcdev=0
    mmcroot=/dev/mmcblk0p2 ro
    mmcrootfstype=ext3 rootwait
    mpurate=auto
    nandargs=setenv bootargs console=${console} ${optargs} mpurate=${mpurate} buddy=${buddy} buddy2=${buddy2} camera=${camera} vram=${vram} omapfb.mode=dvi:${dvimode} omapdss.def_disp=${defaultdisplay} root=${nandroot} rootfstype=${nandrootfstype}
    nandboot=echo Booting from nand ...; run nandargs; nand read ${loadaddr} 280000 400000; bootm ${loadaddr}
    nandroot=ubi0:rootfs ubi.mtd=4
    nandrootfstype=ubifs
    ramargs=setenv bootargs console=${console} ${optargs} mpurate=${mpurate} buddy=${buddy} buddy2=${buddy2} vram=${vram} omapfb.mode=dvi:${dvimode} omapdss.def_disp=${defaultdisplay} root=${ramroot} rootfstype=${ramrootfstype}
    ramboot=echo Booting from ramdisk ...; run ramargs; bootm ${loadaddr}
    ramroot=/dev/ram0 rw ramdisk_size=65536 initrd=0x81000000,64M
    ramrootfstype=ext2
    rdaddr=0x81000000
    usbtty=cdc_acm
    vram=12M




### 1.2 Using the BeagleBoard Angstromg [demo image][1]

Angstrom installation on the BeagleBoard, following the official documentation([here][1] and 
[here][2]) is very easy, but unfortunately, not without problems.

Any approach tired for enabling USB networking between BeagleBoard and a PC host has failed, 
current image contains USB networking kernel modules only for an earlier kernel version.

If we want to use serial port using the default image we have to edit the Angstrom distribution 
settings (on SD card). The right console has to be set in the `/etc/inittab` file on the 
Angstrom distribution SD card. **NOTE:** there is no need for this configuration on the newer 
custom build distributions.

### 1.3 [Narcissus online build system][7]

Angstrom distributions can be customized using [Naricissus][7]. This approach has not been 
tried yet.


### 1.4 Building Angstrom

#### 1.4.1 Docs

 - [OpenEmbedded Getting Started Guide][3]
 - [Angstrom site: Building angstrom][4]
 - [Open Embedded User Manual][5] 
    * targets and stuff
 - [How To Build Custom Angstrom Build for BeagleBone with Ubuntu or Debian][6]


#### 1.4.2 Build steps

Performed on Ubuntu 12.04

    sudo apt-get install git

    sudo apt-get install gawk wget git-core diffstat unzip texinfo build-essential chrpath libsdl1.2-dev xterm

    sudo apt-get install texi2html 

    MACHINE=beagleboard ./oebb.sh config beagleboard
    MACHINE=beagleboard ./oebb.sh update
    MACHINE=beagleboard ./oebb.sh bitbake virtual/kernel

    MACHINE=beagleboard ./oebb.sh bitbake u-boot
    MACHINE=beagleboard ./oebb.sh bitbake console-image

Prepared images can be used like the demo images, following the [official documentation][1]
should lead to fully working system.







[1]: http://downloads.angstrom-distribution.org/demo/beagleboard/   "BeagleBoard Angstrom demo download site"
[2]: http://elinux.org/BeagleBoardBeginners "BeagleBoard Angstrom instalation tutorial"
[3]: http://wiki.openembedded.org/index.php/Getting_Started "Open Embedded Getting Started Guide"
[4]: http://www.angstrom-distribution.org/building-angstrom "Angstrom site: Building Angstrom"
[5]: http://docs.openembedded.org/usermanual/html/ "Open Embedded User Manual"
[6]: http://cwraig.id.au/?p=507 "How To Build Custom Angstrom Build for BeagleBone with Ubuntu or Debian"
[7]: http://narcissus.angstrom-distribution.org/ "Narcissus - Angstrom Custom Builds Creator"