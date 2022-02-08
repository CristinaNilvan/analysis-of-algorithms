#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 1000
#define STEP_SIZE 10

typedef struct k_list_element {
    int key;
    int list_index;
    struct k_list_element* next;
} list_element;

void initialize_array(list_element * array_of_lists[], int k)
{
    for(int i = 1; i <= k; i++)
        array_of_lists[i] = NULL;
}

list_element* create_new_node(int key, int list_index)
{
    list_element * new_node = (list_element *)malloc(sizeof(list_element));

    new_node->key = key;
    new_node->list_index = list_index;
    new_node->next = NULL;

    return new_node;
}

void insert_in_array_of_lists(list_element* array_of_lists[], int key, int index) {
    list_element* new_node = create_new_node(key, index);

    if (array_of_lists[index] == NULL)
        array_of_lists[index] = new_node;
    else {
        list_element* last_node = array_of_lists[index];

        while(last_node->next != NULL) {
            last_node = last_node->next;
        }

        last_node->next = new_node;
    }
}

void insert_last(list_element** head, int key, int index) {
    list_element* new_node = create_new_node(key, index);

    if (*head == NULL)
        *head = new_node;
    else {
        list_element* last_node = *head;

        while (last_node->next != NULL) {
            last_node = last_node->next;
        }

        last_node->next = new_node;
    }
}

int random(int min, int max) {
    return rand() % (max - min) + min;
}

void generate_random_list(int n, list_element* array_of_lists[], int i) {
    for (int j = 1; j <= n; ++j) {
        int new_key = random(1, 500000);
        insert_in_array_of_lists(array_of_lists, new_key, i);
    }
}

void redo_list_ascending(list_element* array_of_lists[], int i) {
    srand(time(NULL));

    list_element* current_node = array_of_lists[i];
    int increment = 10;

    while (current_node->next != NULL) {
        int next_key = current_node->next->key;

        if (current_node->key > current_node->next->key)
            while (next_key < current_node->key)
                next_key = random(1 + increment, 5000000 + increment);

        current_node->next->key = next_key;
        current_node = current_node->next;

        increment += 10;
    }
}

void exchange(list_element* first_elem, list_element* second_elem) {
    list_element aux = *first_elem;
    *first_elem = *second_elem;
    *second_elem = aux;
}

int left_child(int i) {
    return 2 * i;
}

int right_child(int i) {
    return 2 * i + 1;
}

void min_heapify(list_element* array[], int i, int heap_size, int* no_ass, int* no_comp) {
    int smallest;
    int left = left_child(i);
    int right = right_child(i);

    *no_comp += 1;
    if ((left <= heap_size) && (array[left]->key < array[i]->key))
        smallest = left;
    else
        smallest = i;

    *no_comp += 1;
    if ((right <= heap_size) && (array[right]->key < array[smallest]->key))
        smallest = right;

    if (smallest != i) {
        *no_ass += 3;
        exchange(array[i], array[smallest]);
        min_heapify(array, smallest, heap_size, no_ass, no_comp);
    }
}

void build_heap(list_element* array[], int array_length, int* no_ass, int* no_comp) {
    for (int i = array_length / 2; i >= 1; i--)
        min_heapify(array, i, array_length, no_ass, no_comp);
}

list_element* merge_lists(list_element* array[], int k, int* no_ass, int* no_comp) {
    list_element* list_out = NULL;

    int heap_size = k;
    build_heap(array, k, no_ass, no_comp);

    while (heap_size > 0) {
        *no_ass += 2;
        list_element* aux = array[1];
        array[1] = array[1]->next;

        insert_last(&list_out, aux->key, aux->list_index);

        *no_comp += 1;
        if (array[1] == NULL) {
            *no_ass += 1;
            array[1] = array[heap_size];

            heap_size -= 1;
        }

        min_heapify(array, 1, heap_size, no_ass, no_comp);
    }

    return list_out;
}

void print_list(list_element * node) {
    if (node == NULL) {
        printf("\n");
        return;
    }

    printf("%d ", node->key);
    print_list(node->next);
}

void show_array_of_lists(list_element* array_of_lists[], int k) {
    for (int index = 1; index <= k; index++) {
        printf("Array[%d] :", index);

        if (array_of_lists[index] != NULL) {
            list_element* current_node;
            current_node = array_of_lists[index];

            while (current_node != NULL) {
                printf(" %d ", current_node->key);
                current_node = current_node->next;
            }
        }
        printf("\n");
    }
}

void free_list(list_element* head) {
    list_element* auxiliary;

    while (head != NULL) {
        auxiliary = head;
        head = head->next;

        free(auxiliary);
    }
}

void free_array_of_lists(list_element* array_of_lists[], int k) {
    for (int i = 1; i <= k; i++)
        free_list(array_of_lists[i]);
}

void test_basic_case(int n, int k) {
    list_element* array_of_lists[k];
    initialize_array(array_of_lists, k);

    for (int i = 1; i <= k; ++i) {
        generate_random_list(n, array_of_lists, i);
        redo_list_ascending(array_of_lists, i);
    }

    int no_ass = 0, no_comp = 0;

    show_array_of_lists(array_of_lists, k);

    list_element* list_out = NULL;
    list_out = merge_lists(array_of_lists, k, &no_ass, &no_comp);
    print_list(list_out);

    free_array_of_lists(array_of_lists, k);
    free_list(list_out);
}

void test_k_constant() {
    FILE* k_constant_file = fopen("k_constant.csv", "w");

    int k_5 = 5, k_10 = 10, k_100 = 100;

    list_element* array_k_5[k_5];
    list_element* array_k_10[k_10];
    list_element* array_k_100[k_100];

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        list_element* l_out_5 = NULL;
        list_element* l_out_10 = NULL;
        list_element* l_out_100 = NULL;

        int k_5_ass = 0, k_5_comp = 0;
        int k_10_ass = 0, k_10_comp = 0;
        int k_100_ass = 0, k_100_comp = 0;
        int k_5_sum, k_10_sum, k_100_sum;

        initialize_array(array_k_5, k_5);
        initialize_array(array_k_10, k_10);
        initialize_array(array_k_100, k_100);

        for (int i = 1; i <= k_5; i++) {
            generate_random_list(n, array_k_5, i);
            redo_list_ascending(array_k_5, i);
        }

        l_out_5 = merge_lists(array_k_5, k_5, &k_5_ass, &k_5_comp);

        for (int i = 1; i <= k_10; i++) {
            generate_random_list(n, array_k_10, i);
            redo_list_ascending(array_k_10, i);
        }

        l_out_10 = merge_lists(array_k_10, k_10, &k_10_ass, &k_10_comp);

        for (int i = 1; i <= k_100; i++) {
            generate_random_list(n, array_k_100, i);
            redo_list_ascending(array_k_100, i);
        }

        l_out_100 = merge_lists(array_k_100, k_100, &k_100_ass, &k_100_comp);

        k_5_sum = k_5_ass + k_5_comp;
        k_10_sum = k_10_ass + k_10_comp;
        k_100_sum = k_100_ass + k_100_comp;

        fprintf(k_constant_file, "%d,%d,%d,%d\n", n, k_5_sum, k_10_sum, k_100_sum);

        free_array_of_lists(array_k_5, k_5);
        free_array_of_lists(array_k_10, k_10);
        free_array_of_lists(array_k_100, k_100);

        free_list(l_out_5);
        free_list(l_out_10);
        free_list(l_out_100);
    }

    fclose(k_constant_file);
}

void test_n_constant() {
    FILE* n_constant_file = fopen("n_constant.csv", "w");

    int n = 100;
    list_element* array_k[500];

    for (int k = 10; k <= 500; k += 10) {
        initialize_array(array_k, k);

        list_element* list_out = NULL;

        int no_ass = 0, no_comp = 0;
        int sum;

        for (int i = 1; i <= k; ++i) {
            generate_random_list(n, array_k, i);
            redo_list_ascending(array_k, i);
        }

        list_out = merge_lists(array_k, k, &no_ass, &no_comp);

        sum = no_ass + no_comp;

        fprintf(n_constant_file, "%d,%d\n", k, sum);

        free_array_of_lists(array_k, k);
        free_list(list_out);
    }

    fclose(n_constant_file);
}

int main() {
    test_basic_case(4, 3);
    //test_k_constant();
    //test_n_constant();

    return 0;
}
