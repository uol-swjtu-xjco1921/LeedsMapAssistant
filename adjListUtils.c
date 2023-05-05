#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
/* my header files */
#include "adjList.h"
#define DELTA 1e-7
#define DBL_MAX_ 1e15 


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

void addEdge(AdjList* adjList, int start, int dest, double len) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->vertex = dest;
    // printf("%d ",edge->vertex);
    edge->len = len;
    edge->next = adjList->adjLines[start];
    
    adjList->adjLines[start] = edge;
    // printf("%d\n",adjList->lineNum);
}

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

void printDist(double* dist, int nodeNum, Pair* pair2) {
    printf("new id\t    original id\t    min dist\n");
    for (int i = 0; i < nodeNum; i++){
        // if(fabs(DBL_MAX_-dist[i])>DELTA){
        //     printf("%d\t    %d\t   %lf\n", i, find_key_by_value(pair2, nodeNum, i), dist[i]);
        // }else{
        //     printf("%d\t    %d\t   Inf\n", i, find_key_by_value(pair2, nodeNum, i));
        // }
        printf("%d\t    %d\t   %lf\n", i, find_key_by_value(pair2, nodeNum, i), dist[i]);
    }
}
