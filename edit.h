#ifndef edit_h
#define edit_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
/* my header files */
#include "adjList.h"
#include "readMap.h"
#include "routePlan.h"
#include "visualize.h"

int editLinkVal(int mode, RawEdge* rawEdgeList, double changeVal, int linkId);

#endif