/* Nilvan Cristina, grupa 10
*
* Caz mediu statistic :
* - cea mai rapida metoda este Insertion Sort
* - cele mai putine atribuiri le face Selection Sort
* - cele mai putine comparatii le face Insertion Sort
*
* Caz favorabil :
* - cea mai rapida metoda este Bubble Sort
* - cele mai putine atribuiri le fac Bubble Sort si Selection Sort
* - cele mai putin comparatii le fac Bubble Sort si Insertion Sort
*
* Caz defavorabil :
* - cea mai rapida metoda este Selection Sort
* - cele mai putine atribuiri le face Selection Sort
* - comparatiile sunt la fel la fiecare dintre metode
* 
* Imbunatatiri :
* - ca o imbunatatire pentru Bubble Sort am renuntat la un for, introducand in schimb
* un do while cu o variabila care il conditioneaza. Astfel for-ul verifica valorile din
* sir doar daca a avut loc o schimbare reducand complexitatea de la O(n^2) la O(n) in caz favorabil.
*/

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NO_TESTS 5

Profiler profiler("Tema_1");

void copy_array(int* source_array, int* destination_array, int n) {
    for (int i = 0; i < n; i++) {
        destination_array[i] = source_array[i];
    }
}

void swap(int* first_elem, int* second_elem) {
    int aux = *first_elem;
    *first_elem = *second_elem;
    *second_elem = aux;
}

void bubble_sort(int* array, int n) {
    Operation no_assignments = profiler.createOperation("bubble-assignments", n);
    Operation no_comparisons = profiler.createOperation("bubble-comparisons", n);

    bool swapped;

    do {
        swapped = false;

        for (int i = 0; i < n - 1; i++) {
            no_comparisons.count();

            if (array[i] > array[i + 1]) {
                no_assignments.count(3);

                swap(&array[i], &array[i + 1]);
                swapped = true;
            }
        }
        n -= 1;
    } while (swapped);
}

void insertion_sort(int* array, int n) {
    Operation no_assignments = profiler.createOperation("insertion-assignments", n);
    Operation no_comparisons = profiler.createOperation("insertion-comparisons", n);

    int aux, position;

    for (int i = 1; i < n; ++i) {
        no_assignments.count();

        aux = array[i];
        position = i - 1;

        if (array[position] > aux) {
            while ((position >= 0) && (array[position] > aux)) {
                no_comparisons.count();
                no_assignments.count();

                array[position + 1] = array[position];
                position -= 1;
            }
        }
        else 
            no_comparisons.count();

        no_assignments.count();
        array[position + 1] = aux;
    }
}

void selection_sort(int* array, int n) {
    Operation no_assignments = profiler.createOperation("selection-assignments", n);
    Operation no_comparisons = profiler.createOperation("selection-comparisons", n);

    int posMin;

    for (int i = 0; i < n - 1; ++i) {
        posMin = i;

        for (int j = i + 1; j < n; ++j) {
            no_comparisons.count();

            if (array[posMin] > array[j])
                posMin = j;
        }

        if (posMin != i) {
            no_assignments.count(3);

            swap(&array[posMin], &array[i]);
        }
    }
}

void show_elements(int* array, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");
}

void test_sorting_basic_case() {
    int n = 10;
    int first_array[10] = { 21, 3, 2001, 13, 8, 2002, 19, 6, 1975, 10 };
    int second_array[10], third_array[10];

    copy_array(first_array, second_array, n);
    copy_array(first_array, third_array, n);

    printf("Array before sorting : \n");
    show_elements(first_array, n);

    printf("Array after bubble sort : \n");
    bubble_sort(first_array, n);
    show_elements(first_array, n);

    printf("Array after insertion sort : \n");
    insertion_sort(third_array, n);
    show_elements(third_array, n);

    printf("Array after selection sort : \n");
    selection_sort(second_array, n);
    show_elements(second_array, n);
}

void test_average_case() {
    int bubble_array[MAX_SIZE], insertion_array[MAX_SIZE], selection_array[MAX_SIZE];

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int test = 0; test < NO_TESTS; test++) {
            FillRandomArray(bubble_array, n, 10, 50000, true, UNSORTED);

            copy_array(bubble_array, insertion_array, n);
            copy_array(bubble_array, selection_array, n);

            bubble_sort(bubble_array, n);
            insertion_sort(insertion_array, n);
            selection_sort(selection_array, n);
        }
    }

    profiler.divideValues("bubble-assignments", NO_TESTS);
    profiler.divideValues("bubble-comparisons", NO_TESTS);
    profiler.addSeries("bubble-sum", "bubble-assignments", "bubble-comparisons");

    profiler.divideValues("insertion-assignments", NO_TESTS);
    profiler.divideValues("insertion-comparisons", NO_TESTS);
    profiler.addSeries("insertion-sum", "insertion-assignments", "insertion-comparisons");

    profiler.divideValues("selection-assignments", NO_TESTS);
    profiler.divideValues("selection-comparisons", NO_TESTS);
    profiler.addSeries("selection-sum", "selection-assignments", "selection-comparisons");

    profiler.createGroup("Assignments", "bubble-assignments", "insertion-assignments", "selection-assignments");
    profiler.createGroup("Comparisons", "bubble-comparisons", "insertion-comparisons", "selection-comparisons");
    profiler.createGroup("Sums", "bubble-sum", "insertion-sum", "selection-sum");
}

void test_best_case() {
    int bubble_array[MAX_SIZE], insertion_array[MAX_SIZE], selection_array[MAX_SIZE];

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        FillRandomArray(bubble_array, n, 10, 50000, true, ASCENDING);

        copy_array(bubble_array, insertion_array, n);
        copy_array(bubble_array, selection_array, n);

        bubble_sort(bubble_array, n);
        insertion_sort(insertion_array, n);
        selection_sort(selection_array, n);
    }

    profiler.addSeries("bubble-sum", "bubble-assignments", "bubble-comparisons");
    profiler.addSeries("insertion-sum", "insertion-assignments", "insertion-comparisons");
    profiler.addSeries("selection-sum", "selection-assignments", "selection-comparisons");

    profiler.createGroup("Assignments", "bubble-assignments", "insertion-assignments", "selection-assignments");
    profiler.createGroup("Comparisons", "bubble-comparisons", "insertion-comparisons", "selection-comparisons");
    profiler.createGroup("Sums", "bubble-sum", "insertion-sum", "selection-sum");
}

void test_worst_case() {
    int bubble_array[MAX_SIZE], insertion_array[MAX_SIZE], selection_array[MAX_SIZE];

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        FillRandomArray(bubble_array, n, 10, 50000, true, DESCENDING);
        copy_array(bubble_array, insertion_array, n);

        FillRandomArray(selection_array, n, 10, 50000, true, UNSORTED);
        selection_array[n - 1] = 1;  

        bubble_sort(bubble_array, n);
        insertion_sort(insertion_array, n);
        selection_sort(selection_array, n);
    }

    profiler.addSeries("bubble-sum", "bubble-assignments", "bubble-comparisons");
    profiler.addSeries("insertion-sum", "insertion-assignments", "insertion-comparisons");
    profiler.addSeries("selection-sum", "selection-assignments", "selection-comparisons");

    profiler.createGroup("Assignments", "bubble-assignments", "insertion-assignments", "selection-assignments");
    profiler.createGroup("Comparisons", "bubble-comparisons", "insertion-comparisons", "selection-comparisons");
    profiler.createGroup("Sums", "bubble-sum", "insertion-sum", "selection-sum");
}

void test_sorting() {
    test_average_case();

    profiler.reset("best-case");
    test_best_case();

    profiler.reset("worst-case");
    test_worst_case();

    profiler.showReport();
}

int main() {
    test_sorting_basic_case();
    //test_sorting();

    return 0;
}