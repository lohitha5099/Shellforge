#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "token.h"

#define INITIAL_CAPACITY 16

Token *lexer_tokenize(const char *input, int *token_count)
{
    if (input == NULL || token_count == NULL)
    {
        return NULL;
    }

    *token_count = 0;

    int capacity = INITIAL_CAPACITY;

    Token *tokens = malloc(sizeof(Token) * capacity);

    if (tokens == NULL)
    {
        perror("malloc");
        return NULL;
    }

    int i = 0;

    while (input[i] != '\0')
    {
        /* Skip spaces */
        while (input[i] != '\0' &&
               isspace((unsigned char)input[i]))
        {
            i++;
        }

        /* End of input */
        if (input[i] == '\0')
        {
            break;
        }

        /* Increase token array if necessary */
        if (*token_count >= capacity - 1)
        {
            capacity *= 2;

            Token *temp =
                realloc(tokens, sizeof(Token) * capacity);

            if (temp == NULL)
            {
                perror("realloc");
                free(tokens);
                return NULL;
            }

            tokens = temp;
        }

        /* Read one word */
        char word[256];
        int j = 0;

        while (input[i] != '\0' &&
               !isspace((unsigned char)input[i]))
        {
            if (j < (int)sizeof(word) - 1)
            {
                word[j] = input[i];
                j++;
            }

            i++;
        }

        word[j] = '\0';

        /* Create WORD token */
        if (j > 0)
        {
            tokens[*token_count] =
                create_token(TOKEN_WORD, word);

            (*token_count)++;
        }
    }

    /* Add END token */
    if (*token_count >= capacity)
    {
        capacity++;

        Token *temp =
            realloc(tokens, sizeof(Token) * capacity);

        if (temp == NULL)
        {
            perror("realloc");
            free(tokens);
            return NULL;
        }

        tokens = temp;
    }

    tokens[*token_count] =
        create_token(TOKEN_END, "END");

    (*token_count)++;

    return tokens;
}
