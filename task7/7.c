#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 4096

void handler(int sig) {} // обработчик сигнала (пустой, тк ничего не делает)

void copy(const char *in_file, const char *out_file) {
    int in_fd = open(in_file, O_RDONLY);
    if (in_fd == -1) {
        printf("cant open input file");
        exit(1);
    }

    int out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) {
        printf("cant open output file");
        close(in_fd);
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    ssize_t size = read(in_fd, buffer, sizeof(buffer));
    while (size> 0) {
        if (write(out_fd, buffer, size) != size) {
            printf("write ERROR");
            break;
        }
        size = read(in_fd, buffer, sizeof(buffer));
    }
    if (size == -1) {
        printf("read ERROR");
    }
    close(in_fd);
    close(out_fd);
}

void print(const char *file, const char *sender) {
    FILE *out_file = fopen(file, "r");
    if (!out_file) {
        printf("cant open output file (print)");
        return;
    }
    printf("%s:\n", sender);
    int symbol;
    while ((symbol = fgetc(out_file)) != EOF) {
        putchar(symbol);
    }
    fclose(out_file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Enter file name");
    }
    const char *file = argv[1];

    // тк родитель и ребенок при одновременной печати будут мешать друг другу (строчки путаются местами)
    // нужно чтобы ребенок дождался окончания печати родителя (дождался сигнала)
    signal(SIGUSR1, handler); // сигнал, который родитель посылает сыну по завершению печати
    pid_t pid = fork();
    if (pid < 0) {
        printf("fork ERROR");
    }

    if (pid == 0) { // процесс ребенок
        copy(file, "child_copy");
        pause(); // засыпаем до получения любого сигнала
        print("child_copy", "Child");
    } else { // процесс отец
        copy(file, "parent_copy");
        print("parent_copy", "Parent");
        kill(pid, SIGUSR1);
        wait(NULL); // ожидаем дочерний процесс
    }
    return 0;
}
