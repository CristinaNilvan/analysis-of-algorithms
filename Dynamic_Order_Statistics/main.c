#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 3
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NO_TESTS 5

typedef struct tree_node {
    int key;
    int size;
    struct tree_node *parent;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

typedef struct pbt_tree {
    struct tree_node *root;
} pbt_tree;

void print_value_with_indent(int value, int indent) {
    for (int i = 0; i < indent; ++i) {
        printf(" ");
    }

    printf("%d\n", value);
}

int random(int min, int max) {
    return rand() % (max - min) + min;
}

tree_node *create_node(int key, tree_node *given_parent) {
    tree_node *new_node = (tree_node *) malloc(sizeof(tree_node));

    new_node->key = key;
    new_node->size = 0;
    new_node->parent = given_parent;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

int get_size(tree_node *node) {
    if (node == NULL)
        return 0;
    else
        return (get_size(node->left) + get_size(node->right) + 1);
}

tree_node *build_tree(int *ordered_array, int left, int right, tree_node *parent) {
    if (left > right)
        return NULL;

    int mid = (left + right) / 2;
    tree_node *node = create_node(ordered_array[mid], parent);

    node->left = build_tree(ordered_array, left, mid - 1, node);
    node->right = build_tree(ordered_array, mid + 1, right, node);

    node->size = get_size(node);

    return node;
}

int get_rank(const tree_node *node) { return node->left ? node->left->size + 1 : 1; }

tree_node *os_select(tree_node *node, int searched_index, int *no_comp) {
    if (node == NULL)
        return NULL;

    int rank = get_rank(node);

    if (++(*no_comp) && (searched_index == rank))
        return node;
    else if (++(*no_comp) && (searched_index < rank))
        return os_select(node->left, searched_index, no_comp);
    else
        return os_select(node->right, searched_index - rank, no_comp);
}

void transplant(pbt_tree *tree, tree_node *replaced_node, tree_node *new_node, int *no_comp, int *no_ass) {
    if (++(*no_comp) && (replaced_node->parent == NULL))
        tree->root = new_node;
    else if (++(*no_comp) && (replaced_node == replaced_node->parent->left))
        replaced_node->parent->left = new_node;
    else
        replaced_node->parent->right = new_node;

    *no_ass += 1;

    if (++(*no_comp) && (new_node != NULL)) {
        *no_ass += 1;
        new_node->parent = replaced_node->parent;
    }
}

tree_node *tree_minimum(tree_node *node, int *no_comp, int *no_ass) {
    while (++(*no_comp) && (node->left != NULL)) {
        *no_ass += 1;
        node = node->left;
    }

    return node;
}

void update_size(tree_node *node) {
    while (node != NULL) {
        node->size -= 1;
        node = node->parent;
    }
}

void os_delete(pbt_tree *tree, tree_node *node, int *no_comp, int *no_ass) {
    if (node == NULL)
        return;

    if (node->left == NULL)
        transplant(tree, node, node->right, no_comp, no_ass);
    else if (node->right == NULL)
        transplant(tree, node, node->left, no_comp, no_ass);
    else {
        tree_node *minimum = tree_minimum(node->right, no_comp, no_ass);
        *no_ass += 1;

        if ((++(*no_comp) && minimum->parent != node)) {
            transplant(tree, minimum, minimum->right, no_comp, no_ass);
            minimum->right = node->right;
            minimum->right->parent = minimum;

            *no_ass += 2;
        }

        transplant(tree, node, minimum, no_comp, no_ass);
        minimum->left = node->left;
        minimum->left->parent = minimum;

        *no_ass += 2;
    }

    update_size(node);
}

void print_preorder(tree_node *node) {
    if (node == NULL)
        return;

    printf("%d ", node->key);

    print_preorder(node->left);
    print_preorder(node->right);
}

void pretty_print(tree_node *node, int space) {
    if (node == NULL)
        return;

    space += COUNT;

    pretty_print(node->right, space);

    printf("\n");
    print_value_with_indent(node->key, space);

    pretty_print(node->left, space);
}

void free_tree(tree_node *node) {
    if (node == NULL)
        return;

    free_tree(node->left);
    free_tree(node->right);
}

void test_functions() {
    int ordered_array[] = {0, 1, 7, 10, 12, 16, 17, 19, 21, 23, 25};
    int array_length = 10;

    pbt_tree tree;
    tree.root = build_tree(ordered_array, 1, array_length, NULL);

    printf("The tree is : \n");
    pretty_print(tree.root, 0);
    printf("Preorder : \n");
    print_preorder(tree.root);
    printf("\n");


    for (int i = 0; i < 3; ++i) {
        int no_comp = 0, no_ass = 0;
        int index = random(1, 10);

        tree_node *to_delete = os_select(tree.root, index, &no_comp);
        printf("The %d smallest element is %d.\n", index, os_select(tree.root, index, &no_comp)->key);
        os_delete(&tree, to_delete, &no_comp, &no_ass);
    }

    printf("The new tree is : \n");
    pretty_print(tree.root, 0);
    printf("Preorder : \n");
    print_preorder(tree.root);
    printf("\n");

    free_tree(tree.root);
}

void test() {
    FILE *sum_file = fopen("sum.csv", "w");

    pbt_tree tree;

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        int no_comp = 0, no_ass = 0, sum;
        int array[10000];

        for (int i = 1; i <= n; ++i) {
            array[i] = i;
        }

        for (int tests = 0; tests < NO_TESTS; ++tests) {
            tree.root = build_tree(array, 1, n, NULL);

            for (int i = 1; i <= n; ++i) {
                int index = random(1, n);

                tree_node *to_delete = os_select(tree.root, index, &no_comp);
                os_delete(&tree, to_delete, &no_comp, &no_ass);
            }

            free_tree(tree.root);
        }

        no_comp /= NO_TESTS;
        no_ass /= NO_TESTS;
        sum = no_ass + no_comp;

        fprintf(sum_file, "%d,%d\n", n, sum);
    }

    fclose(sum_file);
}

int main() {
    srand(time(NULL));
    test_functions();
    //test();

    return 0;
}
