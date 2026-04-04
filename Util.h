#ifndef UTIL_H_
#define UTIL_H_
#include <stdbool.h>
#include "Simulator.h"
#include "Type.h"

Command hashCommand(char*);
char* reverseHashCommand(Command);
Subcommand hashSubcommand(char*);
TreeType hashTreeType(char*);
char* reverseHashTreeType(TreeType);

void printCommandFormat(Command);
void todo(char*);

void printInfo(char*);
void printWarning(char*);
void printError(char*);

#define MAX_INT_INPUT_CNT 100
bool readSubcommand(char**, Command);
bool readAndParseSubcommandToInt(int*, Command);
bool readAndParseSubcommandsToInts(int, int[], Command);
bool readAndParseSubcommandToTreeType(TreeType*, Command);

#endif  // UTIL_H