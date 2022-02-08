/* Nilvan Cristina, grupa 10
 * In cazul mediu statistic se pot observa urmatoarele :
 * - cea mai rapida metoda este Bottom Up
 * - cele mai putine atribuiri le are Bottom Up
 * - cele mai putine comparatii le are Top Down, dar la o diferenta foarte mica Bottom Up
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_SIZE 10100
#define STEP_SIZE 100
#define NO_TESTS 5

void generate_array(int* array, int n) {
    srand(time(NULL));

    for (int i = 1; i <= n; i++) {
        array[i] = rand() % (500000 - 10) + 10;
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

int parent(int i) {
    return i / 2;
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

void heap_insert(int* array, int* heap_size, int new_value, int* no_ass, int* no_comp) {
    *heap_size = *heap_size + 1;
    array[*heap_size] = new_value;
    *no_ass += 1;

    int position = *heap_size;

    while (position > 1) {
        int parent_node = parent(position);

        *no_comp += 1;
        if (array[parent_node] < array[position]) {
            *no_ass += 3;
            exchange(&array[parent_node], &array[position]);
            position = parent_node;
        }
        else {
            return;
        }
    }
}

void build_top_down(int* array, int* heap, int array_length, int* no_ass, int* no_comp) {
    int position = 0;

    for (int i = 1; i <= array_length; i++)
        heap_insert(heap, &position, array[i], no_ass, no_comp);
}

void heapsort(int* array, int* array_length) {
    int no_ass = 0, no_comp = 0;

    for (int i = *array_length; i >= 2; i--) {
        exchange(&array[1], &array[i]);
        *array_length -= 1;
        max_heapify(array, 1, *array_length, &no_ass, &no_comp);
    }
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

    int no_ass = 0, no_comp = 0;

    copy_array(array, copied_array, array_length);

    printf("Random array : \n");
    show_elements(array, array_length);
    printf("\n");

    printf("Heap bottom up : \n");
    build_bottom_up(array, array_length, &no_ass, &no_comp);
    show_elements(array, array_length);
    printf("\n");

    printf("Heap top down : \n");
    int heap[11];
    build_top_down(copied_array, heap, array_length, &no_ass, &no_comp);
    show_elements(heap, array_length);
    printf("\n");

    printf("Sorted array : \n");
    int array_length_copy = array_length;
    heapsort(array, &array_length_copy);
    show_elements(array, array_length);
}

void test_building_methods() {
    FILE* ass_file = fopen("assignments.csv", "w");
    FILE* comp_file = fopen("comparisons.csv", "w");
    FILE* sum_file = fopen("sums.csv", "w");

    int bottom_array[MAX_SIZE];
    int top_array[MAX_SIZE];

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        int bottom_no_ass = 0, bottom_no_comp = 0;
        int top_no_ass = 0, top_no_comp = 0;
        int bottom_sum, top_sum;

        for (int test = 0; test < NO_TESTS; test++) {
            generate_array(bottom_array, n);
            copy_array(bottom_array, top_array, n);

            int aux_array[MAX_SIZE];

            build_bottom_up(bottom_array, n, &bottom_no_ass, &bottom_no_comp);
            build_top_down(top_array, aux_array, n, &top_no_ass, &top_no_comp);
        }

        bottom_no_ass /= NO_TESTS;
        bottom_no_comp /= NO_TESTS;
        bottom_sum = bottom_no_ass + bottom_no_comp;

        top_no_ass /= NO_TESTS;
        top_no_comp /= NO_TESTS;
        top_sum = top_no_ass + top_no_comp;

        fprintf(ass_file, "%d,%d,%d\n", n, bottom_no_ass, top_no_ass);
        fprintf(comp_file, "%d,%d,%d\n", n, bottom_no_comp, top_no_comp);
        fprintf(sum_file, "%d,%d,%d\n", n, bottom_sum, top_sum);
    }

    fclose(ass_file);
    fclose(comp_file);
    fclose(sum_file);
}

int main() {
    test_functions();
    //test_building_methods();

    return 0;
}
