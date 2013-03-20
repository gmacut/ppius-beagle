Character device Hello worl module
==================================

This module provides access to a virtual device that 
memorizes a character and returns it on read:

    echo "1" > /dev/mymem
    cat /dev/mymem
    1
    echo "3" > /dev/mymem
    cat /dev/mymem
    3

Prerequisites
-------------

`/dev/mymem` has to exist, you can do `touch /dev/mymem` to create it.


Building
--------

Just run `make` :)


Loading
-------

    sudo insmod mem.ko

You can now access `/dev/mymem` device.


Unloading
---------

    sudo rmmod mem
