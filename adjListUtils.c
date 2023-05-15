#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
/* my header files */
#include "adjList.h"
#define DELTA 1e-7
#define DBL_MAX_ 1e15 

/**
 * initAdjList: initialize dijkstra
 * 
 * nodeNum: number of node store in map.
 * 
 * Return:
 *   Pointer to the newly initialized AdjList.
 */
AdjList* initAdjList(int nodeNum) {
    // malloc new adjList and edges inside
    AdjList* adjList = (AdjList*)malloc(sizeof(AdjList));
    adjList->lineNum = nodeNum;
    adjList->adjLines = (Edge**)malloc(nodeNum * sizeof(Edge*));

    for (int i = 0; i < nodeNum; i++) {
        adjList->adjLines[i] = NULL;
    }

    return adjList;
}
/**
 * Purpose: Add an edge to the adjacency list.
 * 
 * Parameters:
 *   adjList: Pointer to the AdjList.
 *   start: Starting node of the edge.
 *   dest: Destination node of the edge.
 *   len: Length or weight of the edge.
 */
void addEdge(AdjList* adjList, int start, int dest, double len) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->vertex = dest;
    edge->len = len;
    edge->next = adjList->adjLines[start];
    
    adjList->adjLines[start] = edge;
}
/**
 * Purpose: Add an edge to the adjacency list for min time.
 * 
 * Parameters:
 *   adjList: Pointer to the AdjList.
 *   start: Starting node of the edge.
 *   dest: Destination node of the edge.
 *   len: Length or weight of the edge.
 *   speed: Max speed of certain link.
 */
void addEdgeTime(AdjList* adjList, int start, int dest, double len, double speed) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->vertex = dest;
    edge->len = len/speed;
    edge->next = adjList->adjLines[start];
    
    adjList->adjLines[start] = edge;
}
/**
 * Purpose: Add total edges to the adjacency list.
 * 
 * Parameters:
 *   adjList: Pointer to the AdjList.
 *   rawEdgeList: Pointer to the list of raw edges(links).
 *   edgeNum: Number of edges in the rawEdgeList.
 */
void addTotalEdge(AdjList* adjList, RawEdge* rawEdgeList, int edgeNum){
    // quickly construct adjList
    for (int i=0;i<edgeNum;i++){
        // Example: addEdge(adjList, start, dest, length)
        // !Note: the edge here has the direction!
        // addEdge(adjList,0,1,2);
        addEdge(adjList, rawEdgeList[i].newNode1, rawEdgeList[i].newNode2, rawEdgeList[i].len);
        addEdge(adjList, rawEdgeList[i].newNode2, rawEdgeList[i].newNode1, rawEdgeList[i].len);
        
    }
}
/**
 * Purpose: Find the nodeId with min distance
 * 
 * Parameters:
 *   dist: list of distances from a node to each node in the map.
 *   sptSet: list indicating whether a node is included in the shortest path tree.
 *   nodeNum: number of nodes.
 * 
 * Return:
 *   Node ID with the min distance.
 */
int minDist(double *dist, bool *sptSet, int nodeNum) {
    // var to store the index of min dist
    int idMin;
    //init the address and value
    int n=0;
    bool* spt=sptSet;
    double* d=dist;
    // init the min dist
    double min = (double)DBL_MAX_;
    // compare 
    
    while(n<nodeNum){
        if (*spt==false && *d<min){
            idMin = n;
            min = *d;
        }
        // iterate and update
        n++;
        spt++;
        d++;
    }
    

    return idMin;
}
// The functions below are for debugging.

/**
 * Purpose: Print the adjacency list.
 * 
 * Parameters:
 *   adjList: Pointer to the AdjList.
 */
void printAdjList(AdjList* adjList){
    puts("");
    for(int i=0;i<adjList->lineNum;i++){
        Edge* tmp=adjList->adjLines[i];
        printf("%d --> ", i);
        while(tmp!=NULL){
            printf("%d ",tmp->vertex);
            tmp=tmp->next;
        }
        puts("");
    }
}
/**
 * Purpose: Print the distances of nodes in the graph.
 * 
 * Parameters:
 *   dist: Array of distances.
 *   nodeNum: Number of nodes.
 *   pair2: Pair ordered by new id
 */
void printDist(double* dist, int nodeNum, Pair* pair2) {
    printf("new id\t    original id\t    min dist\n");
    for (int i = 0; i < nodeNum; i++){
        
        printf("%d\t    %d\t   %lf\n", i, find_key_by_value(pair2, nodeNum, i), dist[i]);
    }
}
