#! /bin/sh

# lib not found
ldd gluster

find /usr/local/ -name "libglusterfs.so.0"

echo /usr/local/lib/ > /etc/ld.so.conf.d/glusterfs.conf

ldconfig

ldconfig -p | grep libglusterfs.so.0

service glusterd start
