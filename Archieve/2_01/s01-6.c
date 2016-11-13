enum
{
    STACK_SIZE = 8000
};

void
processTree(struct node *pnode, void *func)
{
    if (!pnode)
        return;

    struct node *node_stack[STACK_SIZE];
    char left_alr[STACK_SIZE];
    int st_counter = 0;

    node_stack[0] = pnode;
    left_alr[0] = 0;
    st_counter = 1;

    while (st_counter) {
        if (!node_stack[st_counter - 1]) {
            st_counter--;
            continue;
        }
        if (left_alr[st_counter - 1]) {
            ((void (*)(int))func)(node_stack[st_counter - 1]->key);
            node_stack[st_counter - 1] = node_stack[st_counter - 1]->right;
            left_alr[st_counter - 1] = 0;   
        } else {
            left_alr[st_counter - 1] = 1;
            node_stack[st_counter] = node_stack[st_counter - 1]->left;
            left_alr[st_counter] = 0;
            st_counter++;
        }
    }
}