#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define BUFSIZE 256

int fd;
int my_pid;

void *read_f(void *arg) {
    char buf[BUFSIZE];
    int n;
    while (1) {
        n = read(fd, buf, BUFSIZE - 1);
        if (n > 0) {
            buf[n] = '\0';
            // printf("%s\n", buf);
            int sender;
            char msg[BUFSIZE];
            if (sscanf(buf, "%d:%[^\n]", &sender, msg) == 2 && sender != my_pid) {
                printf(" > ");
                printf("%s\n", msg);
            } else {
                write(fd, buf, BUFSIZE-1);
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "too few arguments\n");
        return 1;
    }
    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    my_pid = getpid();
    printf("Chat started\n");

    pthread_t reader;
    pthread_create(&reader, NULL, read_f, NULL);

    char buf[BUFSIZE];
    while (fgets(buf, BUFSIZE, stdin)) {
        if (strlen(buf) != 0) {
            char out[BUFSIZE+20];
            int len = snprintf(out, sizeof(out), "%d:%s\n", my_pid, buf);
            write(fd, out, len);
        }
    }
    pthread_cancel(reader);
    close(fd);
    return 0;
}