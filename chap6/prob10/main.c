#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void run(const char *cmd)
{
    printf("$ %s\n", cmd);
    fflush(stdout);
    if (system(cmd) != 0)
        fprintf(stderr, "command failed: %s\n", cmd);
    printf("\n");
}

int main(void)
{
    run("echo 'This is the week4 subtask' > test.txt");
    run("ls -sl test.txt");
    run("stat test.txt");
    run("ls -i test.txt");

    printf("---- touch: before ----\n");
    run("ls -l --time-style=full-iso test.txt");

    sleep(2);

    run("touch test.txt");

    printf("---- touch: after ----\n");
    run("ls -l --time-style=full-iso test.txt");

    run("stat test.txt");

    return 0;
}
