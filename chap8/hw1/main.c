#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>

extern char **environ;

static void print_user(const char *label, uid_t uid)
{
    struct passwd *pw = getpwuid(uid);
    printf("%s : %d(%s)\n", label, (int)uid, pw ? pw->pw_name : "?");
}

static void print_group(const char *label, gid_t gid)
{
    struct group *gr = getgrgid(gid);
    printf("%s : %d(%s)\n", label, (int)gid, gr ? gr->gr_name : "?");
}

static void print_env(const char *name)
{
    char *val;
    char **ep;

    if (name == NULL || name[0] == '\0') {
        for (ep = environ; *ep != NULL; ep++)
            printf("%s\n", *ep);
        return;
    }

    val = getenv(name);
    if (val == NULL)
        printf("%s: not set\n", name);
    else
        printf("%s=%s\n", name, val);
}

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "usage: %s [-e[name]] [-u] [-g] [-i] [-p]\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-e", 2) == 0) {
            print_env(argv[i] + 2);
        } else if (strcmp(argv[i], "-u") == 0) {
            print_user("My Realistic User ID", getuid());
            print_user("My Valid User ID", geteuid());
        } else if (strcmp(argv[i], "-g") == 0) {
            print_group("My Realistic Group ID", getgid());
            print_group("My Valid Group ID", getegid());
        } else if (strcmp(argv[i], "-i") == 0) {
            printf("My process number : [%d]\n", (int)getpid());
        } else if (strcmp(argv[i], "-p") == 0) {
            printf("My parent's process number : [%d]\n", (int)getppid());
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[i]);
            return 1;
        }
    }

    return 0;
}
