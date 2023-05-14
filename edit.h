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

int searchNode(int mode, RawNode* rawNodeList, double a, double b, int *nodeId, Pair* pairs,int nodeNum);
int editLinkVal(int mode, RawEdge* rawEdgeList, double changeVal, int linkId);
int writeEditedMap(char* newMapFileName,double* bounding, RawEdge* rawEdgeList, RawNode* rawNodeList, Way* wayList, Geom* geomList, int linkNum,int nodeNum,int wayNum, int geomNum);
#endif