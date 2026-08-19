#ifndef PARSER_H
#define PARSER_H

#include "token.h"

/* Maximum number of arguments in one command */
#define MAX_ARGS 64

/* Represents one parsed command */
typedef struct
{
    char *argv[MAX_ARGS];
    int argc;

    char *input;
    char *output;

    int append;
    int background;

} Command;


/* Represents the complete parsed pipeline */
typedef struct
{
    Command commands[MAX_ARGS];
    int count;

} Pipeline;


/* Parser functions */
void init_command(Command *cmd);
void init_pipeline(Pipeline *pipeline);

int parse_tokens(Token *tokens, Pipeline *pipeline);

void print_pipeline(Pipeline *pipeline);

#endif
