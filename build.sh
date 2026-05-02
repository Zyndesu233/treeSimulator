#!/bin/bash
CC=gcc
CFLAGS="-Wall -Wextra"

if ! $CC *.c ./ADTs/*.c -o treeSimulator $CFLAGS -g ;then
    exit 1
fi

while getopts "is:" FLAG; do
    case "$FLAG" in 
    i)
        ./treeSimulator interpret
        break
        ;;
    s)
        ./treeSimulator simulate $OPTARG
        break
        ;;
    *)
        ;;
    esac
done