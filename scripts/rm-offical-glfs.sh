#! /bin/sh

service glusterd stop

# cleanup glusterfs
yum erase glusterfs
yum erase glusterfs-debuginfo
yum erase glusterfs-{cli,libs}

rm -rfv /var/log/glusterfs/
rm -rfv /var/lib/glusterd/
