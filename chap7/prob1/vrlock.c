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
    int fd, id, newscore;
    off_t off = 0;
    off_t pos = -1;
    struct student rec;
    struct flock lock;

    if (argc < 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror(argv[1]);
        return 1;
    }

    printf("Enter student ID to modify: ");
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

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = pos;
    lock.l_len = sizeof(rec);

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

    printf("[locked] name: %s  id: %d  score: %d\n", rec.name, rec.id, rec.score);
    printf("Enter new score (record stays locked until you press Enter): ");
    if (scanf("%d", &newscore) != 1) {
        close(fd);
        return 1;
    }

    rec.score = newscore;
    if (pwrite(fd, &rec, sizeof(rec), pos) != sizeof(rec)) {
        perror("pwrite");
        close(fd);
        return 1;
    }

    printf("updated: name: %s  id: %d  score: %d\n", rec.name, rec.id, rec.score);

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror(argv[1]);
        close(fd);
        return 1;
    }

    printf("[unlocked]\n");
    close(fd);
    return 0;
}
