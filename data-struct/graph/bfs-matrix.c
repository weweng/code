/*
 * This file implements breadth first search of a graph,
 * whihc is a basic way of traversal of graph, which is defined
 * using a adjacent matrix
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define NODES 6
// adjacent matrix defining a grpah as below
//            1
//           / \
//          2   3
//           \ /
//            4
//           / \
//          5   6
int G[NODES+1][NODES+1] = {{0,0,0,0,0,0,0},
                           {0,0,1,1,0,0,0},
                           {0,1,0,0,1,0,0},
                           {0,1,0,0,1,0,0},
                           {0,0,1,1,0,1,1},
                           {0,0,0,0,1,0,0},
                           {0,0,0,0,1,0,0}};
int visited[NODES+1] = {0};

void bfs(int startNode, int result[]) {
    Queue *q=createQueue(NODES+1);
    int i=0;
    // initial set up
    result[i++]=startNode;
    enqueue(q, startNode);
    visited[startNode]=1;

    while(!isEmpty(q)) {
        int u=dequeue(q);
        // search thru graph to find who this node is connected to
        for (int v=1; v<NODES+1;v++) {
            if (G[u][v] == 1 && !visited[v]) {
                visited[v]=1;
                result[i++]=v;
                enqueue(q,v);
            }
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
    int result[NODES]={ 0 };

    bfs(startNode, result);

    for (int i=0; i<NODES; i++) {
        printf("%d%s ", result[i], (i==NODES-1?"\n":","));
    }

    return 1;
}
