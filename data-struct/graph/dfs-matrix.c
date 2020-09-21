/*
 * This file implements depth first search of a graph,
 * whihc is a basic way of traversal of graph, which is defined
 * using a adjacent matrix
 */

#include <stdio.h>
#include <stdlib.h>

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
int result[NODES] = { 0 };

void dfs_matrix(int startNode) {
    static int i=0;
    
    if (visited[startNode]==0) {
        visited[startNode]=1;
        result[i++]=startNode;
        for (int j=1;j<=NODES;j++) {
            if (G[startNode][j] != 0 && visited[j] == 0) {
                dfs_matrix(j);
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

    dfs_matrix(startNode);

    for (int i=0; i<NODES; i++) {
        printf("%d%s ", result[i], (i==NODES-1?"\n":","));
    }

    return 1;
}
