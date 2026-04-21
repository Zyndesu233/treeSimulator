#ifndef COMMANDS_H_
#define COMMANDS_H_
#include "Type.h"
#include <stdio.h>
void runHelp(char* mannualPage);
void runInsert(ForestPtr forestPtr, size_t index, int val);
void runInsertMany(ForestPtr forestPtr, size_t index, int count, int vals[]);
void runPrint(ForestPtr forestPtr, size_t index, FILE* outputStream);
void runDelete(ForestPtr forestPtr);
void runTraversal(ForestPtr forestPtr);
void runNew(ForestPtr forestPtr, TreeType treeType);
void runDumpForest(ForestPtr forestPtr);
void runSearch(ForestPtr forestPtr, size_t index, int val);
void runLoadTree(ForestPtr forestPtr);
#endif