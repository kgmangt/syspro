#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

static int opt_i = 0;
static int opt_p = 0;
static int opt_q = 0;

static void mode_string(mode_t m, char *s)
{
    if (S_ISDIR(m))
        s[0] = 'd';
    else if (S_ISLNK(m))
        s[0] = 'l';
    else if (S_ISCHR(m))
        s[0] = 'c';
    else if (S_ISBLK(m))
        s[0] = 'b';
    else if (S_ISFIFO(m))
        s[0] = 'p';
    else if (S_ISSOCK(m))
        s[0] = 's';
    else
        s[0] = '-';
    s[1] = (m & S_IRUSR) ? 'r' : '-';
    s[2] = (m & S_IWUSR) ? 'w' : '-';
    s[3] = (m & S_IXUSR) ? 'x' : '-';
    s[4] = (m & S_IRGRP) ? 'r' : '-';
    s[5] = (m & S_IWGRP) ? 'w' : '-';
    s[6] = (m & S_IXGRP) ? 'x' : '-';
    s[7] = (m & S_IROTH) ? 'r' : '-';
    s[8] = (m & S_IWOTH) ? 'w' : '-';
    s[9] = (m & S_IXOTH) ? 'x' : '-';
    s[10] = '\0';
}

static void print_entry(const char *dir, const char *name)
{
    char path[4096];
    char mode[11];
    char tbuf[64];
    const char *slash;
    struct stat st;
    struct passwd *pw;
    struct group *gr;

    snprintf(path, sizeof(path), "%s/%s", dir, name);
    if (lstat(path, &st) < 0) {
        perror(path);
        return;
    }

    mode_string(st.st_mode, mode);
    pw = getpwuid(st.st_uid);
    gr = getgrgid(st.st_gid);
    strftime(tbuf, sizeof(tbuf), "%b %d %H:%M", localtime(&st.st_mtime));

    printf("%5ld ", (long)st.st_blocks);

    if (opt_i)
        printf("%lu ", (unsigned long)st.st_ino);

    printf("%s %3lu %s %s %10ld %s ",
           mode,
           (unsigned long)st.st_nlink,
           pw ? pw->pw_name : "?",
           gr ? gr->gr_name : "?",
           (long)st.st_size,
           tbuf);

    slash = (opt_p && S_ISDIR(st.st_mode)) ? "/" : "";

    if (opt_q)
        printf("\"%s%s\"\n", name, slash);
    else
        printf("%s%s\n", name, slash);
}

int main(int argc, char *argv[])
{
    int opt;
    const char *dir;
    DIR *dp;
    struct dirent *d;

    while ((opt = getopt(argc, argv, "ipQ")) != -1) {
        switch (opt) {
        case 'i':
            opt_i = 1;
            break;
        case 'p':
            opt_p = 1;
            break;
        case 'Q':
            opt_q = 1;
            break;
        default:
            fprintf(stderr, "usage: %s [-ipQ] [directory]\n", argv[0]);
            return 1;
        }
    }

    dir = (optind < argc) ? argv[optind] : ".";

    dp = opendir(dir);
    if (dp == NULL) {
        perror(dir);
        return 1;
    }

    while ((d = readdir(dp)) != NULL)
        print_entry(dir, d->d_name);

    closedir(dp);
    return 0;
}
