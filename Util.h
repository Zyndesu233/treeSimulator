#ifndef UTIL_H_
#define UTIL_H_
#include <stdbool.h>
#include "Simulator.h"
#include "Type.h"


// Type coverting functions
Command string2command(char*);
char* command2string(Command);
Subcommand hashSubcommand(char*);
TreeType string2treetype(char*);
char* treetype2string(TreeType);

// Printing helper functions
void setFontColor(FontColor color);
void printCommandFormat(Command);
void printInfo(const char* message_format, ...);
void printWarning(const char* message_format, ...);
void printError(const char* message_format, ...);
void todo(const char* message_format, ...);

// Command line reading helper functions
#define MAX_INT_INPUT_CNT 100
bool readSubcommand(char**, Command);
bool readAndParseSubcommandToInt(int* ret, Command caller);
bool readAndParseSubcommandsToInts(int cnt, int ret[], Command caller);
bool readAndParseSubcommandToTreeType(TreeType*, Command);

#endif  // UTIL_H