/*
 * This file implements dijkstra  algorithm of a graph,
 * whihc is a way of create a graph, which is single 
 * source shortest path algorithm:
 * Two help data structures are needed:
 * 
 * 1. A boolean array sptSet[] to represent the set of vertices 
 * included in SPT. If a value sptSet[v] is true, then vertex v 
 * is included in SPT, otherwise not. 
 * 2. Array dist[] is used to store shortest distance values of all vertices.
 * 
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int boolean;
#define TRUE 1
#define FALSE 0

#define NODES 9
// adjacent matrix defining a grpah as below
int G[NODES][NODES] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 }, 
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 }, 
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 }, 
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 }, 
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 }, 
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 }, 
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 }, 
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 }, 
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };

int miniCost(boolean sptSet[], int dist[]) {
    int u=-1;
    int min=INT_MAX;
    for (int i=0; i<NODES;i++) {
        if (sptSet[i] == FALSE && dist[i] < min) {
            u=i, min=dist[i];
        }
    }
    return u;
}
void dijkstra(int G[NODES][NODES], int srcVertex, int result[]) {
    // helper boolean array to indicate if the vertex has been included in the shortest path
    boolean sptSet[NODES] = {FALSE};
    // final result of the shortest path, index is the vertex, the value is cost of the shortest 
    // path from source vertex
    int dist[NODES];

    for (int i = 0; i < NODES;i++) {
        dist[i]=INT_MAX;
        sptSet[i]=FALSE;
    }

    // initial set for source vertex
    dist[srcVertex]=0;
    //perform relaxation for each vertex
    for (int j = 0; j < NODES-1; j++) {    
        // find the pivot vertex, which is not in the shortest path set, but has minimum distance
        int u=miniCost(sptSet, dist);
        if (u < 0) return; ///something is wrong
        // for the given pivot vertex u, perform two tasks 
        // include u into sptSet, and update cost for each not included vertex if there is less
        sptSet[u] = TRUE;
        for (int v=0; v<NODES;v++) {
            if (sptSet[v] == FALSE && G[u][v] && dist[u] + G[u][v] < dist[v]) {
                dist[v] = dist[u] + G[u][v];
            }
        }
    }
    for (int j = 0; j < NODES; j++) 
        result[j] = dist[j];
}

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("\n please provide starting node between 1 - 7\n");
        return 0;
    }
    int srcVertex = atoi(argv[1]);
    if (srcVertex < 0 || srcVertex > 8) {
        printf("\n invalid start node %d, should be between 0 - 8\n", srcVertex);
        return 0;
    }
    int result[NODES]={ 0 };

    dijkstra(G, srcVertex, result);

    for (int i=0; i<NODES; i++) {
        printf("%d%s ", result[i], (i==NODES-1?"\n":","));
    }

    return 1;
}
