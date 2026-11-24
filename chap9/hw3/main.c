#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

static void run_command(char *cmd)
{
    char *args[MAX_ARGS];
    char *infile = NULL;
    char *outfile = NULL;
    char *tok;
    char *saveptr;
    int argc = 0;
    int background = 0;
    int len;
    pid_t pid;
    int fd;

    len = strlen(cmd);
    while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '\t'))
        cmd[--len] = '\0';

    if (len > 0 && cmd[len - 1] == '&') {
        background = 1;
        cmd[len - 1] = '\0';
    }

    tok = strtok_r(cmd, " \t", &saveptr);
    while (tok != NULL && argc < MAX_ARGS - 1) {
        if (strcmp(tok, ">") == 0) {
            outfile = strtok_r(NULL, " \t", &saveptr);
            if (outfile == NULL) {
                fprintf(stderr, "syntax error: missing file after >\n");
                return;
            }
        } else if (strcmp(tok, "<") == 0) {
            infile = strtok_r(NULL, " \t", &saveptr);
            if (infile == NULL) {
                fprintf(stderr, "syntax error: missing file after <\n");
                return;
            }
        } else {
            args[argc++] = tok;
        }
        tok = strtok_r(NULL, " \t", &saveptr);
    }
    args[argc] = NULL;

    if (argc == 0)
        return;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        if (infile != NULL) {
            fd = open(infile, O_RDONLY);
            if (fd < 0) {
                perror(infile);
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (outfile != NULL) {
            fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror(outfile);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(args[0], args);
        perror(args[0]);
        exit(1);
    }

    if (background)
        printf("[background] pid %d\n", (int)pid);
    else
        waitpid(pid, NULL, 0);
}

int main(void)
{
    char line[MAX_LINE];
    char *cmd;
    char *saveptr;

    signal(SIGCHLD, SIG_IGN);

    while (1) {
        printf("[shell] ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }
        line[strcspn(line, "\r\n")] = '\0';

        if (strcmp(line, "exit") == 0)
            break;

        cmd = strtok_r(line, ";", &saveptr);
        while (cmd != NULL) {
            run_command(cmd);
            cmd = strtok_r(NULL, ";", &saveptr);
        }
    }

    return 0;
}
