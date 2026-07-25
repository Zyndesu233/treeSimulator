# Tree Simulator

> [!WARNING]
> The current version (v6.0.0) is build Cursor.
> It is not reviewed yet and may contain AI slop.
> For stable human-written version, use v5.2.0.

A C rewrite of the original version. It keeps the CLI REPL workflow, prints readable ASCII trees, and uses an OOP-in-C style tree interface so concrete tree implementations can share the same REPL commands.

The first complete implementation includes:

- Binary search tree (`bst`)
- AVL tree (`avl`)
- Splay tree (`splay`, `spl`)
- Red-black tree (`rbt`)
- B-tree (`btree`, `b-tree`, `bt`)

All tree implementations share the same `binary_tree_vtable` contract.

## Build

```sh
make
```

The simulator binary is written to:

```sh
bin/tree-simulator
```

## Run

Start the REPL:

```sh
make run
```

or:

```sh
./bin/tree-simulator
./bin/tree-simulator repl
./bin/tree-simulator -r
```

Run commands from a file:

```sh
./bin/tree-simulator simulate examples/basic_bst.commands
./bin/tree-simulator -s examples/basic_bst.commands
```

## Commands

```text
new|n <bst|avl|splay|rbt>      create a tree
new|n btree <min_degree>       create a B-tree
insert|i <index> <value>       insert a node
insert-many|im <index> <count> <values...>
                                insert multiple nodes
delete|d <index> <value>       delete a node
find|f|search|s <index> <value>
                                find a node
print|p <index>                print a tree
dump                           list all trees
help|h [command]               show help or command detail
quit|q|exit                    leave the simulator
```

Example:

```text
new bst
im 0 3 1 2 3
print 0
```

Output:

```text
[INFO] A new binary search tree with index 0 is created.
[INFO] Inserted 3 of 3 node(s) in tree 0.
[INFO] Printing tree 0 (binary search tree).
    +-3
  +-2
+-1
```

Red-black tree red nodes are printed in red without extra `(R)` labels. Black nodes use the terminal default color. Warnings are yellow and errors are red.

```text
  +-15
+-10
  +-5
```

B-tree nodes print multiple keys inside brackets:

```text
  +-[40 50]
-[20]
  +-[5 10]
```

## Tests

Run all tests:

```sh
make test
```

This builds and runs:

- Unit tests for parser utilities.
- Unit tests for BST and AVL operations.
- Unit tests for splay search/root behavior.
- Unit tests for red-black tree inserts, deletes, and invariants.
- Unit tests for B-tree insert, delete, find, and invariants.
- A script-based simulation test that compares command-file output against an expected file.

Run a simulation test manually:

```sh
sh scripts/run_simulation_test.sh examples/basic_bst.commands examples/basic_bst.expected
```

If no expected file is provided, the script prints the simulator output:

```sh
sh scripts/run_simulation_test.sh examples/basic_bst.commands
```

## Project Layout

```text
src/main.c                  thin CLI entry point
src/core/forest.*           manages multiple indexed trees
src/trees/binary_tree.*     base node and vtable contract
src/trees/avl_tree.*        AVL tree implementation
src/trees/btree.*           B-tree implementation
src/trees/binary_search_tree.*
                             binary search tree implementation
src/trees/red_black_tree.*  red-black tree implementation
src/trees/splay_tree.*      splay tree implementation
src/repl/input.*            token parsing and validation helpers
src/repl/commands.*         REPL command handlers
src/repl/repl.*             interactive and file-mode command loop
src/output/tree_printer.*   original `+-` ASCII tree printer
help/*.txt                  help text dumped by help commands
tests/test_main.c           unit test runner
scripts/run_simulation_test.sh
```

## References

- Tree printer algorithm adapted from [How can I print a binary tree?](https://stackoverflow.com/a/64688332), answer by Luis Colorado, retrieved 2026-07-25, licensed under [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).

## Clean

```sh
make clean
```
