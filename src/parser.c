#include <stdio.h>
#include <string.h>
#include "parser.h"

void init_command(Command *cmd)
{
    int i;

    cmd->argc = 0;
    cmd->input = NULL;
    cmd->output = NULL;
    cmd->append = 0;
    cmd->background = 0;

    for (i = 0; i < MAX_ARGS; i++)
    {
        cmd->argv[i] = NULL;
    }
}

void init_pipeline(Pipeline *pipeline)
{
    int i;

    pipeline->count = 0;

    for (i = 0; i < MAX_ARGS; i++)
    {
        init_command(&pipeline->commands[i]);
    }
}

int parse_tokens(Token *tokens, Pipeline *pipeline)
{
    int i = 0;
    Command *cmd;

    init_pipeline(pipeline);

    cmd = &pipeline->commands[0];
    pipeline->count = 1;

    while (tokens[i].type != TOKEN_END)
    {
        if (tokens[i].type == TOKEN_WORD)
        {
            if (cmd->argc < MAX_ARGS - 1)
            {
                cmd->argv[cmd->argc] = tokens[i].value;
                cmd->argc++;

                cmd->argv[cmd->argc] = NULL;
            }
        }

        else if (tokens[i].type == TOKEN_INPUT)
        {
            i++;

            if (tokens[i].type == TOKEN_WORD)
            {
                cmd->input = tokens[i].value;
            }
        }

        else if (tokens[i].type == TOKEN_OUTPUT)
        {
            i++;

            if (tokens[i].type == TOKEN_WORD)
            {
                cmd->output = tokens[i].value;
                cmd->append = 0;
            }
        }

        else if (tokens[i].type == TOKEN_APPEND)
        {
            i++;

            if (tokens[i].type == TOKEN_WORD)
            {
                cmd->output = tokens[i].value;
                cmd->append = 1;
            }
        }

        else if (tokens[i].type == TOKEN_BACKGROUND)
        {
            cmd->background = 1;
        }

        else if (tokens[i].type == TOKEN_PIPE)
        {
            if (pipeline->count < MAX_ARGS)
            {
                cmd = &pipeline->commands[pipeline->count];
                pipeline->count++;

                init_command(cmd);
            }
        }

        i++;
    }

    return 0;
}

void print_pipeline(Pipeline *pipeline)
{
    int i, j;
    Command *cmd;

    printf("\n");
    printf("=========== PIPELINE ===========\n");
    printf("\n");

    for (i = 0; i < pipeline->count; i++)
    {
        cmd = &pipeline->commands[i];

        printf("Command %d\n", i + 1);
        printf("-------------------------------\n");

        printf("Arguments\n");

        for (j = 0; j < cmd->argc; j++)
        {
            printf("argv[%d] = %s\n", j, cmd->argv[j]);
        }

        printf("Input    : %s\n",
               cmd->input ? cmd->input : "None");

        printf("Output   : %s\n",
               cmd->output ? cmd->output : "None");

        printf("Append   : %s\n",
               cmd->append ? "Yes" : "No");

        printf("Background : %s\n",
               cmd->background ? "Yes" : "No");

        printf("===============================\n");
    }
}
