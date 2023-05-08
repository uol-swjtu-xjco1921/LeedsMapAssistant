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

#define DELTA 1e-7
#define DBL_MAX_ 1e15 
int SDL(char* mapFileName, RawNode* rawNodeList, double* bounding, AdjList* adjList, PathList* pathList){
    #define X_SIZE 800
    #define Y_SIZE 800
    SDL_Rect * graphicPoints;
    SDL_Rect * pathPoints;
    SDL_Window * window;
    SDL_Renderer * renderer;    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Path Draw", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, X_SIZE, Y_SIZE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    int nodeNum = getRawNodeNum(mapFileName);
    graphicPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect) * nodeNum);
    pathPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect)*pathList->pathNum);
    bool* validNode = calloc(nodeNum, sizeof(bool));
    bool* validPathNode = calloc(nodeNum, sizeof(bool));

    double minLat = bounding[0];
    double minLon = bounding[1];
    double maxLat = bounding[2];
    double maxLon = bounding[3];

    int basicNodeSize=6;
    int pathNodeSize=10;

    for(int i = 0; i < nodeNum; i++){
        
        graphicPoints[i].x = (rawNodeList[i].lon - minLon) / (maxLon - minLon) * X_SIZE - basicNodeSize / 2;
        graphicPoints[i].y = (1 - (rawNodeList[i].lat - minLat) / (maxLat - minLat)) * Y_SIZE - basicNodeSize / 2;
        graphicPoints[i].w = basicNodeSize;
        graphicPoints[i].h = basicNodeSize;
        if (rawNodeList[i].lon>=minLon && rawNodeList[i].lon<=maxLon && rawNodeList[i].lat>=minLat && rawNodeList[i].lat<=maxLat){
            validNode[i]=true;
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    for(int i = 0; i < nodeNum; i++){
        if(validNode[i]){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer,&graphicPoints[i]);
        }
    }

    for(int i=0;i<nodeNum;i++){
        Edge* tmp=adjList->adjLines[i];
        
        while(tmp!=NULL){
            
            int end=tmp->vertex;
            if(validNode[i]&&validNode[end])
            {    SDL_RenderDrawLine(renderer,
                    (rawNodeList[i].lon - minLon) / (maxLon - minLon) * X_SIZE - basicNodeSize / 2, 
                    (1 - (rawNodeList[i].lat - minLat) / (maxLat - minLat)) * Y_SIZE - basicNodeSize / 2,  
                    (rawNodeList[end].lon - minLon) / (maxLon - minLon) * X_SIZE - basicNodeSize / 2, 
                    (1 - (rawNodeList[end].lat - minLat) / (maxLat - minLat)) * Y_SIZE - basicNodeSize / 2
                    );
                printf("%d ",tmp->vertex);
            }
            tmp=tmp->next;
        }
        puts("");
    }

    for(int i = 0; i < pathList->pathNum; i++){
        
        pathPoints[i].x = (rawNodeList[pathList->path[i]].lon - minLon) / (maxLon - minLon) * X_SIZE - pathNodeSize / 2;
        pathPoints[i].y = (1 - (rawNodeList[pathList->path[i]].lat - minLat) / (maxLat - minLat)) * Y_SIZE - pathNodeSize / 2;
        pathPoints[i].w = pathNodeSize;
        pathPoints[i].h = pathNodeSize;
        if (rawNodeList[pathList->path[i]].lon>=minLon && rawNodeList[pathList->path[i]].lon<=maxLon && rawNodeList[pathList->path[i]].lat>=minLat && rawNodeList[pathList->path[i]].lat<=maxLat){
            validPathNode[i]=true;
        }
    }
    for(int i = 0; i < pathList->pathNum; i++){
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer,&pathPoints[i]);
    }
    for(int i=0;i<pathList->pathNum-1;i++){
        int begin=pathList->path[i];
        int end=pathList->path[i+1];
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, 
            (rawNodeList[begin].lon - minLon) / (maxLon - minLon) * X_SIZE - basicNodeSize / 2, 
            (1 - (rawNodeList[begin].lat - minLat) / (maxLat - minLat)) * Y_SIZE - basicNodeSize / 2,  
            (rawNodeList[end].lon - minLon) / (maxLon - minLon) * X_SIZE - basicNodeSize / 2, 
            (1 - (rawNodeList[end].lat - minLat) / (maxLat - minLat)) * Y_SIZE - basicNodeSize / 2
            );

    }

    SDL_RenderPresent(renderer);
    SDL_Delay(20000);
    
    return 0;
}