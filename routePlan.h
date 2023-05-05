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

void dijk(AdjList* adjList, int start, Pair* pairs2);

#endif