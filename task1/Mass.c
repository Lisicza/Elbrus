#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000
#define NUMBER_LENGTH 10

struct thread_array {
    int length;
    int* array;
    int first;
    int real_size;
};
int cmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

void* sort(void* thr_arr) {
    struct thread_array* local_thr_arr = thr_arr;
    int* local_arr = local_thr_arr->array;
    int length = local_thr_arr->length;
    int first = local_thr_arr->first;
    if (first >= local_thr_arr->real_size) {
        return NULL;
    }
    if (first+length > local_thr_arr->real_size) {
        length = local_thr_arr->real_size - first;
    }
    if (local_thr_arr->real_size-(first+length)<length) {
        length = local_thr_arr->real_size-first;
    }
    qsort(&local_arr[first], length, sizeof(int), cmp);
    local_thr_arr->length = length;
    return NULL;
}
int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("%s", "too few arguments\n");
        return 1;
    }
    int thread_number = atoi(argv[1]);
    printf("Threads number: ");
    printf("%d", thread_number);
    printf("\n");
    printf("Enter your numbers ...\n");

    int array[ARRAY_SIZE] = {0};
    char symbol = (char)getchar();
    int pos = 0;
    char number_ch[NUMBER_LENGTH+1];
    int number_length = 0;

    while (symbol != '\n') {
        if (symbol == ' ') { // в случае когда ввели очередное число и поставили пробел
            number_ch[number_length] = '\0'; // добавляем завершающий ноль чтобы не чистить число
            array[pos++] = atoi(number_ch); // добавляем новое число в массив
            number_length = 0;
        } else {
            number_ch[number_length++] = symbol;
        }
        symbol = (char)getchar(); // читаем новый символ
    }
    if (number_length > 0) {
        number_ch[number_length] = '\0';
        array[pos++] = atoi(number_ch);
    }
    printf("Number of numbers = ");
    printf("%d", pos);
    printf("\n");

    printf("Entered numbers: ");
    for (int i = 0; i < pos; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    pthread_t thread[thread_number];
    struct thread_array thr_arr[thread_number];
    return 0;
}