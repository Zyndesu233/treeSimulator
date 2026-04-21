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
bool readAndParseSubcommandToSize(size_t* ret, Command caller);
bool readAndParseSubcommandToTreeType(TreeType*, Command);

// Macros for dynammic array
#define da_append(da, item)\
do {\
    if((da).count >= (da).capacity) {\
        if((da).capacity == 0) (da).capacity = 256;\
        else (da).capacity *= 2;\
        (da).items = realloc((da).items, (da).capacity*sizeof(*(da).items));\
    }\
    (da).items[(da).count++] = item;\
} while (0)

#endif  // UTIL_H