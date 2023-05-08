#ifndef routePlan_h
#define routePlan_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
/* my header files */
#include "adjList.h"
#include "readMap.h"

typedef struct _PathList{
    int pathNum;
    int* path; 
}PathList;

void dijk(AdjList* adjList, int start, Pair* pairs2, int* pd); 
void backtrackPath(int* pd, PathList* pathList, int start, int end, int nodeNum);
void printPath(PathList* pathList);

#endif