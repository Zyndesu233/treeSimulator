#1/bin/bash
gcc Simulator.c Util.c Commands.c ./ADTs/AVLTree.c ./ADTs/BinarySearchTree.c ./ADTs/TreeNode.c ./ADTs/SplayTree.c ./ADTs/RedBLackTree.c -o main -Wall -Wextra && ./main