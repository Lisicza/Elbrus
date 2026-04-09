#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int visible(const char *s) {
    for (; *s; s++)
        if (*s != ' ') {
            return 1;
        }
    return 0;
}

int plain(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}
int lex(const void *a, const void *b) {
    return strcasecmp(*(char**)a, *(char**)b);
}
int rplain(const void *a, const void *b) {
    return -strcmp(*(char**)a, *(char**)b);
}
int rlex(const void *a, const void *b) {
    return -strcasecmp(*(char**)a, *(char**)b);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("too few arguments\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Cant open input file\n");
        return 1;
    }

    char **lines = NULL;
    char *line = NULL;
    size_t n = 0;
    size_t cap = 0;
    while (getline(&line, &cap, f) != -1) {
        line[strcspn(line, "\n")] = 0;
        if (visible(line)) {
            lines = realloc(lines, (++n) * sizeof(char*));
            lines[n-1] = strdup(line);
        }
    }
    free(line);
    fclose(f);
    if (n) {
        int (*cmp_function)(const void*, const void*) = plain;
        if (strcmp(argv[3], "lex") == 0) {
            cmp_function = lex;
        }
        else if (strcmp(argv[3], "rplain") == 0) {
            cmp_function = rplain;
        }
        else if (strcmp(argv[3], "rlex") == 0) {
            cmp_function = rlex;
        }
        qsort(lines, n, sizeof(char*), cmp_function);

        f = fopen(argv[2], "w");
        for (size_t i = 0; i < n; i++) {
            fprintf(f, "%s\n", lines[i]);
            free(lines[i]);
        }
        free(lines);
        fclose(f);
    }
    return 0;
}