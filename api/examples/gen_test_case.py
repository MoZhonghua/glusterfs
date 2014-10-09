#! /usr/bin/python

import os
import time
import sys
import optparse


# worker / size / count / mnt_point
test_cases = [
		(1,		0,		100000),
		(2,		0,		100000),
		(5,		0,		100000),
		(10,	0,		100000),

		(1,		4096,	100000),
		(2,		4096,	100000),
		(5,		4096,	100000),
		(10,	4096,	100000),

		(1,  1024*1024,   10000),
		(2,  1024*1024,   10000),
		(5,  1024*1024,   10000),
		(10, 1024*1024,   10000),

		(1, 16*1024*1024, 1000),
		(2, 16*1024*1024, 1000),
		(5, 16*1024*1024, 1000),
		(10, 16*1024*1024, 1000),

		]

def now():
	t = time.gmtime()
	return "%04d-%02d-%02d_%02d%02d%02d" %   \
			(t.tm_year, t.tm_mon, t.tm_mday, \
			t.tm_hour, t.tm_min, t.tm_sec)

def gen_tests_scripts(test_cases, is_write=True, is_api=True, mnt_point="", server="", volume=""):
	for (worker, size, count) in test_cases:
		cmd_common ="-w%d -s%d -n%d" % (worker, size, count)
		output_common = "w%d-s%d-n%d"  % (worker, size, count)

		if is_api:
			output = "%s_%s.log" % (output_common, volume)
			if (is_write):
				print "../open_files_test %s %s %s | tee api_wr_%s" % \
						(cmd_common, server, volume, output)
			else:
				print "../open_files_test -r -f api_wr_%s %s %s %s | tee api_rd_%s" % \
						(output, cmd_common, server, volume, output)
		else:
			output = "%s_%s.log" % (output_common, mnt_point.replace("/", "_"))
			if (is_write):
				print "../open_files_test -x %s %s | tee mnt_wr_%s" % \
						(cmd_common, mnt_point, output)
			else:
				print "../open_files_test -x -r -f mnt_wr_%s %s %s | tee mnt_rd_%s" % \
						(output, cmd_common, mnt_point, output)

def main():
	#gen_tests_scripts(test_cases, is_api=False, mnt_point="/mnt/testdata")
	#gen_tests_scripts(test_cases, is_api=False, is_write=False, mnt_point="/mnt/testdata")
	#gen_tests_scripts(test_cases, server="x2", volume="afrvol")
	#gen_tests_scripts(test_cases, is_write=False, server="x2", volume="afrvol")

	parser = optparse.OptionParser()
	parser.add_option("-r", action="store_false", dest="is_write", default=True, help="read mode")
	parser.add_option("-x", action="store_false", dest="is_api", default=True, help="using mount to r/w")
	(options, args) = parser.parse_args()

	if (options.is_api):
		gen_tests_scripts(test_cases, is_write=options.is_write, server=args[0], volume=args[1])
	else:
		gen_tests_scripts(test_cases, is_api=False, is_write=options.is_write, mnt_point=args[0])

main()




