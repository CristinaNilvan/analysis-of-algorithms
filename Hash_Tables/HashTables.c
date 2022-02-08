#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TABLE_SIZE 10007
#define NO_TESTS 2
#define MIN 1
#define MAX 100000

typedef struct table_cell {
    int key;
    int status;
} cell;

enum {
    FREE, OCCUPIED
};

void set_table_free(cell *hash_table, int table_size) {
    for (int i = 0; i < table_size; i++)
        hash_table[i].status = FREE;
}

int hash_function(int key, int table_size, int i) {
    return (key + 10 * i + 5 * i * i) % table_size;
}

void insert_key(cell *hash_table, int value, int table_size) {
    for (int i = 0; i < table_size; i++) {
        int position = hash_function(value, table_size, i);

        if (hash_table[position].status == FREE) {
            hash_table[position].key = value;
            hash_table[position].status = OCCUPIED;
            return;
        }
    }
}

int search_key(cell *hash_table, int value, int table_size, int *effort) {
    for (int i = 0; i < table_size; i++) {
        int position = hash_function(value, table_size, i);
        *effort += 1;

        if (hash_table[position].status == FREE)
            return -1;
        else if (hash_table[position].key == value)
            return position;
    }

    return -1;
}

int random(int min, int max) {
    return rand() % (max - min) + min;
}

void generate_unique_array(int *array, int n, int *min, int *max) {
    srand(time(NULL));

    int increment = 10;
    array[0] = random(*min, *max);

    for (int i = 1; i < n; i++) {
        array[i] = random(*min + increment, *max + increment);

        for (int j = 1; j < i; ++j)
            if (array[i] == array[j]) {
                increment += 10;
                array[i] = random(*min + increment, *max + increment);
            }
    }

    *min += increment;
    *max += increment;
}

void print_table(cell *hash_table, int table_size) {
    printf("The hash table is : \n");

    for (int i = 0; i < table_size; i++) {
        if (hash_table[i].status == OCCUPIED)
            printf("hash_table[%d] = %d \n", i, hash_table[i].key);
        else
            printf("hash_table[%d] = -- \n", i);
    }
}

void test_basic_case() {
    int size = 15;
    cell *hash_table = (cell *) malloc(TABLE_SIZE * sizeof(cell));

    set_table_free(hash_table, size);

    //0.95 => 14.25 => 14
    int values_found[] = {19, 36, 5, 21, 4, 26, 14, 17, 40, 12, 3, 57, 27, 43};
    int values_not_found[] = {77, 85, 30, 63};

    for (int i = 0; i < sizeof(values_found) / sizeof(int) - 1; i++)
        insert_key(hash_table, values_found[i], size);

    print_table(hash_table, size);
    printf("\n");

    for (int i = 4; i < 8; i++) {
        int effort = 0;
        int searched = search_key(hash_table, values_found[i], size, &effort);

        if (searched != -1)
            printf("Elementul %d a fost gasit in tabela cu efort : %d.\n", values_found[i], effort);
    }

    printf("\n");

    for (int i = 0; i < 4; i++) {
        int effort = 0;
        int searched = search_key(hash_table, values_not_found[i], size, &effort);

        if (searched == -1)
            printf("Elementul %d nu a fost gasit in tabela cu efort : %d.\n", values_not_found[i], effort);
    }

    free(hash_table);
}

/* N = 10007
 * 0.8 => 8005.6 => 8006
 * 0.85 => 8505.95 => 8506
 * 0.9 => 9006.3 => 9007
 * 0.95 => 9506.65 => 9507
 * 0.99 => 9906.93 => 9907
 * */

void test() {
    FILE *table_file = fopen("table.csv", "w");

    int values[5] = {8006, 8506, 9007, 9507, 9907};

    for (int i = 0; i < 5; ++i) {
        cell *hash_table = (cell *) malloc(TABLE_SIZE * sizeof(cell));
        set_table_free(hash_table, TABLE_SIZE);

        int no_elements = values[i];

        int found_elements[10007];
        int not_found_elements[10007];

        int medium_effort_found, medium_effort_not_found;
        int max_effort_found = 0, max_effort_not_found  = 0;
        int med_effort_final = 0, med_effort_not_final = 0;

        for (int tests = 0; tests < NO_TESTS; tests++) {
            int eff_sum_found = 0, eff_sum_not_found = 0;

            int min_f = MIN;
            int max_f = MAX;
            generate_unique_array(found_elements, no_elements, &min_f, &max_f);

            int min_nf = max_f;
            int max_nf = 800000;
            generate_unique_array(not_found_elements, no_elements, &min_nf, &max_nf);

            for (int j = 0; j < no_elements; ++j) {
                insert_key(hash_table, found_elements[j], TABLE_SIZE);
            }

            for (int j = 0; j < 3000; ++j) {
                int effort;

                if (j % 2 == 0) {
                    effort = 0;
                    int searched = search_key(hash_table, found_elements[random(0, no_elements)], TABLE_SIZE, &effort);

                    if (searched != -1) {
                        if (effort > max_effort_found)
                            max_effort_found = effort;

                        eff_sum_found += effort;
                    }
                } else {
                    effort = 0;
                    int searched = search_key(hash_table, not_found_elements[random(0, no_elements)], TABLE_SIZE, &effort);

                    if (searched == -1) {
                        if (effort > max_effort_not_found)
                            max_effort_not_found = effort;

                        eff_sum_not_found += effort;
                    }
                }
            }

            medium_effort_found = eff_sum_found / 1500;
            med_effort_final += medium_effort_found;

            medium_effort_not_found = eff_sum_not_found / 1500;
            med_effort_not_final += medium_effort_not_found;
        }

        med_effort_final /= NO_TESTS;
        med_effort_not_final /= NO_TESTS;

        fprintf(table_file, "%d,%d,%d,%d,%d\n", no_elements, med_effort_final, max_effort_found,
                med_effort_not_final, max_effort_not_found);

        free(hash_table);
    }
}

int main() {
    test_basic_case();
    //test();

    return 0;
}
