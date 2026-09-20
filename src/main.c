#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "expand.h"
#include "builtin.h"
#include "executor.h"

#define MAX_INPUT 1024

int main(void)
{
    char input[MAX_INPUT];

    printf("====================================\n");
    printf("          Shellforge\n");
    printf("A Unix Style Shell written in C\n");
    printf("====================================\n");

    while (1)
    {
        printf("shellforge$ ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        int token_count = 0;

        Token *tokens = lexer_tokenize(input, &token_count);

        if (tokens == NULL)
        {
            fprintf(stderr, "Lexer error\n");
            continue;
        }

        printf("\n-------------- TOKENS --------------\n");

        for (int i = 0; i < token_count; i++)
        {
            print_token(tokens[i], i);
        }

        printf("-------------------------------------\n");

        Pipeline pipeline;

        if (parse_tokens(tokens, &pipeline) != 0)
        {
            fprintf(stderr, "Parser error\n");

            for (int i = 0; i < token_count; i++)
                free_token(&tokens[i]);

            free(tokens);
            continue;
        }

        expand_pipeline(&pipeline);

        print_pipeline(&pipeline);

        if (handle_builtin(pipeline.commands[0].argv))
        {
            for (int i = 0; i < token_count; i++)
                free_token(&tokens[i]);

            free(tokens);
            continue;
        }

        execute_pipeline(&pipeline);

        for (int i = 0; i < token_count; i++)
            free_token(&tokens[i]);

        free(tokens);
    }

    printf("\nShellforge terminated.\n");

    return 0;
}
