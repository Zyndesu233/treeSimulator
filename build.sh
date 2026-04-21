#!/bin/bash
CC=gcc
CFLAGS="-Wall -Wextra"

while getopts "r" FLAG; do
    case "$FLAG" in 
    r)
        $CC Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator $CFLAGS && ./treeSimulator
        ;;
    *)
        $CC Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator $CFLAGS
        ;;
    esac
done