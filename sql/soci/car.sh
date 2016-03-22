#!/bin/bash

programmname=gtkprog
sources="`ls | grep '.cc'`"
export LD_LIBRARY_PATH=/usr/local/lib64/

#g++ $sources -std=gnu++11 `pkg-config --cflags --libs gtk+-3.0 libsoci` -o $programmname  &> compilerlog

g++ $sources -std=gnu++11 `pkg-config --cflags --libs gtk+-3.0`-lsoci_core -lsoci_sqlite3 -I /usr/local/include/soci/ -L /home/oesi/Projekte/build/lib/ -I /home/oesi/Projekte/soci-3.2.3  -I /home/oesi/Projekte/soci-3.2.3/core -o $programmname  &> compilerlog

if [[ $? -eq 0 ]]; then
	./$programmname
else
	lines="`cat compilerlog | wc -l`"
	if [[ $lines -gt 20 ]]; then
		less compilerlog
	else
		cat compilerlog
	fi
fi
