// Ajouter les directives d'inclusion nécessaires
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main() {

    if (fork() == 0)  {
        mkfifo("pipe2", 0600);

        if (fork() == 0) {
            mkfifo("pipe1", 0600);      

            if (fork() == 0) {
                int pipe1 = open("pipe1", O_WRONLY); dup2(pipe1, 1); close(pipe1);
                execlp("cat", "cat", "In.txt", NULL);
                _exit(0);
            }
            int pipe2 = open("pipe2", O_WRONLY); dup2(pipe2, 1); close(pipe2);
            int pipe1 = open("pipe1", O_RDONLY); dup2(pipe1, 0); close(pipe1);
            execlp("tr", "tr","[a-z]","[A-Z]", NULL);
            _exit(0);

        }

        int out = open("Outn.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660);
        dup2(out, 1);
        close(out);
        int pipe2 = open("pipe2", O_RDONLY); dup2(pipe2, 0); close(pipe2);
        execlp("diff", "diff", "-", "In.txt", NULL);
        _exit(0);
    }

    wait(NULL);

    unlink("pipe1");
    unlink("pipe2");
    return 0;
}



