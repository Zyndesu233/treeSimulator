# Todos
- Support B-trees
- Compile mode for checking

# Change Log
## v5.1.0
Factor out input handling from command simulation.

Modified Commands:
- HELP
- INSERT
- INSERT_MANY
- PRINT
- NEW
- DUMP_FOREST
- SEARCH\
    SEARCH command for red black tree is implemented.

## v5.0.0
Dynamic array is used as memory allocation system for trees.

Modified Commands:
- DUMP_FOREST\
    DUMP_TREES is renamed to DUMP_FOREST.
- NEW
- LOAD_TREE

The above commands adapt the new memory allocation system and no longer takes index as arguments.

New Utility Commands:
- `readAndParseSubcommandToSize`

## v4.1.0
New Utility Commands:
- `setFontColor`\
    Allow deveoper to set terminal text color.
- `todo`

Others:
- `build.sh` is updated\
    `-r` flag is introduced for build and run

## v4.0.0
New Utility Commands:
- `string2command`
- `command2string`
- `hashSubcommand`
- `treetype2string`
- `string2treetype`

Modified Utility Commands:
- `printInfo`
- `printWarning`
- `printError`

The above printing helper functions now support string format like `printf`.

Others:
- `README.md` is updated
- `build.sh` is updated
- The CLI is improved
- New ASCII logo is introduced

## v3.4.1
Modified Command:
- LOAD_TREE\
    It supports red black trees now.

## v3.4.0
The source code is reviewed.

New Tree Supported:
- Red Black Tree

New Utility Commands:
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

Removed Utility Commands:
- `parseSubcommandTo2Int`\
    It is equivalent to call `readAndParseSubcommandToInt` twice and hence is deprecated.

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