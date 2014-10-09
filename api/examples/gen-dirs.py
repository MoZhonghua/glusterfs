#! /usr/bin/python

import os;
import sys;

def create_dirs(path):
	os.chdir(path)
	for i in range(0, 256):
		p = "%02x" % (i)
		os.mkdir(p)
		os.chdir(p)
		for j in range(0, 256):
			c = "%02x" % (j)
			os.mkdir(c)
			print "%s/%s" % (p, c)
		os.chdir("..")

def main():
	if (len(sys.argv) >= 2):
		create_dirs(sys.argv[1])
	else:
		create_dirs(os.getcwd())

main()

