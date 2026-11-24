#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_LEN 1024

static char lines[MAX_LINES][MAX_LEN];
static int count = 0;

static void print_line(int n)
{
    if (n < 1 || n > count) {
        printf("%d: (out of range)\n", n);
        return;
    }
    printf("%d: %s\n", n, lines[n - 1]);
}

static void handle_token(char *tok)
{
    int a, b, i;
    char *dash;

    if (strcmp(tok, "*") == 0) {
        for (i = 1; i <= count; i++)
            print_line(i);
        return;
    }

    dash = strchr(tok, '-');
    if (dash != NULL && dash != tok) {
        if (sscanf(tok, "%d-%d", &a, &b) == 2) {
            if (a > b) {
                int t = a;
                a = b;
                b = t;
            }
            for (i = a; i <= b; i++)
                print_line(i);
            return;
        }
    }

    if (sscanf(tok, "%d", &a) == 1)
        print_line(a);
    else
        printf("invalid input: %s\n", tok);
}

int main(void)
{
    FILE *fp;
    char buf[MAX_LEN];
    char input[MAX_LEN];
    char *tok;

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

    printf("Enter line numbers (n / n,m / n-m / *): ");
    if (fgets(input, sizeof(input), stdin) == NULL)
        return 1;
    input[strcspn(input, "\r\n")] = '\0';

    tok = strtok(input, ", \t");
    while (tok != NULL) {
        handle_token(tok);
        tok = strtok(NULL, ", \t");
    }

    return 0;
}
