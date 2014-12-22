#! /usr/bin/python

import os
import sys
import socket

# 0x00000001000000006666666699999998

def parse_dht_xattr(s):
    # len("0x00000001000000006666666699999998"):
    if len(s) != 34:
	return (0, 0, 0, 0)

    count_str = s[2:9]
    type_str = s[10:17]
    start_str = s[18:25]
    end_str = s[26:33]

    return (socket.ntohl(int(count_str, 16)), 
	    socket.ntohl(int(type_str, 16)), 
	    socket.ntohl(int(start_str, 16)), 
	    socket.ntohl(int(end_str, 16)))

def main():
    r = parse_dht_xattr(sys.argv[1])
    print r

    if r == (0, 0, 0, 0):
	return

    interval = (r[3] - r[2]) 
    print "%d %d\n" % (0xFFFFFFFF / interval, interval)

main()
