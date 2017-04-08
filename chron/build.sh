#!/bin/bash

echo -------------------------STARTBUILD----------------------------

#compile
gcc -c plugin.c -fPIC `pkg-config --cflags geany` 

#link
gcc plugin.o -o plugin.so -shared `pkg-config --libs geany` 

#install
read -p "install the plugin? (y/n): " choice
if [ "$choice" == "y" ]
then
	sudo cp plugin.so /usr/lib/x86_64-linux-gnu/geany/
fi

