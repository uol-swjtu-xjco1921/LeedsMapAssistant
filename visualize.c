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
#include "visualize.h"
#define DELTA 1e-7
#define DBL_MAX_ 1e15 

#define X_SIZE 1500
#define Y_SIZE 1200
#define RADIO 0.8

void printMenu(){
    ;
}

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

int SDL(char* mapFileName, RawNode* rawNodeList, double* bounding, AdjList* adjList, PathList* pathList){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Rect * graphicPoints;
    SDL_Rect * pathPoints;
    SDL_Window * window;
    SDL_Renderer * renderer;    
    
    window = SDL_CreateWindow("Path visualization", SDL_WINDOWPOS_CENTERED,
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
        
        graphicPoints[i].x = calXRelativePos(rawNodeList[i].lon,minLon,maxLon) * X_SIZE  - basicNodeSize / 2;
        graphicPoints[i].y = (1 - calYRelativePos(rawNodeList[i].lat,minLat,maxLat)) * Y_SIZE  - basicNodeSize / 2;
        graphicPoints[i].w = basicNodeSize;
        graphicPoints[i].h = basicNodeSize;
        if (withinBounding(rawNodeList[i].lon,rawNodeList[i].lat,bounding)){
            validNode[i]=true;
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    SDL_Event ev;
    bool quit=false;

    while(!quit){
        while(SDL_PollEvent(&ev)){
            if(ev.type==SDL_QUIT){
                quit=true;
            }
            else
            {
                for(int i = 0; i < nodeNum; i++){
                    if(validNode[i]){
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        SDL_RenderDrawRect(renderer,&graphicPoints[i]);
                    }
                }

                for(int i=0;i<nodeNum;i++){
                    Edge* tmp=adjList->adjLines[i];
                    
                    while(tmp!=NULL){
                        
                        int end=tmp->vertex;
                        if(validNode[i]&&validNode[end])
                        {    SDL_RenderDrawLine(renderer,
                                calXRelativePos(rawNodeList[i].lon,minLon,maxLon)* X_SIZE , 
                                (1 - calYRelativePos(rawNodeList[i].lat,minLat,maxLat)) * Y_SIZE ,  
                                calXRelativePos(rawNodeList[end].lon,minLon,maxLon) * X_SIZE , 
                            (1 - calYRelativePos(rawNodeList[end].lat,minLat,maxLat)) * Y_SIZE 
                                );
                            printf("%d ",tmp->vertex);
                        }
                        tmp=tmp->next;
                    }
                    puts("");
                }

                for(int i = 0; i < pathList->pathNum; i++){
                    // 
                    pathPoints[i].x = calXRelativePos(rawNodeList[pathList->path[i]].lon, minLon, maxLon) * X_SIZE - pathNodeSize / 2;
                    pathPoints[i].y = (1 - (calYRelativePos(rawNodeList[pathList->path[i]].lat, minLat, maxLat))) * Y_SIZE - pathNodeSize / 2;
                    pathPoints[i].w = pathNodeSize;
                    pathPoints[i].h = pathNodeSize;
                    if (withinBounding(rawNodeList[pathList->path[i]].lon,rawNodeList[pathList->path[i]].lat,bounding)){
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
            }
        }
    }

    
    
    return 0;
}


