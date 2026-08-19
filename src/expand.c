#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "expand.h"

/*
 * Expand environment variables.
 *
 * Example:
 *     "Hello $USER"
 * becomes:
 *     "Hello lohitha"
 *
 * Also supports:
 *     $HOME
 *     $PATH
 *     ${HOME}
 */

char *expand_variables(const char *input)
{
    if (input == NULL)
        return NULL;

    size_t size = 1024;
    size_t len = 0;

    char *result = malloc(size);

    if (result == NULL)
        return NULL;

    result[0] = '\0';

    for (size_t i = 0; input[i] != '\0'; i++)
    {
        /* Normal character */
        if (input[i] != '$')
        {
            if (len + 2 >= size)
            {
                size *= 2;

                char *temp = realloc(result, size);

                if (temp == NULL)
                {
                    free(result);
                    return NULL;
                }

                result = temp;
            }

            result[len++] = input[i];
            result[len] = '\0';
            continue;
        }

        /* Handle $$ */
        if (input[i + 1] == '$')
        {
            char pid[32];

            snprintf(pid, sizeof(pid), "%d", getpid());

            size_t pid_len = strlen(pid);

            while (len + pid_len + 1 >= size)
            {
                size *= 2;

                char *temp = realloc(result, size);

                if (temp == NULL)
                {
                    free(result);
                    return NULL;
                }

                result = temp;
            }

            strcpy(result + len, pid);
            len += pid_len;

            i++;
            continue;
        }

        /* Handle ${VARIABLE} */
        if (input[i + 1] == '{')
        {
            size_t start = i + 2;
            size_t end = start;

            while (input[end] != '\0' && input[end] != '}')
                end++;

            if (input[end] == '}')
            {
                size_t var_len = end - start;

                char variable[256];

                if (var_len >= sizeof(variable))
                    var_len = sizeof(variable) - 1;

                strncpy(variable, input + start, var_len);
                variable[var_len] = '\0';

                char *value = getenv(variable);

                if (value == NULL)
                    value = "";

                size_t value_len = strlen(value);

                while (len + value_len + 1 >= size)
                {
                    size *= 2;

                    char *temp = realloc(result, size);

                    if (temp == NULL)
                    {
                        free(result);
                        return NULL;
                    }

                    result = temp;
                }

                strcpy(result + len, value);
                len += value_len;

                i = end;
                continue;
            }
        }

        /* Handle $VARIABLE */
        if (isalpha((unsigned char)input[i + 1]) ||
            input[i + 1] == '_')
        {
            size_t start = i + 1;
            size_t end = start;

            while (isalnum((unsigned char)input[end]) ||
                   input[end] == '_')
            {
                end++;
            }

            size_t var_len = end - start;

            char variable[256];

            if (var_len >= sizeof(variable))
                var_len = sizeof(variable) - 1;

            strncpy(variable, input + start, var_len);
            variable[var_len] = '\0';

            char *value = getenv(variable);

            if (value == NULL)
                value = "";

            size_t value_len = strlen(value);

            while (len + value_len + 1 >= size)
            {
                size *= 2;

                char *temp = realloc(result, size);

                if (temp == NULL)
                {
                    free(result);
                    return NULL;
                }

                result = temp;
            }

            strcpy(result + len, value);
            len += value_len;

            i = end - 1;
            continue;
        }

        /*
         * If '$' is not followed by a valid variable,
         * keep the '$' character.
         */
        if (len + 2 >= size)
        {
            size *= 2;

            char *temp = realloc(result, size);

            if (temp == NULL)
            {
                free(result);
                return NULL;
            }

            result = temp;
        }

        result[len++] = '$';
        result[len] = '\0';
    }

    return result;
}


/*
 * Expand all arguments of one command.
 */
void expand_command(Command *cmd)
{
    if (cmd == NULL)
        return;

    for (int i = 0; i < cmd->argc; i++)
    {
        if (cmd->argv[i] != NULL)
        {
            char *expanded = expand_variables(cmd->argv[i]);

            if (expanded != NULL)
            {
                cmd->argv[i] = expanded;
            }
        }
    }

    /* Expand input redirection filename */
    if (cmd->input != NULL)
    {
        char *expanded = expand_variables(cmd->input);

        if (expanded != NULL)
            cmd->input = expanded;
    }

    /* Expand output redirection filename */
    if (cmd->output != NULL)
    {
        char *expanded = expand_variables(cmd->output);

        if (expanded != NULL)
            cmd->output = expanded;
    }
}


/*
 * Expand all commands in the pipeline.
 */
void expand_pipeline(Pipeline *pipeline)
{
    if (pipeline == NULL)
        return;

    for (int i = 0; i < pipeline->count; i++)
    {
        expand_command(&pipeline->commands[i]);
    }
}
