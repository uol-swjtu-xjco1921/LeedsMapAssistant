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


#define DELTA 1e-7
#define DBL_MAX_ 1e15 

#define X_SIZE 1600
#define Y_SIZE 960
#define MAP_X_SIZE 1200

bool withinBounding(double lon, double lat, double *bounding){
    double minLat = bounding[0];
    double minLon = bounding[1];
    double maxLat = bounding[2];
    double maxLon = bounding[3];
    return lon>=minLon && lon<=maxLon && lat>=minLat && lat<=maxLat;
}
double calXRelativePos(double lon, double minLon, double maxLon){
    return (lon - minLon) / (maxLon - minLon);
}
double calYRelativePos(double lat, double minLat, double maxLat){
    return (lat - minLat) / (maxLat - minLat);
}

int findNearestNode(RawNode *rawNodeList, int nodeNum, double xPos, double yPos, double *bounding)
{
    int id=-1;
    double minLat = bounding[0];
    double minLon = bounding[1];
    double maxLat = bounding[2];
    double maxLon = bounding[3];
    double nearest = 10;
    for(int i=0;i<nodeNum;i++){
        double x=calXRelativePos(rawNodeList[i].lon, minLon, maxLon)* MAP_X_SIZE;
        double y=(1 - calYRelativePos(rawNodeList[i].lat, minLat, maxLat)) * Y_SIZE;
        if ((x-xPos) * (x-xPos) + (y-yPos) * (y-yPos) < nearest * nearest){
            nearest=sqrt((x-xPos) * (x-xPos) + (y-yPos) * (y-yPos));
            id=i;
        }
    }
    return id;
}

int findLink(int start, int end, RawEdge* rawEdgeList,int linkNum){
    for(int i=0;i<linkNum;i++){
        
        if(
            (rawEdgeList[i].newNode1==start&&rawEdgeList[i].newNode2==end)
            ||(rawEdgeList[i].newNode2==start&&rawEdgeList[i].newNode1==end)
          ){
            return i;
        }
    }
    return -1;
}



