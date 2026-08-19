#ifndef EXPAND_H
#define EXPAND_H

#include "parser.h"

/* Expand environment variables such as $HOME, $USER, etc. */
char *expand_variables(const char *input);

/* Expand variables in all command arguments */
void expand_command(Command *cmd);

/* Expand variables in the complete pipeline */
void expand_pipeline(Pipeline *pipeline);

#endif
