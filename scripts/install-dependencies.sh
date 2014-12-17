#! /bin/sh
# yum install vim
# yum install ntp
# yum install gcc

yum erase glusterfs
yum erase glusterfs-debuginfo
yum erase glusterfs-{cli,libs}

yum install openssl-devel
yum install python-devel
yum install flex-devel
yum install librdmacm-devel
yum install libaio{,-devel}
yum install lvm2-devel
yum install glib2-devel
yum install libxml2-devel
yum install readline{,-devel}
