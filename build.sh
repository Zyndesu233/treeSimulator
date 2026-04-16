#!/bin/bash
while getopts "r" FLAG; do
    case "$FLAG" in 
    r)
        gcc Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator -Wall -Wextra && ./treeSimulator
        ;;
    *)
        gcc Simulator.c Util.c Commands.c ./ADTs/*.c -o treeSimulator -Wall -Wextra
        ;;
    esac
done