#ifndef COMMANDS_H_
#define COMMANDS_H_
#include "Type.h"
void runHelp();
void runInsert(ForestPtr forestPtr);
void runInsertMany(ForestPtr forestPtr);
void runPrint(ForestPtr forestPtr);
void runDelete(ForestPtr forestPtr);
void runTraversal(ForestPtr forestPtr);
void runNew(ForestPtr forestPtr);
void runDumpForest(ForestPtr forestPtr);
void runSearch(ForestPtr forestPtr);
void runLoadTree(ForestPtr forestPtr);
#endif