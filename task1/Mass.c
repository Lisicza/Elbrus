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

void merge(int L[], int R[], int L_size, int R_size) {
    int* S = malloc((L_size+R_size)*sizeof(int));
    int l = 0;
    int r = 0;
    int s = 0;
    while (l < L_size && r < R_size) {
        if (L[l] <= R[r]) {
            S[s] = L[l++];
        }
        else {
            S[s] = R[r++];
        }
        s++;
    }
    while (l < L_size) {
        S[s++] = L[l++];
    }
    while (r < R_size) {
        S[s++] = R[r++];
    }
    for (int i =0; i<R_size+L_size; i++) {
        L[i] = S[i];
    }
    free(S);
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

    for (int i = 0; i < thread_number; i++) {
        thr_arr[i].array = array;
        thr_arr[i].first = i*(pos/thread_number);
        thr_arr[i].length = pos/thread_number;
        thr_arr[i].real_size = pos;
        pthread_create(&thread[i], NULL, sort, &thr_arr[i]);
    }

    for (int i = 0; i < thread_number; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("Presorted numbers: ");
    for (int i = 0; i < pos; i++) {
        printf("%d ", array[i]);
    }
    int sum = 0;
    for (int i = 0; i < thread_number; i++) {
        printf("\nBlock ");
        printf("%d", i);
        printf(" length: ");
        printf("%d", thr_arr[i].length);
        sum += thr_arr[i].length;
    }
    printf("\nSum length: ");
    printf("%d", sum);
    printf("\n");

    int merged_len = thr_arr[0].length;   // длина объединённого блока
    int R_start = merged_len;
    for (int i = 1; i < thread_number; i++) {
        merge(&array[0], &array[R_start], merged_len, thr_arr[i].length);
        merged_len += thr_arr[i].length;
        R_start = merged_len;
    }

    printf("Sorted numbers: ");
    for (int i = 0; i < pos; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}