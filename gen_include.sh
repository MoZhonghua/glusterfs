#! /bin/sh

find . -name '*.h' -exec dirname {} \; | uniq | sort > ycm.include

