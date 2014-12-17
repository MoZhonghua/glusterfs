#! /bin/sh

service glusterd stop

make uninstall

# remove volume files
rm -rfv /var/lib/glusterd

# glusterfs logs and runtime files
rm -rfv /usr/local/var/
