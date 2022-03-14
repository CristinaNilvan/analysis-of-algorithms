#include <stdio.h>
#include <malloc.h>

typedef struct multiway_tree {
    int value;
    int no_children;
    struct multiway_tree *children[];
} m_way;

typedef struct binary_tree {
    int value;
    struct binary_tree *first_child;
    struct binary_tree *next_brother;
} binary_tree;

void print_value_with_indent(int value, int indent) {
    for (int i = 0; i < indent; ++i) {
        printf(" ");
    }

    printf("%d\n", value);
}

void pretty_print_parent_array(const int *parent_array, int size, int parent, int indent) {
    for (int i = 0; i < size; ++i) {
        if (parent_array[i] == parent) {
            print_value_with_indent(i + 1, indent);

            pretty_print_parent_array(parent_array, size, i + 1, indent + 3);
        }
    }
}

m_way *create_m_way_node(int value) {
    m_way *node = (m_way *) malloc(sizeof(m_way));
    node->value = value;
    node->no_children = 0;

    return node;
}

void parents_to_m_way(const int *parent_array, int size, m_way *parent) {
    for (int i = 0; i < size; ++i) {
        if (parent_array[i] == parent->value) {
            m_way *new_child = create_m_way_node(i + 1);
            parent->children[parent->no_children] = new_child;

            parents_to_m_way(parent_array, size, parent->children[parent->no_children]);

            parent->no_children += 1;
        }
    }
}

void pretty_print_m_way_tree(m_way *parent, int indent) {
    for (int i = 0; i < parent->no_children; ++i) {
        print_value_with_indent(parent->children[i]->value, indent);

        pretty_print_m_way_tree(parent->children[i], indent + 3);
    }
}

binary_tree *create_b_tree_node(int value) {
    binary_tree *node = (binary_tree *) malloc(sizeof(binary_tree));
    node->value = value;
    node->first_child = NULL;
    node->next_brother = NULL;

    return node;
}

binary_tree *add_new_node(binary_tree *new_node, int value) {
    if (new_node == NULL)
        return NULL;

    if (new_node->first_child) {
        binary_tree *current_node = new_node->first_child;

        while (current_node->next_brother)
            current_node = current_node->next_brother;

        return (current_node->next_brother = create_b_tree_node(value));
    }
    else
        return (new_node->first_child = create_b_tree_node(value));
}

void m_way_to_b_tree(binary_tree *b_tree, m_way *parent) {
    for (int i = 0; i < parent->no_children; ++i) {
        binary_tree *new_node = add_new_node(b_tree, parent->children[i]->value);

        m_way_to_b_tree(new_node, parent->children[i]);
    }
}

void pretty_print_b_tree(binary_tree *b_tree, int indent) {
    if (b_tree == NULL)
        return;

    while (b_tree) {
        print_value_with_indent(b_tree->value, indent);

        if (b_tree->first_child)
            pretty_print_b_tree(b_tree->first_child, indent + 3);

        b_tree = b_tree->next_brother;
    }
}

int main() {
    int parent_representation[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    int size = 9;

    printf("Parent representation : \n");
    pretty_print_parent_array(parent_representation, size, -1, 0);
    printf("\n");

    m_way *m_way_tree = create_m_way_node(-1);
    parents_to_m_way(parent_representation, size, m_way_tree);

    printf("Multiway tree : \n");
    pretty_print_m_way_tree(m_way_tree, 0);
    printf("\n");

    binary_tree *b_tree = create_b_tree_node(-1);
    m_way_to_b_tree(b_tree, m_way_tree);

    printf("Binary tree : \n");
    pretty_print_b_tree(b_tree->first_child, 0);

    return 0;
}
