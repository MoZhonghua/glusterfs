#! /usr/bin/python
import sys
import os

def list_split(l, s):
	r = []
	for i in l:
		r += i.split(s)
	return r

def multi_split(str, seps):
	result = [str]
	for s in seps:
		result = list_split(result,  s)
	return result

def reconstruct_testcase(filename):
	parts = multi_split(os.path.basename(filename), ["-", "_"])
	type = parts[0]
	rw = parts[1]
	worker = parts[2]
	size = parts[3]
	count = parts[4]
	path = parts[len(parts)-2]

	print type, rw, worker, size, count, path

def main():
	if len(sys.argv) <= 1:
		print "no file"
		return

	reconstruct_testcase(sys.argv[1])

main()

