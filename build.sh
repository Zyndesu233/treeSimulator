#!/bin/bash
CC=gcc
CFLAGS="-Wall -Wextra"

while getopts "rs:" FLAG; do
    case "$FLAG" in 
    r)
        $CC Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator $CFLAGS && ./treeSimulator repl
        ;;
    s)
        $CC Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator $CFLAGS && ./treeSimulator simulate $OPTARG
        ;;
    *)
        $CC Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator $CFLAGS
        ;;
    esac
done