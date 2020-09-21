#include <stdio.h>
#include <stdlib.h>

typedef struct Node_ {
    struct Node_ *lnode;
    int data;
    struct Node_ *rnode;
} Node;

Node *createNode(int val) {
    Node *n = calloc(1, sizeof(Node));
    n->data=val;
    return n;
}

void insert_node(Node *root, int val) {
    Node *t = root;
    if (val <= t->data ) {
        if (t->lnode==NULL) {
            t->lnode=createNode(val);
        } else {
            insert_node(t->lnode, val);
        }
    } else {
        if (t->rnode==NULL) {
            t->rnode=createNode(val);
        } else {
            insert_node(t->rnode, val);
        }
    }
}

int contain_node(Node *root, int val) {
    printf("\n searched node is %d ", root->data);
    if (root->data==val) {
        return 1;
    } else if (val < root->data)  {
        return root->lnode? contain_node(root->lnode, val):0;
    } else if (val > root->data){
        return root->rnode? contain_node(root->rnode, val):0;       
    }
}
void Rtrav_preorder(Node *root) {
    if (root) {
        printf("%d, ", root->data);
        Rtrav_preorder(root->lnode);
        Rtrav_preorder(root->rnode);
    }
}
void Rtrav_inorder(Node *root) {
    if (root) {
        Rtrav_inorder(root->lnode);
        printf("%d, ", root->data);
        Rtrav_inorder(root->rnode);
    }
}
void Rtrav_postorder(Node *root) {
    if (root) {
        Rtrav_postorder(root->lnode);
        Rtrav_postorder(root->rnode);
        printf("%d, ", root->data);
    }
}
int main(int argc, char *argv[]) {
    Node *root = createNode(50);
    int A[] = {10,80,20,30,70,90,35,40,60,75,81,95};
    int len=sizeof(A)/sizeof(A[0]);
    for (int i=0; i<len; i++){
        insert_node(root, A[i]);
    }
    printf("\nPre-order traversal:");
    Rtrav_preorder(root);    
    printf("\nIn-order traversal:");
    Rtrav_inorder(root);    
    printf("\nPost-order traversal:");
    Rtrav_postorder(root);
    printf("\n Tree contains 60: %s", contain_node(root, 60)?"YES":"NO");    
}