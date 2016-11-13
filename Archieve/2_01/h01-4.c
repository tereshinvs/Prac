#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int prior;
    unsigned key;
    struct Node *left;
    struct Node *right;
} Node;

void
split(Node *v, unsigned key, Node **left, Node **right)
{
    if (!v) {
        *left = *right = NULL;
        return;
    }
    if (key < v->key) {
        split(v->left, key, left, &(v->left));
        *right = v;
    } else {
        split(v->right, key, &(v->right), right);
        *left = v;
    }
}

void
insert(Node **v, Node *t)
{
    if (!(*v)) {
        *v = t;
        return;
    }
    if (t->prior > (*v)->prior) {
        split(*v, t->key, &(t->left), &(t->right));
        *v = t;
    } else
        insert(t->key < (*v)->key ? &((*v)->left) : &((*v)->right), t);
}

void
erase_all(Node *v)
{
    if (!v)
        return;
    erase_all(v->left);
    erase_all(v->right);
    free(v);
}

void
print_all(Node *v)
{
    if (!v)
        return;
    print_all(v->right);
    printf("%u\n", v->key);
    print_all(v->left);
}

int
main(void)
{
    Node *tree = NULL, *temp;
    unsigned a;
    while (scanf("%u", &a) != EOF) {
        if (a) {
            temp = (Node *)calloc(1, sizeof(Node));
            temp->key = a;
            temp->prior = rand();
            temp->left = temp->right = NULL;
            insert(&tree, temp);
        }
    }
    print_all(tree);
    erase_all(tree);
    return 0;
}
