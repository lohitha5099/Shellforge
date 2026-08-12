#ifndef LEXER_H
#define LEXER_H

#include "token.h"

Token *lexer_tokenize(const char *input, int *token_count);

#endif
