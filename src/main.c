#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../include/lexer.h"
#include "../include/token.h"

int main()
{
    printf("=====================================\n");
    printf("        Shellforge\n");
    printf("  A Unix Style Shell written in C\n");
    printf("=====================================\n");

    while (1)
    {
        char *input = readline("shellforge$ ");

        if (input == NULL)
        {
            printf("\nExiting...\n");
            break;
        }

        if (strlen(input) == 0)
        {
            free(input);
            continue;
        }

        if (strcmp(input, "exit") == 0)
        {
            free(input);
            printf("Exiting...\n");
            break;
        }

        add_history(input);

        int token_count = 0;

        Token *tokens = lexer_tokenize(input, &token_count);

        printf("\n------------ TOKENS ------------\n");

        for (int i = 0; i < token_count; i++)
        {
            print_token(tokens[i], i);
        }

        printf("--------------------------------\n");

        for (int i = 0; i < token_count; i++)
        {
            free_token(&tokens[i]);
        }

        free(tokens);
        free(input);
    }

    return 0;
}
