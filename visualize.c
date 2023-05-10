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

#define X_SIZE 1200
#define Y_SIZE 960
#define RADIO 0.8




// int mySdl(char* mapFileName, RawNode* rawNodeList, double* bounding, AdjList* adjList, PathList* pathList, double* dist){
//     SDL_Init(SDL_INIT_EVERYTHING);
//     // puts(SDL_GetError());
//     if (TTF_Init() == -1)
//     {
//         return -1;
//     }
//     font=TTF_OpenFont("times.ttf",20);
//     puts(SDL_GetError());

//     window = SDL_CreateWindow("Path visualization", SDL_WINDOWPOS_CENTERED,
//     SDL_WINDOWPOS_CENTERED, X_SIZE, Y_SIZE, SDL_WINDOW_SHOWN);
//     renderer = SDL_CreateRenderer(window, -1, 0);
    
//     int nodeNum = getRawNodeNum(mapFileName);
//     graphicPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect) * nodeNum);
//     pathPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect)*pathList->pathNum);
//     bool* validNode = (bool*)calloc(nodeNum, sizeof(bool));
//     bool* validPathNode = (bool*)calloc(nodeNum, sizeof(bool));

//     double minLat = bounding[0];
//     double minLon = bounding[1];
//     double maxLat = bounding[2];
//     double maxLon = bounding[3];

//     int basicNodeSize=6;
//     int pathNodeSize=8;

//     for(int i = 0; i < nodeNum; i++){
        
//         graphicPoints[i].x = calXRelativePos(rawNodeList[i].lon,minLon,maxLon) * X_SIZE  - basicNodeSize / 2;
//         graphicPoints[i].y = (1 - calYRelativePos(rawNodeList[i].lat,minLat,maxLat)) * Y_SIZE  - basicNodeSize / 2;
//         graphicPoints[i].w = basicNodeSize;
//         graphicPoints[i].h = basicNodeSize;
//         if (withinBounding(rawNodeList[i].lon,rawNodeList[i].lat,bounding)){
//             validNode[i]=true;
//         }
//     }


//     //初始化
//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//     SDL_RenderClear(renderer);
//     for(int i = 0; i < nodeNum; i++){
//         if(validNode[i]){
//             SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//             SDL_RenderDrawRect(renderer,&graphicPoints[i]);
//         }
//     }

//     for(int i=0;i<nodeNum;i++){
//         Edge* tmp=adjList->adjLines[i];
        
//         while(tmp!=NULL){
            
//             int end=tmp->vertex;
//             if(validNode[i]&&validNode[end])
//             {    SDL_RenderDrawLine(renderer,
//                     calXRelativePos(rawNodeList[i].lon,minLon,maxLon)* X_SIZE , 
//                     (1 - calYRelativePos(rawNodeList[i].lat,minLat,maxLat)) * Y_SIZE ,  
//                     calXRelativePos(rawNodeList[end].lon,minLon,maxLon) * X_SIZE , 
//                 (1 - calYRelativePos(rawNodeList[end].lat,minLat,maxLat)) * Y_SIZE 
//                     );
//                 // printf("%d ",tmp->vertex);
//             }
//             tmp=tmp->next;
//         }
//         // puts("");
//     }

    
    
    
//     SDL_RenderPresent(renderer);

//     // double* pts2=1;
//     SDL_Event ev;
//     int nid[2]={-1,-1};
//     char strArr[2][10] = {"start", "end"};

//     bool quit=false;
//     int n=0;
//     while(!quit){

//         while(SDL_PollEvent(&ev)){

//             if(ev.type==SDL_QUIT){
//                 quit=true;
//             }
  
//             if (ev.type==SDL_MOUSEBUTTONDOWN){
                
                
//                 if(ev.button.button == SDL_BUTTON_LEFT) {




//                     // 处理鼠标左键按下事件
//                     SDL_Rect arr;
//                     int x = ev.button.x;
//                     int y = ev.button.y;
//                     n=(n+1)%2;
//                     printf("click point pos: (%d ,%d)\n", x,y);
//                     nid[1-n]=findNearestNode(rawNodeList,nodeNum,x,y,bounding);
                    
//                     if(nid[1-n]!=-1){
                    
//                         arr.w = pathNodeSize;
//                         arr.h = pathNodeSize;
//                         arr.x = graphicPoints[nid[1-n]].x;
//                         arr.y = graphicPoints[nid[1-n]].y;
//                         SDL_RenderDrawRect(renderer,&arr);
//                         SDL_SetRenderDrawColor(renderer,0,0,0,255);
//                         SDL_RenderFillRect(renderer,&arr);
//                         printf("At %s pos: (%d, %d)\n",strArr[1-n], graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
//                         char text[200];
//                         sprintf(text, "At %s pos: (%d, %d)", strArr[1-n], graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);

//                         // printText(300, 100, 100, text);
//                         // printf("related point of %s: id=%d lon=%lf, lat=%lf\n", strArr[1-n],rawNodeList[nid[1-n]].id,rawNodeList[nid[1-n]].lon,rawNodeList[nid[1-n]].lat);
//                     }else{
                        
//                         printf("related point of %s: None \n", strArr[1-n]);
//                         n=(n+1)%2;
//                     }

//                     if(nid[1-n]!=-1&&nid[n]!=-1){
//                         int* pd=(int*)calloc(nodeNum,sizeof(int));
//                         dijk(adjList,nid[1-n],pd, dist);

//                         backtrackPath(pd, pathList, nid[1-n], nid[n], nodeNum);
//                         printf("min dist=%lf\n", dist[nid[n]]);
//                         showTask1Path(pathList, rawNodeList, bounding, pathNodeSize, validPathNode);
//                     }
                    
//                     SDL_RenderPresent(renderer);


//             }
                
//             }
//         }

//     }

    
//     return 0;
// }


