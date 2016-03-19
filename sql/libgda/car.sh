#!/bin/bash

programmname=gtkprog
sources="`ls | grep '.cc'`"

g++ $sources -std=gnu++11 `pkg-config --cflags --libs gtk+-3.0 libgda-5.0` -o $programmname  &> compilerlog


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
