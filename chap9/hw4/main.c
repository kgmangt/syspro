#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARG 64

int main(void)
{
    char line[MAXLINE];
    char *args[MAXARG];
    char *tok;
    char *saveptr;
    int argc;
    int background;
    int status;
    int len;
    pid_t pid, child;

    while (1) {
        printf("Pls input cmd : ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        line[strcspn(line, "\r\n")] = '\0';

        len = strlen(line);
        while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '\t'))
            line[--len] = '\0';

        background = 0;
        if (len > 0 && line[len - 1] == '&') {
            background = 1;
            line[len - 1] = '\0';
        }

        argc = 0;
        tok = strtok_r(line, " \t", &saveptr);
        while (tok != NULL && argc < MAXARG - 1) {
            args[argc++] = tok;
            tok = strtok_r(NULL, " \t", &saveptr);
        }
        args[argc] = NULL;

        if (argc == 0)
            continue;

        printf("[%d] Parent process start\n", (int)getpid());

        pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            printf("[%d] child process start\n", (int)getpid());
            fflush(stdout);
            execvp(args[0], args);
            perror(args[0]);
            exit(1);
        }

        if (!background) {
            child = wait(&status);
            printf("[%d] child process end %d\n", (int)getpid(), (int)child);
        } else {
            child = waitpid(pid, &status, 0);
            printf("[%d] child process end %d\n", (int)getpid(), (int)child);
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            printf("SUCCESS\n\n");
        else
            printf("FAIL\n\n");
    }

    printf("Parent process end\n");
    printf("Exit\n");
    return 0;
}
