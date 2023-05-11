#ifndef adjList_h
#define adjList_h

/* my header file */
#include "readMap.h"

/* structs to perform dijkstra */ 
typedef struct _Edge {
    int vertex; // the id of the node that this edge points at
    double len; //length or cost(i.e. time) of going through the edge
    struct _Edge* next; // link list of edges
} Edge;

typedef struct _AdjList {
    int lineNum; // number of lines(each node has a line) of adjacent list
    Edge** adjLines; // list to store each node's edges 
    // adjList with n nodes in the form like:
    // node  reachable nodes
    // 0   -->   1 2 3 
    // 1   -->   0 2 4 6 
    // 2   -->   x y ...
    // :   -->   : : : :
    // n-1 -->   a b c d
} AdjList;

// utilities
AdjList* initAdjList(int nodeNum);
void addEdge(AdjList* adjList, int start, int dest, double len);
void addEdgeTime(AdjList* adjList, int start, int dest, double len, double speed);
void addTotalEdge(AdjList* adjList, RawEdge* rawEdgeList, int edgeNum);
int minDist(double *dist, bool *sptSet, int nodeNum);

// for debugging
void printDist(double* dist, int nodeNum, Pair* pairs2);
void printAdjList(AdjList* adjList);


#endif