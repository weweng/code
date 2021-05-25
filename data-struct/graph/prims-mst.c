#include <stdlib.h>
#include <stdio.h>

/* I: mean Infinity, V: means number of vertex */
#define I 0xFFFF
#define V 8

void PrimsMST(int G[V][V], int n) {
    /* 
     * The key assited data structure is an array near[V], 
     * the index of array representing a vertex from 0 to V-1
     * the value of array representing the vertex which has minimun distance 
     * from given vertex to all explored vertexs, For explored vertex, it is set to zero.
     * 
     *  near[V]  I  v1,  0,  v3, 0, ............
     *           0, 1,   2,  3,  4,  5, ... V-1
     *
     * Each iteration is to find the vertex which has minimum cost
     * and put it into the result table 
     *
     * The result table is 
     * res[2][V-1]: holding start vertex and end vertex of an edge which is part of MinCost Span Tree
     *
     */
    int res[2][V-1] = { 0 };
    int res_v[V] = {0 }; // only for explored vertex
    int near[V] = { I, I, I, I, I, I, I, I };

    // identify the minicost edge in the whole graph to get started
    int u, v, min = I, minS = 0, minE = 0, res_e_idx = 0, res_v_idx = 0;;
    for (u = 0; u < V; u++) {
        for (v =0; v <= u; v++) { // since it is symetrical, no need go thru all
            if (G[u][v] < min) {
                min = G[u][v];
                minS = u;
                minE = v;
            }
        }
    }

    do {
        res[0][res_e_idx] = minS;
        res[1][res_e_idx] = minE;
        res_e_idx++;
        if (res_e_idx == (V-1)) break; // found all mini edges

        //add minS, minE vertex into explored vertex set
        int included_s = 0, included_e = 0;
        for (int i =0; i < res_v_idx; i++) {
            if (res_v[i] == minS) {
                included_s = 1;
            }
            if (res_v[i] == minE) {
                included_e = 1;
            }
        }
        if (!included_s) res_v[res_v_idx++] = minS;
        if (!included_e) res_v[res_v_idx++] = minE;

        near[minS] = 0;
        near[minE] = 0;
        
        // update near table to get next candidate edges
        // go thru one by one for not-included vertex against the included vertex
        // fill near table with included-vertex for not-included vertex
        int k = 0;
        for (u = 0; u < V; u++) {
            min = I;
            k = 0;
            for (v = 0; v < res_v_idx-1; v++) {
                if ( near[u] != 0 && G[u][res_v[v]] < min) {
                    min = G[u][res_v[v]];
                    k = res_v[v];
                }
            }
            if (k) near[u] = k;
        }

        // go thru near table to identify new min edge
        min = I;
        int tmp_s = 0, tmp_e = 0;
        for (u = 0; u < V; u++) {
            if (near[u] != 0 && near[u] != I && G[u][near[u]] < min) {
                min = G[u][near[u]];
                tmp_s = u; tmp_e = near[u];
            }
        }
        if (tmp_s ==0 && tmp_e ==0) break;

        minS = tmp_s; minE = tmp_e;
        
    } while (1);
    
    printf("MiniCost Spanning Tree is:\n");
    for (u = 0; u < V-1; u++) {
	    printf(" %d->%d \n", res[0][u], res[1][u]);
    }
    
}

int main() {
 
    int cost [V][V] = {
            {I, I, I, I, I, I, I, I},
            {I, I, 25, I, I, I, 5, I},
            {I, 25, I, 12, I, I, I, 10},
            {I, I, 12, I, 8, I, I, I},
            {I, I, I, 8, I, 16, I, 14},
            {I, I, I, I, 16, I, 20, 18},
            {I, 5, I, I, I, 20, I, I},
            {I, I, 10, I, 14, 18, I, I},
    };
 
    int n = sizeof(cost[0])/sizeof(cost[0][0]) - 1;
 
    PrimsMST(cost, n);
 
    return 0;
}
