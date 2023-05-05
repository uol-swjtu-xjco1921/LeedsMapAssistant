#include <stdio.h>
/* my header files */
#include "routePlan.h"

#define DELTA 1e-7
#define DBL_MAX_ 1e15 

void dijk(AdjList* adjList, int start, Pair* pairs2) {
    // init dijkstra
    int lineNum = adjList->lineNum;
    bool* travelSet=(bool*)malloc(lineNum*sizeof(bool)); //present set of traveled points 
    double* dist=(double*)malloc(lineNum*sizeof(double)); //distance of each point
    
    for (int i = 0; i < lineNum; i++){
        travelSet[i] = false;
        dist[i] = DBL_MAX_;
    }
    dist[start] = 0;
    // perform dijk
    for (int count = 0; count < lineNum - 1; count++) {
        int idMin = minDist(dist, travelSet, lineNum);
        travelSet[idMin] = true;
        Edge* tmp;
        for (tmp = adjList->adjLines[idMin]; tmp != NULL; tmp = tmp->next) {
            double tmpLen = tmp->len;
            int tmpId = tmp->vertex;
            if ( fabs(DBL_MAX_-dist[idMin]) > DELTA && travelSet[tmpId]==false && dist[tmpId]-dist[idMin]-tmpLen > DELTA ) {
                dist[tmpId] = dist[idMin] + tmpLen;
            }
        }
    }

    printDist(dist, lineNum, pairs2);
}