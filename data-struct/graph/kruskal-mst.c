#include <stdlib.h>
#include <stdio.h>

/* I: mean Infinity, V: means number of vertex */
#define I 0xFFFF
#define V 8

typedef struct Edge_ {
    struct Edge_ *next;
    int s;
    int e;
    int val;
} Edge;

void KruskalMST(int G[V][V], int n) {
    /* 
     * The key assited data structure are:
     *   - sorted linked list which has all edges 
     *   - disjoin subet table/Array for loop detection: each vertex has one value
     *     if the value is positive, it is its parent vertex,
     *     if the value is negative, it is number child it has
     *   - an array hold all vertexes of included edges
     *
     * The result table is 
     * res[2][V-1]: holding start vertex and end vertex of an edge which is part of MinCost Span Tree
     *
     */
    int res[2][V-1] = { 0 };
    int res_v[V] = {0 }; // only for explored vertex
    int subset[V] = { -1, -1, -1, -1, -1, -1, -1, -1 };
    
    Edge *head = NULL, *tmp, *prev;
    int u, v, c = 0, res_v_idx = 0;
    for (u = 0; u < V; u++) {
        for (v=0; v < u; v++) {
            if (G[u][v] != I) {
                Edge *e = calloc(1, sizeof(Edge));
                e->s = u;
                e->e = v;
                e->val = G[u][v];
                if (head == NULL) {
                    head = e;
                } else {
                    // perform insert sorting
                    if (e->val <= head->val) {
                        e->next = head;
                        head = e;
                    } else {
                        prev = head;
                        tmp = head->next;
                        while (tmp) {
                            if (e->val <= tmp->val) {
                                prev->next = e;
                                e->next = tmp;
                                break;
                            } else {
                                prev = tmp;
                                tmp = tmp->next;
                            }
                        }
                    }
                }                   
            }
        }
    }
    printf("\n Edge list:");  tmp = head; while (tmp) { printf("[%d->%d]=%d, ", tmp->s, tmp->e, tmp->val); tmp=tmp->next; }

    // go thru all edges and add the edge which doesn't create a loop
    tmp = head;
    c = 0;
    while (tmp) {
        if (c == V-1) break; // find enough edges
        
        // check if this edge causes a loop?
        int i_s = 0, i_e = 0, inc_v = -1, new_v = -1, p;
        for (int i = 0; i < res_v_idx; i++) {
            if (res_v[i] == tmp->s) i_s = 1;
            if (res_v[i] == tmp->e) i_e = 1;
        }
        if (!i_s || !i_e) {
            // at least one vertex is not included, will not cause loop, add it
            res[0][c] = tmp->s;
            res[1][c] = tmp->e;

            if (!i_s) res_v[res_v_idx++] = tmp->s;
            if (!i_e) res_v[res_v_idx++] = tmp->e;
                
            c++;
                
            // update subset table for later use
            if (!i_s && i_e) {
                inc_v = tmp->e;
                new_v = tmp->s;
            } else if (!i_e && i_s){
                inc_v = tmp->s;
                new_v = tmp->e;                    
            }
            if (inc_v > 0 && new_v > 0) {
                // merge new vertex into existing subset by finding included vertex's parent
                p = inc_v;
                while (subset[p] > 0) {
                    p = subset[p];
                }
                // join start node into subset under node p
                subset[p] = subset[p] - 1;
                subset[new_v] = p;
            } else {
                // both are not included, form a new subset using start vertex as parent
                subset[tmp->s] = -2;
                subset[tmp->e] = tmp->s;
            }
        } else {
            // both vertexes are included, check subset table to see if this new edge will create a loop
            int p_s = tmp->s;
            while (subset[p_s] > 0) {
                p_s = subset[p_s];
            }
            int p_e = tmp->e;
            while (subset[p_e] > 0) {
                p_e = subset[p_e];
            }
            if (p_s != p_e) {
                // different parent, will not cause loop, add them
                res[0][c] = tmp->s;
                res[1][c] = tmp->e;
                c++;
                // now need t join two subsets to form bigger subset
                if (subset[p_s] > subset[p_e]) { // since they are negative numbers
                    subset[p_e] += subset[p_s];
                    subset[p_s] = p_e;
                } else {
                    subset[p_s] += subset[p_e];
                    subset[p_e] = p_s;
                }
            }
        }

        tmp = tmp->next;
    }
    
    printf("MiniCost Spanning Tree is:\n");
    for (u = 0; u < V-1; u++) {
	    printf(" %d->%d \n", res[0][u], res[1][u]);
    }

    while (head) {
        tmp = head->next;
        free(head);
        head = tmp;
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
 
    KruskalMST(cost, n);
 
    return 0;
}

