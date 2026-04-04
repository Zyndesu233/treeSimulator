# Change Log
## v3.4.0
The source code is reviewed.

New Tree Supported:
- Red Black Tree

New Utility Command:
- `printInfo`
- `printWarning`
- `printError`
- `readSubcommand`
- `readAndParseSubcommandsToInts`

## v3.3.1
Modified Command:
- LOAD_TREE

## v3.3.0
New Command:
- LOAD_TREE

New Utility Command:
- `readAndParseSubcommandToTreeType`

## v3.2.1
The manul for each command is updated.

Modified Command:
- HELP

## v3.2.0
`Commands.c` is modified.

Modified Commands:
- HELP
- SEARCH

New Utility Command:
- `printCommandFormat`
- `todo`

## v3.1.0
New Utility Command:
- `readAndParseSubcommandToInt`

Deprecated Utility Commands:
- `parseSubcommandTo2Int`: Equivalent to call `readAndParseSubcommandToInt` twice.

## v3.0.0
The simulator is reconstructed.

New Feature:
- `Util.h` and `Util.c` are added for utility functions benefits developers

New tree supported:
- Splay tree

New Commands:
- Search

New Utility Command:
- `parseSubcommandTo2Int`

## v2.1.1
New Commands:
- DELETE

## v2.1.0
New tree supported:
- Binary Search Tree

## v2.0.1
Modified Commands:
- PRINT

Deprecated Commands:
- CLEAR: Equivalent to call NEW on existing tree.

## v2.0.0
The simulator is reconstructed.

New Features:
- The simulator can now maintain multiple trees
- The simulator can now handle different kinds of trees

New Commands:
- NEW
- DUMP_TREE

Modified Commands:
- QUIT
- INSERT
- INSERT_MANY
- TRAVERSAL

## v1.2.1
Modified Command:
- HELP

## v1.2.0
New Command:
- INSERT_MANY

## v1.1.0
New Command:
- TRAVERSAL

## v1.0.0
New tree supported:
- AVL Tree

New Commands:
- QUIT
- HELP
- INSERT
- PRINT
- DELETE
- CLEAR