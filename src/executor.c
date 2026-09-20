#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "executor.h"

void execute_pipeline(Pipeline *pipeline)
{
    for (int i = 0; i < pipeline->count; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            return;
        }

        if (pid == 0)
        {
            execvp(pipeline->commands[i].argv[0],
                   pipeline->commands[i].argv);

            perror("execvp");
            exit(EXIT_FAILURE);
        }

        if (!pipeline->commands[i].background)
        {
            waitpid(pid, NULL, 0);
        }
    }
}
