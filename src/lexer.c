#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "token.h"

#define INITIAL_CAPACITY 16

static int add_token(Token **tokens,
                     int *count,
                     int *capacity,
                     TokenType type,
                     const char *value)
{
    if (*count >= *capacity - 1)
    {
        *capacity *= 2;

        Token *temp = realloc(*tokens,
                              sizeof(Token) * (*capacity));

        if (temp == NULL)
        {
            perror("realloc");
            return -1;
        }

        *tokens = temp;
    }

    (*tokens)[*count] = create_token(type, value);
    (*count)++;

    return 0;
}

Token *lexer_tokenize(const char *input, int *token_count)
{
    if (input == NULL || token_count == NULL)
        return NULL;

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
        /* Skip whitespace */
        if (isspace((unsigned char)input[i]))
        {
            i++;
            continue;
        }

        /* Pipe */
        if (input[i] == '|')
        {
            if (add_token(&tokens, token_count, &capacity,
                          TOKEN_PIPE, "|") != 0)
            {
                free(tokens);
                return NULL;
            }

            i++;
            continue;
        }

        /* Input redirection */
        if (input[i] == '<')
        {
            if (add_token(&tokens, token_count, &capacity,
                          TOKEN_INPUT, "<") != 0)
            {
                free(tokens);
                return NULL;
            }

            i++;
            continue;
        }

        /* Output / append redirection */
        if (input[i] == '>')
        {
            if (input[i + 1] == '>')
            {
                if (add_token(&tokens, token_count, &capacity,
                              TOKEN_APPEND, ">>") != 0)
                {
                    free(tokens);
                    return NULL;
                }

                i += 2;
            }
            else
            {
                if (add_token(&tokens, token_count, &capacity,
                              TOKEN_OUTPUT, ">") != 0)
                {
                    free(tokens);
                    return NULL;
                }

                i++;
            }

            continue;
        }

        /* Background */
        if (input[i] == '&')
        {
            if (add_token(&tokens, token_count, &capacity,
                          TOKEN_BACKGROUND, "&") != 0)
            {
                free(tokens);
                return NULL;
            }

            i++;
            continue;
        }

        /* Read a normal word */
        char word[256];
        int j = 0;

        while (input[i] != '\0' &&
               !isspace((unsigned char)input[i]) &&
               input[i] != '|' &&
               input[i] != '<' &&
               input[i] != '>' &&
               input[i] != '&')
        {
            if (j < (int)sizeof(word) - 1)
            {
                word[j++] = input[i];
            }

            i++;
        }

        word[j] = '\0';

        if (j > 0)
        {
            if (add_token(&tokens, token_count, &capacity,
                          TOKEN_WORD, word) != 0)
            {
                for (int k = 0; k < *token_count; k++)
                    free_token(&tokens[k]);

                free(tokens);
                return NULL;
            }
        }
    }

    /* END token */
    if (add_token(&tokens, token_count, &capacity,
                  TOKEN_END, "END") != 0)
    {
        for (int k = 0; k < *token_count; k++)
            free_token(&tokens[k]);

        free(tokens);
        return NULL;
    }

    return tokens;
}
