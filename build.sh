#!/bin/bash
CC=gcc
CFLAGS="-Wall -Wextra"

while getopts "rc:" FLAG; do
    case "$FLAG" in 
    r)
        $CC Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator $CFLAGS && ./treeSimulator simulate
        ;;
    c)
        $CC Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator $CFLAGS && ./treeSimulator compile $OPTARG
        ;;
    *)
        $CC Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator $CFLAGS
        ;;
    esac
done