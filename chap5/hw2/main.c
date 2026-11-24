#include <stdio.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_LEN 1024

static char lines[MAX_LINES][MAX_LEN];

int main(void)
{
    FILE *fp;
    char buf[MAX_LEN];
    int count = 0;
    int i;

    fp = fopen("test.txt", "r");
    if (fp == NULL) {
        perror("test.txt");
        return 1;
    }

    while (count < MAX_LINES && fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strcspn(buf, "\r\n")] = '\0';
        strcpy(lines[count], buf);
        count++;
    }
    fclose(fp);

    for (i = count - 1; i >= 0; i--)
        printf("%s\n", lines[i]);

    return 0;
}
