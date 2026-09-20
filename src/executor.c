#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "executor.h"

void execute_pipeline(Pipeline *pipeline)
{
    int previous_pipe = -1;
    pid_t pids[MAX_ARGS];

    for (int i = 0; i < pipeline->count; i++)
    {
        int pipefd[2];

        if (i < pipeline->count - 1)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                return;
            }
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            return;
        }

        if (pid == 0)
        {
            if (previous_pipe != -1)
            {
                if (dup2(previous_pipe, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                close(previous_pipe);
            }

            if (i < pipeline->count - 1)
            {
                if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                close(pipefd[0]);
                close(pipefd[1]);
            }

            execvp(pipeline->commands[i].argv[0],
                   pipeline->commands[i].argv);

            perror("execvp");
            exit(EXIT_FAILURE);
        }

        pids[i] = pid;

        if (previous_pipe != -1)
            close(previous_pipe);

        if (i < pipeline->count - 1)
        {
            close(pipefd[1]);
            previous_pipe = pipefd[0];
        }
    }

    for (int i = 0; i < pipeline->count; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
}
