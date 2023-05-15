#ifndef visualize_h
#define visualize_h

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

double calXRelativePos(double lon, double minLon, double maxLon);
double calYRelativePos(double lat, double minLat, double maxLat);
double my_sqrt(double x);
int printText(int wd, int ht, int boxSize, char* input);

int findNearestNode(RawNode* rawNodeList, int nodeNum, double xPos, double yPos, double* bounding);
#endif