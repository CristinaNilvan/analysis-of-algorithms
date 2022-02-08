/* Nilvan Cristina, grupa 10
 *
 * In caz mediu statistic HeapSort si QuickSort au performanta O(nlgn) dar QuickSort este mai eficienta. HeapSort
 * la fiecare iteratie trebuie sa refaca array-ul intr-un heap si astfel necesita multe comparatii si atribuiri.
 *
 * In caz favorabil QuickSort are o performanta de O(nlgn) iar in caz defavorabil O(n^2).
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NO_TESTS 5

int random(int min, int max) {
    return rand() % (max - min) + min;
}

void generate_array(int* array, int n) {
    srand(time(NULL));

    for (int i = 1; i <= n; i++) {
        array[i] = random(10, 500000);
    }
}

void generate_ascending_array(int* array, int n) {
    srand(time(NULL));

    array[1] = random(10, 500000);
    for (int i = 2; i <= n; i++) {
        array[i] = random(10, 500000);

        while (array[i] < array[i - 1]) {
            array[i] = random(10, 500000);
        }
    }
}

void generate_array_quick_best(int* array, int n) {
    srand(time(NULL));

    array[n] = random(10000, 50000);

    for (int i = 1; i < n; ++i) {
        if (i % 2 == 0)
            array[i] = random(10, array[n]);
        else
            array[i] = random(array[n], 500000);
    }
}

void copy_array(const int* source_array, int* destination_array, int n) {
    for (int i = 1; i <= n; i++) {
        destination_array[i] = source_array[i];
    }
}

void exchange(int* first_elem, int* second_elem) {
    int aux = *first_elem;
    *first_elem = *second_elem;
    *second_elem = aux;
}

int left_child(int i) {
    return 2 * i;
}

int right_child(int i) {
    return 2 * i + 1;
}

void max_heapify(int* array, int i, int heap_size, int* no_ass, int* no_comp) {
    int largest;
    int left = left_child(i);
    int right = right_child(i);

    *no_comp += 1;
    if ((left <= heap_size) && (array[left] > array[i]))
        largest = left;
    else
        largest = i;

    *no_comp += 1;
    if ((right <= heap_size) && (array[right] > array[largest]))
        largest = right;

    if (largest != i) {
        *no_ass += 3;
        exchange(&array[i], &array[largest]);
        max_heapify(array, largest, heap_size, no_ass, no_comp);
    }
}

void build_bottom_up(int* array, int array_length, int* no_ass, int* no_comp) {
    for (int i = array_length / 2; i >= 1; i--)
        max_heapify(array, i, array_length, no_ass, no_comp);
}

void heapsort(int* array, int array_length, int* no_ass, int* no_comp) {
    build_bottom_up(array, array_length, no_ass, no_comp);

    int array_length_copy = array_length;

    for (int i = array_length; i >= 2; i--) {
        *no_ass += 3;
        exchange(&array[1], &array[i]);
        array_length_copy -= 1;

        max_heapify(array, 1, array_length_copy, no_ass, no_comp);
    }
}

int partition(int* array, int left, int right, int* no_ass, int* no_comp) {
    int pivot = array[right];
    *no_ass += 1;

    int i = left - 1;

    for (int j = left; j <= right - 1; j++) {
        *no_comp += 1;
        if (array[j] <= pivot) {
            i += 1;

            *no_ass += 3;
            exchange(&array[i], &array[j]);
        }
    }

    *no_ass += 3;
    exchange(&array[i + 1], &array[right]);

    return (i + 1);
}

void quicksort(int* array, int left, int right, int* no_ass, int* no_comp) {
    if (left < right) {
        int partition_index = partition(array, left, right, no_ass, no_comp);

        quicksort(array, left, partition_index - 1, no_ass, no_comp);
        quicksort(array, partition_index + 1, right, no_ass, no_comp);
    }
}

int partition_quick_select(int* array, int left, int right, int pivot_index) {
    int pivot = array[pivot_index];
    exchange(&array[pivot_index], &array[right]);

    int i = left;
    for (int j = left; j <= right - 1; j++)
        if (array[j] < pivot) {
            exchange(&array[i], &array[j]);
            i += 1;
        }

    exchange(&array[right], &array[i]);

    return i;
}

int quick_select(int* array, int left, int right, int index) {
    if (left == right)
        return array[left];

    int pivot_index = random(left, right);
    pivot_index = partition_quick_select(array, left, right, pivot_index);

    if (index == pivot_index)
        return array[pivot_index];
    else if (index < pivot_index)
        return quick_select(array, left, pivot_index - 1, index);
    else
        return quick_select(array, pivot_index + 1, right, index);
}

void show_elements(int* array, int n) {
    for (int i = 1; i <= n; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");
}

void test_functions() {
    int array_length = 10;
    int array[11] = { 0, 35, 33, 42, 10, 14, 19, 27, 44, 26, 31 };
    int copied_array[11];
    int qsel_array[11];

    copy_array(array, copied_array, array_length);
    copy_array(array, qsel_array, array_length);

    printf("Random array : \n");
    show_elements(array, array_length);
    printf("\n");

    printf("The searched index is : %d\n", 4);
    int element = quick_select(qsel_array, 1, array_length, 4);
    printf("The element is : %d\n", element);
    printf("\n");

    printf("Sorted array with heapsort : \n");
    int no_ass = 0, no_comp = 0;
    heapsort(array, array_length, &no_ass, &no_comp);
    show_elements(array, array_length);
    printf("\n");

    no_ass = 0;
    no_comp = 0;
    printf("Sorted array with quicksort : \n");
    quicksort(copied_array, 1, array_length, &no_ass, &no_comp);
    show_elements(copied_array, array_length);
}

void test_sorting_average_case() {
    FILE* sum_file = fopen("sums.csv", "w");

    int heapsort_array[MAX_SIZE];
    int quicksort_array[MAX_SIZE];

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        int heapsort_no_ass = 0, heapsort_no_comp = 0;
        int quicksort_no_ass = 0, quicksort_no_comp = 0;
        int heapsort_sum, quicksort_sum;

        for (int test = 0; test < NO_TESTS; test++) {
            generate_array(heapsort_array, n);
            copy_array(heapsort_array, quicksort_array, n);

            heapsort(heapsort_array, n, &heapsort_no_ass, &heapsort_no_comp);
            quicksort(quicksort_array, 1, n, &quicksort_no_ass, &quicksort_no_comp);
        }

        heapsort_no_ass /= NO_TESTS;
        heapsort_no_comp /= NO_TESTS;
        heapsort_sum = heapsort_no_ass + heapsort_no_comp;

        quicksort_no_ass /= NO_TESTS;
        quicksort_no_comp /= NO_TESTS;
        quicksort_sum = quicksort_no_ass + quicksort_no_comp;

        fprintf(sum_file, "%d,%d,%d\n", n, heapsort_sum, quicksort_sum);
    }

    fclose(sum_file);
}

void test_quicksort_on_cases() {
    FILE* quick_file = fopen("quick.csv", "w");

    int array_worst[MAX_SIZE];
    int array_best[MAX_SIZE];

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += 500) {
        int best_no_ass = 0, best_no_comp = 0;
        int worst_no_ass = 0, worst_no_comp = 0;
        int best_sum, worst_sum;

        generate_array_quick_best(array_best, n);
        generate_ascending_array(array_worst, n);

        quicksort(array_best, 1, n, &best_no_ass, &best_no_comp);
        quicksort(array_worst, 1, n, &worst_no_ass, &worst_no_comp);

        best_sum = best_no_ass + best_no_comp;
        worst_sum = worst_no_ass + worst_no_comp;

        fprintf(quick_file, "%d,%d,%d\n", n, best_sum, worst_sum);
    }

    fclose(quick_file);
}

int main() {
    test_functions();
    //test_sorting_average_case();
    //test_quicksort_on_cases();

    return 0;
}
