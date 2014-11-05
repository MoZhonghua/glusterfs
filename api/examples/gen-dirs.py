#! /usr/bin/python

import os;
import sys;

def ensure_dir(f):
	if not os.path.exists(f):
		os.mkdir(f)

def create_dirs(path):
	os.chdir(path)
	for i in range(0, 256):
		p = "%02x" % (i)
		ensure_dir(p)
		os.chdir(p)
		for j in range(0, 256):
			c = "%02x" % (j)
			ensure_dir(c)
			print "%s/%s" % (p, c)
		os.chdir("..")

def main():
	if (len(sys.argv) >= 2):
		create_dirs(sys.argv[1])
	else:
		create_dirs(os.getcwd())

main()

