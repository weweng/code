#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"

#define MAX_TREE_NODE 100

typedef struct Node_ {
    struct Node_ *lnode;
    int data;
    struct Node_ *rnode;
} Node;

Node* createTreeNode(int val) {
    Node * node = calloc(1, sizeof(Node));
    if (node) node->data = val;

    return node;
}

int RcountTree_sum(Node *t) {
    int x, y;
    if(t!=NULL) {
        x=RcountTree_sum(t->lnode);
        y=RcountTree_sum(t->rnode);
        return x+y+t->data;         
    }
    return 0;
}

int RcountTree_node(Node *t) {
    int x, y;
    if(t!=NULL) {
        x=RcountTree_node(t->lnode);
        y=RcountTree_node(t->rnode);
        return x+y+1;         
    }
    return 0;
}

int RcountTree_height(Node *t) {
    int x, y;
    if(t!=NULL) {
        x=RcountTree_height(t->lnode);
        y=RcountTree_height(t->rnode);
        return x>y? x+1:y+1;         
    }
    return 0;
}


void RDisplayTree_preorder(Node *t) {
    if (t == NULL) return;

    printf("%d,", t->data);
    RDisplayTree_preorder(t->lnode);
    RDisplayTree_preorder(t->rnode);
}

void RDisplayTree_inorder(Node *t) {
    if (t == NULL) return;

    RDisplayTree_inorder(t->lnode);
    printf("%d,", t->data);
    RDisplayTree_inorder(t->rnode);
}

void RDisplayTree_postorder(Node *t) {
    if (t == NULL) return;

    RDisplayTree_postorder(t->lnode);
    RDisplayTree_postorder(t->rnode);
    printf("%d,", t->data);
}

void IDisplayTree_preorder(Node *t) {
    Stack *st=createStack(MAX_TREE_NODE);

    while (t!=NULL || !isEmpty(st)) {
        if (t!=NULL) {
            printf("%d,", t->data);
            if (!push(st,t)) return;
            t=t->lnode;
        } else {
            if (!pop(st, (void **)&t)) return;
            t=t->rnode;
        }
    }
 }

void IDisplayTree_inorder(Node *t) {
    Stack *st=createStack(MAX_TREE_NODE);

    while (t!=NULL || !isEmpty(st)) {
        if (t!=NULL) {
            if (!push(st,t)) return;
            t=t->lnode;
        } else {
            if (!pop(st, (void **)&t)) return;
            printf("%d,", t->data);
            t=t->rnode;
        }
    } 
 }

void IDisplayTree_postorder(Node *t) {
    Stack *st=createStack(MAX_TREE_NODE);

    while (t!=NULL || !isEmpty(st)) {
        if (t!=NULL) {
            if (!push(st,t)) return;
            t=t->lnode;
        } else {
            long tmp;
            if (!pop(st, (void **)&t)) return;
            tmp = (long) t;
            if (tmp > 0) {
                push(st, (void *)-tmp);
                t=t->rnode;
            } else {
                t = (Node *)(-tmp);
                printf("%d,", t->data);
                t=NULL;
            }   
        }
    } 
}

void IDisplayTree_level(Node *t) {
    Queue *q = createQueue(MAX_TREE_NODE);
    printf("%d,", t->data);
    enqueue(q, t);

    while (!isEmpty(q)) {
        t=dequeue(q);
        if (t->lnode) {
            printf("%d,", t->lnode->data);
            enqueue(q, t->lnode);
        }
        if (t->rnode) {
            printf("%d,", t->rnode->data);
            enqueue(q, t->rnode);
        }

    }
}

void freeTree(Node *t) {
    Stack *st = createStack(MAX_TREE_NODE);

    while (t && !isEmptyStack(st)) {
        if (t) {
            if (!push(st, t)) return;
            t = t->lnode; // go all the way down to bottom of left
        } else {
            if (!pop(st, (void **)&t)) return; // pop a node to proccess it all right side child nodes
            long tmp = (long) t;
            if(t > 0) {
                push(st, (void *)-tmp);
                t = t->rnode;
            } else {
                Node *n = (Node *)(-tmp);
                free(n);
                t = NULL;
            } 
        }
    }
}

void fillTreeRef(Node *t, Stack *st_) {
    if (t) {
        fillTreeRef(t->lnode, st_);
        push(st_, t);
        fillTreeRef(t->rnode, st_);
    }
}

void RfreeTree(Node *t) {
    Stack *st_ = createStack(MAX_TREE_NODE);

    fillTreeRef(t, st_);
    while (!isEmptyStack(st_)) {
        Node *n;
        pop(st_, (void **)&n);
        free(n);
    }
}
int main(int argc, char*argv[]) {

    Queue *q = createQueue(MAX_TREE_NODE);
    Node *root;

    if (!q) {
        printf("\n failed to create assist Q \n");
        return 0;
    }
    
    int val = -1;
    printf("\n please enter root node value: ");
    scanf("%d", &val);
    Node *node = createTreeNode(val);
    root = node;
    if (!enqueue(q, node)) {
        printf("\n Failed to enqueue root node\n");
        return 0;
    }

    while (!isEmpty(q)) {
        Node *p = dequeue(q);
        printf("please input left node for node %d:", p->data);
        scanf("%d", &val);
        if (val != -1) {
            node = createTreeNode(val);
            enqueue(q, node);
            p->lnode = node;
        }
        printf("please input right node for node %d:", p->data);
        scanf("%d", &val);
        if (val != -1) {
            node = createTreeNode(val);
            enqueue(q, node);
            p->rnode = node;
        }
    }
    printf("\n Recursive Preorder traversal:\n");
    RDisplayTree_preorder(root);

    printf("\n Recursive Inorder traversal:\n");
    RDisplayTree_inorder(root);

    printf("\n Recursive Preorder traversal:\n");
    RDisplayTree_postorder(root);

    printf("\n========================\n");
    printf("\n Iterative Preorder traversal:\n");
    IDisplayTree_preorder(root);

    printf("\n Iterative Inorder traversal:\n");
    IDisplayTree_inorder(root);

    printf("\n Iterative Postorder traversal:\n");
    IDisplayTree_postorder(root);

    printf("\n Iterative Level traversal:\n");
    IDisplayTree_level(root);

    printf("\n Total tree sum: %d \n", RcountTree_sum(root));
    printf("\n Total tree nodes: %d \n", RcountTree_node(root));
    printf("\n Overall height: %d \n", RcountTree_height(root));
    //freeTree(root);
    RfreeTree(root);
    free(q);

    return 1;
}
