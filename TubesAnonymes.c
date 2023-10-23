// Ajouter les directives d'inclusion nécessaires
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    if (fork() == 0)  {
        int pipe2[2];
        pipe(pipe2);

        if (fork() == 0) {
            int pipe1[2];
            pipe(pipe1);

            if (fork() == 0) {
                dup2(pipe1[1], 1); close(pipe1[0]); close(pipe1[1]);
                execlp("cat", "cat", "In.txt", NULL);
                _exit(0);
            }

            dup2(pipe2[1], 1); close(pipe2[0]); close(pipe2[1]);
            dup2(pipe1[0], 0); close(pipe1[0]); close(pipe1[1]);
            execlp("tr", "tr","[a-z]","[A-Z]", NULL);
            _exit(0);
        }

        int out = open("Output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
        dup2(out, 1);
        close(out);
        dup2(pipe2[0], 0); close(pipe2[0]); close(pipe2[1]);

        execlp("diff", "diff", "-", "In.txt", NULL);
        _exit(0);
    }

    return 0;
}