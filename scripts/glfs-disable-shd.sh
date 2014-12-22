#! /bin/sh

#ref https://github.com/gluster/glusterfs/blob/master/doc/admin-guide/en-US/markdown/admin_managing_volumes.md#self-heal

# disable self-heal-daemon on afr1vol; the daemon will
# be started anyway, but skip this volume
gluster volume set afr1vol cluster.self-heal-daemon off

# disable nfs
gluster volume set afr1vol nfs.disable on

# change client and brick log level
# DEBUG/<INFO>/WARNING/ERROR/CRITICAL/NONE/TRACE
gluster volume set afr1vol diagnostics.brick-log-level DEBUG
gluster volume set afr1vol diagnostics.client-log-level DEBUG

# disable cache
gluster volume set afr1vol performance.cache-max-file-size 0
gluster volume set afr1vol performance.cache-size 4194304


# disable aio
gluster volume set afr1vol linux-aio off
