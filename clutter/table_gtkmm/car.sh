#!/bin/bash

export LD_LIBRARY_PATH=/usr/local/lib64/
programmname=gtkprog
sources="`ls | grep '.cc'`"

if [[ $1 = "win" ]]; then
  echo "todo"
else
  g++ $sources -std=gnu++11 `pkg-config --cflags --libs gtkmm-3.0 clutter-1.0 clutter-gtk-1.0` -o $programmname  &> compilerlog 
fi

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
