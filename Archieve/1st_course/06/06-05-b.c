#include <stdio.h>
#include <stdlib.h>

typedef struct node node;

struct node {
	int data, key, prior;
	node *left, *right;
};

void split(node *v, int key, node **left, node **right) {
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

void insert(node **v, node *t) {
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

node *search(node *v, int key) {
	while (v) {
		if (v->key == key)
			return v;
		v = key < v->key ? v->left : v->right;
	}
	return NULL;
}

void merge(node **v, node *left, node *right) {
	if (!left || !right) {
		*v = left ? left : right;
		return;
	}
	if (left->prior > right->prior) {
		merge(&(left->right), left->right, right);
		*v = left;
	} else {
		merge(&(right->left), left, right->left);
		*v = right;
	}
}

void erase(node **v, int key) {
	if (!(*v))
		return;
	if ((*v)->key == key) {
		node *temp = *v;
		merge(v, (*v)->left, (*v)->right);
		free(temp);
	}
	else
		erase(key < (*v)->key ? &((*v)->left) : &((*v)->right), key);
}

void erase_all(node *v) {
	if (!v)
		return;
	erase_all(v->left);
	erase_all(v->right);
	free(v);
}

int main(void) {
	node *tree = NULL, *temp;
	char op;
	int key, data;
	while (1) {
		scanf("%c", &op);
		switch (op) {
			case 'A':
				scanf("%d%d", &key, &data);
				temp = search(tree, key);
				if (temp) {
					temp->data = data;
					break;
				}
				temp = (node *)malloc(sizeof(node));
				temp->key = key;
				temp->data = data;
				temp->prior = rand();
				temp->left = temp->right = NULL;
				insert(&tree, temp);
				break;
			case 'S':
				scanf("%d", &key);
				temp = search(tree, key);
				if (temp)
					printf("%d %d\n", key, temp->data);
				break;
			case 'D':
				scanf("%d", &key);
				if (search(tree, key))
					erase(&tree, key);
				break;
			case 'F':
				erase_all(tree);
				return 0;
				break;
			default:
				break;
		}
		while (getchar() != '\n');
	}
	return 0;
}