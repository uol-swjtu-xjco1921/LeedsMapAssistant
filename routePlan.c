#include <stdio.h>
/* my header files */
#include "routePlan.h"

#define DELTA 1e-7
#define DBL_MAX_ 1e15 

void dijk(AdjList* adjList, int start, int* pd, double* dist) {
    // init dijkstra
    int lineNum = adjList->lineNum;
    
    bool* travelSet=(bool*)malloc(lineNum*sizeof(bool)); //present set of traveled points 
    
    for (int i = 0; i < lineNum; i++){
        pd[i]=-1;
        travelSet[i] = false;
        dist[i] = DBL_MAX_;
    }
    dist[start] = 0;
    // perform dijk
    for (int count = 0; count < lineNum - 1; count++) {
        //u
        int idMin = minDist(dist, travelSet, lineNum);
        travelSet[idMin] = true;
        Edge* tmp;
        // int num=0;
        for (tmp = adjList->adjLines[idMin]; tmp != NULL; tmp = tmp->next) {
            double tmpLen = tmp->len;
            int tmpId = tmp->vertex;
            if ( fabs(DBL_MAX_-dist[idMin]) > DELTA && travelSet[tmpId]==false && dist[tmpId]-dist[idMin]-tmpLen > DELTA ) {
                dist[tmpId] = dist[idMin] + tmpLen;
                pd[tmpId]=idMin;
            }
        }
    }
    
}

void dijkTime(AdjList* adjListTime, int start, int* pd, double* dist) {
    // init dijkstra
    int lineNum = adjListTime->lineNum;
    
    bool* travelSet=(bool*)malloc(lineNum*sizeof(bool)); //present set of traveled points 
    
    for (int i = 0; i < lineNum; i++){
        pd[i]=-1;
        travelSet[i] = false;
        dist[i] = DBL_MAX_;
    }
    dist[start] = 0;
    // perform dijk
    for (int count = 0; count < lineNum - 1; count++) {
        //u
        int idMin = minDist(dist, travelSet, lineNum);
        travelSet[idMin] = true;
        Edge* tmp;
        // int num=0;
        for (tmp = adjListTime->adjLines[idMin]; tmp != NULL; tmp = tmp->next) {
            double tmpLen = tmp->len;
            int tmpId = tmp->vertex;
            if ( fabs(DBL_MAX_-dist[idMin]) > DELTA && travelSet[tmpId]==false && dist[tmpId]-dist[idMin]-tmpLen > DELTA ) {
                dist[tmpId] = dist[idMin] + tmpLen;
                pd[tmpId]=idMin;
            }
        }
    }
    
}

void printPath(PathList* pathList){
    puts("");
    for(int i=0;i<pathList->pathNum;i++){
        printf("第%d站: id=%d\n",i,pathList->path[i]);
    }
}

void backtrackPath(int* pd, PathList* pathList, int start, int end, int nodeNum) {
    int path[nodeNum];
    int path_index = 0;

    int current_node = end;

    while (current_node != start) {
        path[path_index++] = current_node;
        current_node = pd[current_node];
    }
    path[path_index] = start;
        
    pathList->pathNum=path_index+1;
    pathList->path=(int*)malloc(sizeof(int)*(path_index+1));
    for (int i = path_index; i >= 0; i--) {
        pathList->path[path_index-i]=path[i];
        
    }
     
}

