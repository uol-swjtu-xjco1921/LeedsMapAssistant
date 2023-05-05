#ifndef readMap_h
#define readMap_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* structs to store map data in expected way */
typedef struct _RawEdge { 
    // store the info of edges(links) in map assign the new id to edge's newId
    int id; // id in the 'link' label
    int newId; // assigned id for future usage
    int node1; // id of the start node
    int node2; // id of the end node
    int newNode1; // assigned and new id of the start node
    int newNode2; // assigned and new id of the end node
    int way; // way info in the 'link' label
    double len; // length value in the 'link' label
    double veg; // veg value in the 'link' label
    double arch; // arch value in the 'link' label
    double land; // land value in the 'link' label
    char* poi; // poi info in the 'link' label
}RawEdge;

typedef struct _RawNode { 
    //store the info of nodes in map and assign the new id to node's newId 
    int id; // id in the 'node' label
    int newId; // assigned and new id in the 'node' label
    double lat; //  lat value in the 'node' label
    double lon; //  lon value in the 'node' label
}RawNode;

typedef struct _Pair {
    // bridge the original id with new id 
    int key; // original id
    int value; // new id
    // related functions:
    // 1.find_value_by_key
    // 2.find_key_by_value
}Pair;

// utilities
int getRawNodeNum(char* mapFileName);
int getRawLinkNum(char* mapFileName);
int compare_pair_key(const void *a, const void *b);
int find_value_by_key(Pair* pairs, int n, int key);
int find_key_by_value(Pair* pairs2, int n, int value);
int getRawNode(char* mapFileName, RawNode* rawNodeList, int nodeNum, Pair* pairs, Pair* pairs2);
int getRawLink(char* mapFileName, RawEdge* rawEdgeList, int edgeNum, Pair* pairs, int nodeNum);


// for debugging
void printRawEdgeList(RawEdge* rawEdgeList, int numEdges);
void printRawNodeList(RawNode* rawNodeList, int numNodes);
void printPairs(Pair* pairs, int numNodes);

#endif