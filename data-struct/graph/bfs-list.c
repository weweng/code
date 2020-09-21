/*
 * This file implements breadth first search of a graph,
 * whihc is a basic way of traversal of graph, which is defined
 * using a adjacent list
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define NODES 6
// adjacent list defining a grpah as below
//            1
//           / \
//          2   3
//           \ /
//            4
//           / \
//          5   6
typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;
// G holds graph info, each node's info (of connected nodes) is accessed
// using index, which has a linked list of connected/neighbor nodes
Node *G[NODES+1];

Node *newNode(int num) {
    Node *newNode=calloc(1, sizeof(Node));
    newNode->data=num;
    return newNode;
}

void freeGraph(Node *G[]) {
    for (int i=1; i<=NODES;i++) {
        Node *tmp = G[i];
        while (tmp) {
            Node *next=tmp->next;
            free(tmp);
            tmp=next;
        }
    }
}

void createGraph(Node *G[]) {
    Node *node1, *node2, *node3, *node4;

    for (int i=1; i<=NODES;i++) {
        switch(i) {
        case 1: // first node connects to 2 and 3
            node1=newNode(2);
            node2=newNode(3);
            node1->next=node2;
            G[i]=node1;
            break;
        case 2:
        case 3:
            node1=newNode(1);
            node2=newNode(4);
            node1->next=node2;
            G[i]=node1;
            break;
        case 4:
            node1=newNode(2);
            node2=newNode(3);
            node3=newNode(5);
            node4=newNode(6);
            node1->next=node2;
            node2->next=node3;
            node3->next=node4;
            G[i]=node1;
            break;
        case 5:
        case 6:
            node1=newNode(4);
            G[i]=node1;
            break;
        default:
            break;
        }
    }
}

int visited[NODES+1] = { 0 };

void bfs_list(int startNode, int result[]) {
    int i = 0;
    Queue *q=createQueue(NODES);
    if (!q) return;

    // initial setup
    result[i++]=startNode;
    visited[startNode]=1;
    enqueue(q,startNode);
    // loop thru all neighbor/connected nodes for enqueued nodes
    while (!isEmpty(q)) {
        int u = dequeue(q);
        Node *tmp=G[u];
        while (tmp) {
            int v=tmp->data;
            if (visited[v] != 1) {
                visited[v]=1;
                result[i++]=v;
                enqueue(q,v);
            }
            tmp=tmp->next;
        }
    }
}

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("\n please provide starting node between 1 - 7\n");
        return 0;
    }
    int startNode = atoi(argv[1]);
    if (startNode < 1 || startNode > 7) {
        printf("\n invalid start node %d, should be between 1 - 7\n", startNode);
        return 0;
    }
    int result[NODES] = { 0 };

    createGraph(G);

    bfs_list(startNode, result);

    for (int i=0; i<NODES; i++) {
        printf("%d%s ", result[i], (i==NODES-1?"\n":","));
    }

    freeGraph(G);

    return 1;
}
