# Tree Simulator
A tree simulator written in C aim for visualize different tree abstract data type.

Now supports binary search trees, AVL trees, splay trees and red black trees.

Some functions are not yet implemented. Pull requests are welcomed.

## How to Run?
1. Run `./build.sh` to build the project.
2. Run `./treeSimulator` to start the simulator.

OR

1. Run `./build.sh -r` to build and run the simulator.

## How to get started?
1. Run `h` or `help` in the simulator to display the whole command list.
2. Run `h <command>` or `help <command>` for detailed explanation for a command.

## Example
```
>  n bst
[INFO] A new binary search tree labeled as index 0 is created.

> i 0 1
[INFO] TreeNode(1) inserted in Tree(0).

> i 0 2
[INFO] TreeNode(2) inserted in Tree(0).

> i 0 3
[INFO] TreeNode(3) inserted in Tree(0).

> p 0
[INFO] Printing Tree(0):
    +-3
  +-2
+-1

> s 0 1
[INFO] Node(1) exists in Tree(0)

> d 0 1
[INFO] TreeNode(1) deleted in Tree(0).

> p 0
[INFO] Printing Tree(0):
  +-3
+-2
```  

## File Structure
1. `Simulator.h` and `Simulator.c` implement the workflow of the simulator.
2. `Commands.h` and `Commands.c` implement the commands used by users.
3. `Type.h` defines the custom types.
4. `Util.h` and `Util.c` contain helper functions for developers.
5. `.h` and `.c` files under `./ADTs/` implement different data structures.

## Reference
1. [How can I print a binary tree?](https://stackoverflow.com/questions/64660540/how-can-i-print-a-binary-tree) \
    The function `print_node` and `print_subtree` in `Commands.c` is inspired by the answer from user Luis Colorado.