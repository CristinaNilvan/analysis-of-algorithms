#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NO_TESTS 5

typedef struct graph_node_struct {
    int key;
    struct graph_node_struct *next;
} graph_node;

typedef struct {
    int number_of_nodes;
    graph_node **adjacency_lists;
    int *parent_nodes;
    int *discover_time;
    int *finish_time;
    int *color_of_nodes;
} Graph;

typedef struct topological_struct {
    int key;
    int finish_time;
} topological_node;

typedef struct tarjan_struct {
    int key;
    int index;
    int low_link;
    int comp;
    bool on_stack;
} tarjan_node;

typedef struct operation {
    int no_ass;
    int no_comp;
} operation;

enum {
    WHITE, GRAY, BLACK
};

graph_node *create_node(int key) {

    graph_node *node = (graph_node *) malloc(sizeof(graph_node));

    node->key = key;
    node->next = NULL;

    return node;
}

void allocate_memory(Graph *graph) {
    graph->adjacency_lists = (graph_node **) calloc(graph->number_of_nodes, sizeof(graph_node *));
    graph->parent_nodes = (int *) calloc(graph->number_of_nodes, sizeof(int));
    graph->discover_time = (int *) calloc(graph->number_of_nodes, sizeof(int));
    graph->finish_time = (int *) calloc(graph->number_of_nodes, sizeof(int));
    graph->color_of_nodes = (int *) calloc(graph->number_of_nodes, sizeof(int));
}

void reinitialize_graph(Graph *graph) {
    for (int i = 0; i < graph->number_of_nodes; i++) {
        graph->parent_nodes[i] = -1;
        graph->discover_time[i] = -1;
        graph->finish_time[i] = -1;
        graph->color_of_nodes[i] = WHITE;
    }
}

bool search_node(Graph graph, int first_vertex, int second_vertex) {
    bool found = false;
    graph_node *current_node = graph.adjacency_lists[first_vertex];

    while (current_node != NULL) {
        if (current_node->key == second_vertex) {
            found = true;
            break;
        }

        current_node = current_node->next;
    }

    return found;
}

void insert_in_adjacency_list(Graph *graph, int first_vertex, int second_vertex) {
    graph_node *to_insert = create_node(second_vertex);

    if (graph->adjacency_lists[first_vertex] == NULL)
        graph->adjacency_lists[first_vertex] = to_insert;
    else {
        graph_node *current_node = graph->adjacency_lists[first_vertex];

        while (current_node->next != NULL)
            current_node = current_node->next;

        current_node->next = to_insert;
    }
}

void initialize_adjacency_lists(Graph *graph) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        graph->adjacency_lists[i] = NULL;
}

void initialize_parent_nodes(Graph *graph) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        graph->parent_nodes[i] = -1;
}

void depth_first_search_visit(Graph *graph, int visited_node, int *time, operation *no_operations) {
    *time += 1;
    graph->discover_time[visited_node] = *time;
    graph->color_of_nodes[visited_node] = GRAY;
    no_operations->no_ass += 2;

    graph_node *current_node = graph->adjacency_lists[visited_node];

    while (current_node != NULL) {
        int neighbour_node = current_node->key;
        no_operations->no_ass += 1;

        no_operations->no_comp += 1;
        if (graph->color_of_nodes[neighbour_node] == WHITE) {
            graph->parent_nodes[neighbour_node] = visited_node;
            no_operations->no_ass += 1;

            depth_first_search_visit(graph, neighbour_node, time, no_operations);
        }

        no_operations->no_ass += 1;
        current_node = current_node->next;
    }

    graph->color_of_nodes[visited_node] = BLACK;
    *time = *time + 1;
    graph->finish_time[visited_node] = *time;
    no_operations->no_ass += 2;
}

void depth_first_search(Graph *graph, operation *no_operations) {
    int time = 0;

    for (int i = 0; i < graph->number_of_nodes; ++i) {
        no_operations->no_comp += 1;
        if (graph->color_of_nodes[i] == WHITE)
            depth_first_search_visit(graph, i, &time, no_operations);
    }
}

void swap(topological_node *first_elem, topological_node *second_elem) {
    topological_node aux = *first_elem;
    *first_elem = *second_elem;
    *second_elem = aux;
}

void bubble_sort(topological_node *array, int n) {
    bool swapped;

    do {
        swapped = false;

        for (int i = 0; i < n - 1; i++) {
            if (array[i].finish_time < array[i + 1].finish_time) {
                swap(&array[i], &array[i + 1]);
                swapped = true;
            }
        }
        n -= 1;
    } while (swapped);
}

void push_int(int *stack, int *no_elements, int new_element) {
    stack[*no_elements] = new_element;
    *no_elements += 1;
}

int *topological_sort(Graph *graph, int *current_in_list) {
    operation no_operations;
    no_operations.no_ass = 0;
    no_operations.no_comp = 0;
    depth_first_search(graph, &no_operations);

    topological_node *values_array = (topological_node *) calloc(graph->number_of_nodes, sizeof(topological_node));

    for (int i = 0; i < graph->number_of_nodes; ++i) {
        values_array[i].key = i;
        values_array[i].finish_time = graph->finish_time[i];
    }

    bubble_sort(values_array, graph->number_of_nodes);

    int *list = (int *) calloc(graph->number_of_nodes, sizeof(int));

    for (int i = 0; i < graph->number_of_nodes; ++i) {
        push_int(list, current_in_list, values_array[i].key);
    }

    return list;
}

void print_array(int *array, int number) {
    for (int i = 0; i < number; i++)
        printf("%d ", array[i]);

    printf("\n");
}

void print_graph(Graph graph) {
    for (int i = 0; i < graph.number_of_nodes; ++i) {
        printf("%d : ", i);
        graph_node *current_node = graph.adjacency_lists[i];

        while (current_node != NULL) {
            printf("%d ", current_node->key);
            current_node = current_node->next;
        }

        printf("\n");
    }

    printf("\n");
}

void push(tarjan_node **stack, int *no_elements, tarjan_node *new_element) {
    stack[*no_elements] = new_element;
    *no_elements += 1;
}

tarjan_node *pop(tarjan_node **stack, int *no_elements) {
    tarjan_node *element = stack[*no_elements];
    *no_elements -= 1;

    return element;
}

tarjan_node *create_tarjan_node(int key) {
    tarjan_node *node = (tarjan_node *) malloc(sizeof(tarjan_node));

    node->key = key;
    node->index = -1;
    node->low_link = -1;
    node->comp = 0;
    node->on_stack = false;

    return node;
}

int min(int first, int second) {
    if (first < second)
        return first;

    return second;
}

void strong_connect(Graph graph, tarjan_node **tarjan_nodes, tarjan_node *current_node, int *index, int visited_node,
                    int *no_components) {
    current_node->index = *index;
    current_node->low_link = *index;
    *index += 1;

    tarjan_node **stack = (tarjan_node **) calloc(graph.number_of_nodes, sizeof(tarjan_node *));
    int elements_on_stack = 0;

    push(stack, &elements_on_stack, current_node);
    current_node->on_stack =  true;

    graph_node *node_in_graph = graph.adjacency_lists[visited_node];

    while (node_in_graph != NULL) {
        int neighbour_in_graph = node_in_graph->key;
        tarjan_node *tarjan_neigh_node = tarjan_nodes[neighbour_in_graph];

        if (tarjan_neigh_node->index == -1) {
            strong_connect(graph, tarjan_nodes, tarjan_neigh_node, index, neighbour_in_graph, no_components);
            current_node->low_link = min(current_node->low_link, tarjan_neigh_node->low_link);
        } else if (tarjan_neigh_node->on_stack) {
            current_node->low_link = min(current_node->low_link, tarjan_neigh_node->index);
        }

        node_in_graph = node_in_graph->next;
    }

    if (current_node->low_link == current_node->index) {
        *no_components += 1;

        tarjan_node *aux = pop(stack, &elements_on_stack);

        while (aux != current_node) {
            aux = pop(stack, &elements_on_stack);
            aux->on_stack = false;
            aux->comp = *no_components;
        }
    }
}

void tarjan(Graph graph) {
    int index = 0;
    int no_components = 0;

    tarjan_node **tarjan_nodes = (tarjan_node **) calloc(graph.number_of_nodes, sizeof(tarjan_node *));

    for (int i = 0; i < graph.number_of_nodes; ++i)
        tarjan_nodes[i] = create_tarjan_node(i);

    for (int i = 0; i < graph.number_of_nodes; ++i)
        if (tarjan_nodes[i]->index == -1)
            strong_connect(graph, tarjan_nodes, tarjan_nodes[i], &index, i, &no_components);

    printf("Number of components : %d.\n", no_components);

    for (int j = 0; j < no_components; ++j) {
        for (int i = 0; i < graph.number_of_nodes; ++i) {
            if (tarjan_nodes[i]->comp == j)
                printf("%d ", tarjan_nodes[i]->key);
        }

        printf("\n");
    }
}

int random(int min, int max) {
    return rand() % (max - min) + min;
}

Graph generate_random_graph(int vertices, int edges) {
    Graph graph;
    graph.number_of_nodes = vertices;
    allocate_memory(&graph);

    initialize_adjacency_lists(&graph);
    initialize_parent_nodes(&graph);

    for (int i = 0; i < edges; ++i) {
        int first = random(0, vertices);
        int second = random(0, vertices);

        while (search_node(graph, first, second))
            second = random(0, vertices);

        insert_in_adjacency_list(&graph, first, second);
    }

    return graph;
}

void free_adjacency_lists(Graph *graph) {
    for (int i = 0; i < (*graph).number_of_nodes; i++)
        while ((*graph).adjacency_lists[i] != NULL) {
            graph_node *to_be_freed = (*graph).adjacency_lists[i];
            (*graph).adjacency_lists[i] = (*graph).adjacency_lists[i]->next;

            free(to_be_freed);
        }

    free((*graph).adjacency_lists);
}

void free_memory(Graph *graph) {
    free((*graph).parent_nodes);
    free((*graph).discover_time);
    free((*graph).finish_time);
    free((*graph).color_of_nodes);
    free_adjacency_lists(graph);
}

void test_functions() {
    Graph graph;

    graph.number_of_nodes = 4;
    allocate_memory(&graph);

    initialize_adjacency_lists(&graph);
    initialize_parent_nodes(&graph);

    insert_in_adjacency_list(&graph, 0, 1);
    insert_in_adjacency_list(&graph, 0, 3);
    insert_in_adjacency_list(&graph, 1, 2);
    insert_in_adjacency_list(&graph, 1, 3);
    insert_in_adjacency_list(&graph, 2, 3);
    insert_in_adjacency_list(&graph, 3, 0);

    print_graph(graph);

    operation no_operations;
    no_operations.no_ass = 0;
    no_operations.no_comp = 0;

    printf("Depth First Search\n");
    depth_first_search(&graph, &no_operations);
    print_array(graph.parent_nodes, graph.number_of_nodes);

    printf("Tarjan\n");
    tarjan(graph);

    reinitialize_graph(&graph);

    printf("Topological sort\n");
    int current_in_list = 0;
    int *topological_list = topological_sort(&graph, &current_in_list);
    print_array(topological_list, current_in_list);
    printf("\n");

    free_memory(&graph);
}

void test_constant_vertices() {
    FILE *file = fopen("constant_v.csv", "w");

    Graph graph;

    for (int edges = 1000; edges <= 4500; edges += 100) {
        operation no_operations;

        no_operations.no_ass = 0;
        no_operations.no_comp = 0;

        for (int test = 0; test < NO_TESTS; ++test) {
            graph = generate_random_graph(100, edges);
            depth_first_search(&graph, &no_operations);

            free_memory(&graph);
        }

        int sum = no_operations.no_ass / NO_TESTS + no_operations.no_comp / NO_TESTS;

        fprintf(file, "%d,%d\n", edges, sum);
    }

    fclose(file);
}

void test_constant_edges() {
    FILE *file = fopen("constant_e.csv", "w");

    Graph graph;

    for (int vertices = 100; vertices <= 200; vertices += 10) {
        operation no_operations;

        no_operations.no_ass = 0;
        no_operations.no_comp = 0;

        for (int test = 0; test < NO_TESTS; ++test) {
            graph = generate_random_graph(vertices, 4500);
            depth_first_search(&graph, &no_operations);

            free_memory(&graph);
        }

        int sum = no_operations.no_ass / NO_TESTS + no_operations.no_comp / NO_TESTS;

        fprintf(file, "%d,%d\n", vertices, sum);
    }

    fclose(file);
}

int main() {
    srand(time(NULL));
    test_functions();
    //test_constant_vertices();
    //test_constant_edges();

    return 0;
}
