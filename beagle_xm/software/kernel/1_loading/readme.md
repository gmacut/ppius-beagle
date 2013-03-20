Kernel module Hello world!
==========================

Outputs messages to system log when loading and unloading module.
Based on [this](http://blog.markloiseau.com/2012/04/hello-world-loadable-kernel-module-tutorial/).

Prerequisites
-------------

    $ sudo -i
    # apt-get install module-assistant
    # m-a prepare


Building
--------

Just run `make` :)


Loading
-------

    sudo insmod hello.ko

Check log with `dmesg | tail` to view message.


Unloading
---------

    sudo rmmod hello

Check log with `dmesg | tail` to view message.
