USB networking configuration with running Angstrom Linux
========================================================


1. Desired network configuration
--------------------------------

The desired network configuration consists of a Linux PC connected to the Web, and
a BeagleBoard conected to the PC using an USB networking. We call that PC 
computer HOST. Host is connected to a local 192.168.1.0 wireless network 
(DHCP, default gateway 192.168.1.1, DNS also 192.168.1.1, host identifies network
as wlan0). 
The goal is to share that Web access with the BeagleBoard.

BeagleBoard connects to the host using USB networking. Both host and the BeagleBoard
identify the interface as usb0). Static configuration of the network parameters is 
used, DHCP failed on the BeagleBoard side (when the board asks for parameters it 
looses all connectivity and responivity). The chosen configuration for a network 
between the board and the host is:

 - Network:         192.168.137.0
 - BeagleBoard's address: 192.168.137.20
 - Host address:    192.168.137.1
 - DNS server:      8.8.8.8

We use Google's public DNS to avoid this network's dependency to the configuration
of the external network. That way we can change he way host accesses the Internet,
and keep the BeagleBoard configuration the same.


2. Configuration
----------------

### 2.1 The BeagleBoard side:

 - changed relevant part of `/etc/network/interfaces` to

        iface usb0 inet static
            pre-up modprobe g_ether host_addr=16:0F:15:5A:E1:21 dev_addr=16:0F:15:5A:E1:20 
            address 192.168.137.20
            netmask 255.255.255.0
            network 192.168.137.0
            gateway 192.168.137.1
            dns-nameservers 8.8.8.8
            post-down rmmod g_ether

 - `ifup usb0`
 - something overwrites the `/etc/resolv.conf` file and sets up a wrong DNS, we have to 
   change it. Set the DNS server in the `/etc/resolv.conf` to:

        nameserver 8.8.8.8

 - `ifconfig` should return something like:
    
        root@beagleboard:~# ifconfig
        ...
        usb0    Link encap:Ethernet  HWaddr 16:0F:15:5A:E1:20  
                inet addr:192.168.137.20  Bcast:192.168.137.255  Mask:255.255.255.0
                inet6 addr: fe80::140f:15ff:fe5a:e120/64 Scope:Link
                UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
                RX packets:18234 errors:0 dropped:0 overruns:0 frame:0
                TX packets:9642 errors:0 dropped:0 overruns:0 carrier:0
                collisions:0 txqueuelen:1000 
                RX bytes:26825873 (25.5 MiB)  TX bytes:706261 (689.7 KiB)

 - `route -n` should return something like:

        root@beagleboard:~# route -n
        Kernel IP routing table
        Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
        0.0.0.0         192.168.137.1   0.0.0.0         UG    0      0        0 usb0
        192.168.137.0   0.0.0.0         255.255.255.0   U     0      0        0 usb0

 - after the configuration of the host side, `ping www.google.com` returns

        root@beagleboard:~# ping www.google.com
        PING www.google.com (173.194.44.52): 56 data bytes
        64 bytes from 173.194.44.52: seq=0 ttl=55 time=38.940 ms
        64 bytes from 173.194.44.52: seq=1 ttl=55 time=38.997 ms
        64 bytes from 173.194.44.52: seq=2 ttl=55 time=39.675 ms



### 2.2 The host side

Host PC was runing an updated version of the Ubuntu 12.04 (kernel 3.2.0-35-generic-pae). 
After enabling of the BeagleBoard usb0 interface, the host detects the connection and 
automatically enables DHCP on it. We set the desired static parameters (ip 192.168.137.1/24, 
leave gateway empty) using NetworkManager GUI. After this, we should be able to ping 
192.168.137.20 from the host, and 192.168.137.1 from the BeagleBoard.

Alternatively, if we want to use manual network configuration, we can use this:

 - change relevant part of `/etc/network/interfaces` to

        iface usb0 inet static
            address 192.168.137.1
            netmask 255.255.255.0
            network 192.168.137.0

 - `ifup usb0`

When we have network configured, we can setup BeagleBoard Internet access.

Firstly, we have to enable Network Address Translation for our 
BeagleBoard - host network:

    sudo -i
    iptables -t nat -A POSTROUTING -o wlan0 -j MASQUERADE
    echo "1" > /proc/sys/net/ipv4/ip_forward
    exit

Also, we can remove the current configuration by using the similar calls:

    sudo -i
    iptables -t nat -D POSTROUTING -o wlan0 -j MASQUERADE
    echo "0" > /proc/sys/net/ipv4/ip_forward
    exit

Now, from the BeagleBoard we should be able to do `ping 192.168.1.1` to ping the outer
network's gateway, or `ping www.google.com` to access the Web.
