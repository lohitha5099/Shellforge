#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

Token create_token(TokenType type, const char *value)
{
    Token token;

    token.type = type;

    token.value = malloc(strlen(value) + 1);

    if (token.value == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(token.value, value);

    return token;
}

void print_token(Token token, int index)
{
    if (token.type == TOKEN_WORD)
    {
        printf("%d : WORD    %s\n", index, token.value);
    }
    else if (token.type == TOKEN_END)
    {
        printf("%d : END     %s\n", index, token.value);
    }
}

void free_token(Token *token)
{
    if (token != NULL)
    {
        free(token->value);
        token->value = NULL;
    }
}
