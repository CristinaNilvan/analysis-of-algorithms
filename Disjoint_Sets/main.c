#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STEP_SIZE 10
#define MAX_SIZE 400
#define NO_TESTS 5

typedef struct disjoint_set {
    int key;
    int rank;
    struct disjoint_set *parent;
} ds_node;

typedef struct edge_struct {
    int first_vertex;
    int second_vertex;
    int weight;
} edge;

ds_node *create_node(int key) {
    ds_node *new_node = (ds_node *) malloc(sizeof(ds_node));

    new_node->key = key;
    new_node->parent = NULL;

    return new_node;
}

void make_set(ds_node *new_set, int *no_ass) {
    new_set->rank = 0;
    new_set->parent = new_set;

    *no_ass += 2;
}

void link(ds_node *first_repr, ds_node *second_repr, int *no_ass, int *no_comp) {
    if (++(*no_comp) && (first_repr->rank > second_repr->rank)) {
        second_repr->parent = first_repr;
        *no_ass += 1;
    }
    else {
        first_repr->parent = second_repr;
        *no_ass += 1;

        if (++(*no_comp) && (first_repr->rank == second_repr->rank)) {
            second_repr->rank = second_repr->rank + 1;
            *no_ass += 1;
        }
    }
}

ds_node *find_set(ds_node *rep, int *no_ass, int *no_comp) {
    if (++(*no_comp) && (rep != rep->parent)) {
        rep->parent = find_set(rep->parent, no_ass, no_comp);
        *no_ass += 1;
    }

    return rep->parent;
}

void union_sets(ds_node *first_repr, ds_node *second_repr, int *no_ass, int *no_comp) {
    link(find_set(first_repr, no_ass, no_comp), find_set(second_repr, no_ass, no_comp), no_ass, no_comp);
}

void exchange(edge* first_elem, edge* second_elem) {
    edge aux = *first_elem;
    *first_elem = *second_elem;
    *second_elem = aux;
}

void sort_edges(edge *edges, int no_edges) {
    for (int i = 0; i < no_edges; ++i) {
        for (int j = 0; j < no_edges; ++j) {
            if (edges[j].weight > edges[i].weight)
                exchange(&edges[i], &edges[j]);
        }
    }
}

int **allocate_memory(int length) {
    int **matrix = (int **)malloc(length * sizeof(int*));
    for(int i = 0; i < length; i++)
        matrix[i] = (int *)malloc(length * sizeof(int));

    return matrix;
}

void initialize_matrix(int **matrix, int length) {
    for (int i = 0; i < length; ++i)
        for (int j = 0; j < length; ++j) {
             matrix[i][j] = 0;
        }
}

int **kruskal(int **graph, int graph_length, int *no_ass, int *no_comp) {
    ds_node *nodes[graph_length];

    int **mst = allocate_memory(graph_length);
    initialize_matrix(mst, graph_length);

    for (int i = 0; i < graph_length; ++i) {
        ds_node *new_node = create_node(i);
        nodes[i] = new_node;
        make_set(nodes[i], no_ass);
    }

    edge edges[graph_length * graph_length];
    int no_edges = 0;
    for (int i = 0; i < graph_length; ++i) {
        for (int j = 0; j < graph_length; ++j) {
            if (i < j) {
                edges[no_edges].first_vertex = i;
                edges[no_edges].second_vertex = j;
                edges[no_edges].weight = graph[i][j];
                no_edges += 1;
            }
        }
    }

    sort_edges(edges, no_edges);

    for (int i = 0; i < no_edges; ++i) {
        int first = edges[i].first_vertex;
        int second = edges[i].second_vertex;

        ds_node *first_n = nodes[first];
        ds_node *second_n = nodes[second];

        if (find_set(first_n, no_ass, no_comp) != find_set(second_n, no_ass, no_comp)) {
            mst[first][second] = edges[i].weight;
            mst[second][first] = edges[i].weight;
            union_sets(first_n, second_n, no_ass, no_comp);
        }
    }

    return mst;
}

void print_matrix(int **matrix, int length) {
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
            printf("%d  ", matrix[i][j]);
        }

        printf("\n");
    }
}

void free_memory(int **matrix, int length) {
    for(int i = 0; i < length; ++i)
        free(matrix[i]);

    free(matrix);
}

void test_ds_functions() {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ds_node *nodes[5];

    int no_ass = 0, no_comp = 0;

    for (int i = 0; i < 10; ++i) {
        ds_node *new_node = create_node(array[i]);
        nodes[i] = new_node;
        make_set(nodes[i], &no_ass);
    }

    for (int i = 0; i < 9; ++i) {
        if (i % 2 == 0)
            union_sets(nodes[i], nodes[i + 1], &no_ass, &no_comp);
    }

    for (int i = 0; i < 10; ++i) {
        printf("Node %d with parent %d.\n", nodes[i]->key, nodes[i]->parent->key);
    }

    for (int i = 0; i < 5; ++i) {
        if (find_set(nodes[i], &no_ass, &no_comp))
            printf("Node with key %d was found.\n", nodes[i]->key);
    }
    printf("\n");
}

void test_kruskal_algo() {
    int graph_length = 4;
    int **graph = allocate_memory(graph_length);

    graph[0][0] = 0;
    graph[0][1] = 5;
    graph[0][2] = 3;
    graph[0][3] = 2;

    graph[1][0] = 5;
    graph[1][1] = 0;
    graph[1][2] = 7;
    graph[1][3] = 1;

    graph[2][0] = 3;
    graph[2][1] = 7;
    graph[2][2] = 0;
    graph[2][3] = 4;

    graph[3][0] = 2;
    graph[3][1] = 1;
    graph[3][2] = 4;
    graph[3][3] = 0;

    printf("The initial graph : \n");
    print_matrix(graph, graph_length);

    int no_ass = 0, no_comp = 0;
    printf("The minimum spanning tree : \n");
    int **mst = kruskal(graph, graph_length, &no_ass, &no_comp);
    print_matrix(mst, graph_length);
    printf("\n");

    free_memory(graph, graph_length);
    free_memory(mst, graph_length);
}

int random(int min, int max) {
    return rand() % (max - min) + min;
}

int **generate(int n) {
    int **graph = allocate_memory(n);
    initialize_matrix(graph, n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i < j) {
                graph[i][j] = random(1, n);
                graph[j][i] = graph[i][j];
            }
        }
    }

    return graph;
}

void test() {
    FILE *sum_file = fopen("sum.csv", "w");

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        int no_ass = 0, no_comp = 0;
        int sum;

        for (int test = 0; test < NO_TESTS; ++test) {
            int **graph = generate(n);
            int **mst = kruskal(graph, n, &no_ass, &no_comp);

            free_memory(graph, n);
            free_memory(mst, n);
        }

        no_ass /= NO_TESTS;
        no_comp /= NO_TESTS;
        sum = no_ass + no_comp;

        fprintf(sum_file, "%d,%d\n", n, sum);
    }

    fclose(sum_file);
}

int main() {
    srand(time(NULL));
    test_ds_functions();
    test_kruskal_algo();
    //test();

    return 0;
}
