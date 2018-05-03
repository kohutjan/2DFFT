#!/bin/bash

cmd=false
gui=false

if [ $# -eq 1 ];then 
    arg=$1;
    if [ "$arg" = "cmd" ];then
	cmd=true
    elif [ "$arg" = "gui" ];then
	gui=true
    fi; 
else
    cmd=true
    gui=true
fi;

if [ "$cmd" = true ];then
    rm -rf build
    mkdir build
    cd build 
    cmake ..
    make -j 7
    cd ..
fi;

if [ "$gui" = true ];then
    rm -rf buildGUI
    mkdir buildGUI
    cd buildGUI
    /opt/Qt/5.10.1/gcc_64/bin/qmake ../GUI/
    make -j 7
    cp -r ../GUI/images .
    cd ..
fi;
