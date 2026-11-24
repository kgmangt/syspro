#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct student {
    char name[20];
    int id;
    int score;
};

int main(int argc, char *argv[])
{
    int fd, id;
    off_t off = 0;
    off_t pos = -1;
    struct student rec;
    struct flock lock;

    if (argc < 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror(argv[1]);
        return 1;
    }

    printf("Enter student ID to search: ");
    if (scanf("%d", &id) != 1) {
        close(fd);
        return 1;
    }

    while (pread(fd, &rec, sizeof(rec), off) == sizeof(rec)) {
        if (rec.id == id) {
            pos = off;
            break;
        }
        off += sizeof(rec);
    }

    if (pos < 0) {
        printf("student %d not found\n", id);
        close(fd);
        return 1;
    }

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = pos;
    lock.l_len = sizeof(rec);

    printf("waiting for read lock...\n");
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror(argv[1]);
        close(fd);
        return 1;
    }

    if (pread(fd, &rec, sizeof(rec), pos) != sizeof(rec)) {
        perror("pread");
        close(fd);
        return 1;
    }

    printf("name: %s  id: %d  score: %d\n", rec.name, rec.id, rec.score);

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror(argv[1]);
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
