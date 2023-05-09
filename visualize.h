#ifndef visualize_h
#define visualize_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
/* my header files */
#include "adjList.h"
#include "readMap.h"
#include "routePlan.h"

int SDL(char* mapFileName, RawNode* rawNodeList, double* bounding, AdjList* adjList, PathList* pathList);
#endif