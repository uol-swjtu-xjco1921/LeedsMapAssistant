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
SDL_Rect * graphicPoints;
SDL_Rect * pathPoints;
SDL_Window * window;
SDL_Renderer * renderer;    
TTF_Font *font;

#define DELTA 1e-7
#define DBL_MAX_ 1e15 

#define X_SIZE 1200
#define Y_SIZE 960
#define RADIO 0.8

void printText(int wd, int ht, int boxSize, char* input){
    SDL_Surface *textAttr;
    SDL_Color black={0,0,0,255};
    textAttr=TTF_RenderText_Solid(font,input,black);
    SDL_Texture* text_texture=SDL_CreateTextureFromSurface(renderer,textAttr);
    SDL_Rect dest = {0, 0, textAttr->w, textAttr->h};
    SDL_Rect src = {wd, ht, textAttr->w, boxSize};
    SDL_RenderCopy(renderer, text_texture, &dest, &src); 
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture); 
    SDL_FreeSurface(textAttr); 
    // return textAttr->w;
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
double my_sqrt(double x) {
    double y = x / 2.0; // 初始值可以取 x/2，也可以取其他值
    double eps = 1e-6;  // 精度要求可以根据具体情况调整

    while (fabs(y * y - x) > eps) {
        y = (y + x / y) / 2.0;
    }

    return y;
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
        double x=calXRelativePos(rawNodeList[i].lon, minLon, maxLon)* X_SIZE ;
        double y=(1 - calYRelativePos(rawNodeList[i].lat, minLat, maxLat)) * Y_SIZE;
        if ((x-xPos) * (x-xPos) + (y-yPos) * (y-yPos) < nearest * nearest){
            nearest=my_sqrt((x-xPos) * (x-xPos) + (y-yPos) * (y-yPos));
            id=i;
        }
    }
    return id;
}

int showTask1Path(PathList* pathList, RawNode* rawNodeList, double *bounding,int pathNodeSize, bool* validPathNode){
    double minLat = bounding[0];
    double minLon = bounding[1];
    double maxLat = bounding[2];
    double maxLon = bounding[3];
    for(int i = 0; i < pathList->pathNum; i++){
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
        SDL_RenderDrawRect(renderer,&pathPoints[i]);
    }
    for(int i=0;i<pathList->pathNum-1;i++){
        int begin=pathList->path[i];
        int end=pathList->path[i+1];
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, 
            (rawNodeList[begin].lon - minLon) / (maxLon - minLon) * X_SIZE , 
            (1 - (rawNodeList[begin].lat - minLat) / (maxLat - minLat)) * Y_SIZE ,  
            (rawNodeList[end].lon - minLon) / (maxLon - minLon) * X_SIZE , 
            (1 - (rawNodeList[end].lat - minLat) / (maxLat - minLat)) * Y_SIZE 
        );

    }
    // SDL_RenderPresent(renderer);
    return 0;
}

int mySdl(char* mapFileName, RawNode* rawNodeList, double* bounding, AdjList* adjList, PathList* pathList){
    SDL_Init(SDL_INIT_EVERYTHING);
    // puts(SDL_GetError());
    if (TTF_Init() == -1)
    {
        return -1;
    }
    font=TTF_OpenFont("times.ttf",20);
    puts(SDL_GetError());

    window = SDL_CreateWindow("Path visualization", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, X_SIZE, Y_SIZE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    int nodeNum = getRawNodeNum(mapFileName);
    graphicPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect) * nodeNum);
    pathPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect)*pathList->pathNum);
    bool* validNode = (bool*)calloc(nodeNum, sizeof(bool));
    bool* validPathNode = (bool*)calloc(nodeNum, sizeof(bool));

    double minLat = bounding[0];
    double minLon = bounding[1];
    double maxLat = bounding[2];
    double maxLon = bounding[3];

    int basicNodeSize=6;
    int pathNodeSize=8;

    for(int i = 0; i < nodeNum; i++){
        
        graphicPoints[i].x = calXRelativePos(rawNodeList[i].lon,minLon,maxLon) * X_SIZE  - basicNodeSize / 2;
        graphicPoints[i].y = (1 - calYRelativePos(rawNodeList[i].lat,minLat,maxLat)) * Y_SIZE  - basicNodeSize / 2;
        graphicPoints[i].w = basicNodeSize;
        graphicPoints[i].h = basicNodeSize;
        if (withinBounding(rawNodeList[i].lon,rawNodeList[i].lat,bounding)){
            validNode[i]=true;
        }
    }


    //初始化
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
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
                // printf("%d ",tmp->vertex);
            }
            tmp=tmp->next;
        }
        // puts("");
    }

    
    
    
    SDL_RenderPresent(renderer);

    // double* pts2=1;
    SDL_Event ev;
    int nid[2]={-1,-1};
    char strArr[2][10] = {"start", "end"};

    bool quit=false;
    int n=0;
    while(!quit){

        while(SDL_PollEvent(&ev)){

            if(ev.type==SDL_QUIT){
                quit=true;
            }
  
            if (ev.type==SDL_MOUSEBUTTONDOWN){
                
                
                if(ev.button.button == SDL_BUTTON_LEFT) {




                    // 处理鼠标左键按下事件
                    SDL_Rect arr;
                    int x = ev.button.x;
                    int y = ev.button.y;
                    n=(n+1)%2;
                    printf("click point pos: (%d ,%d)\n", x,y);
                    nid[1-n]=findNearestNode(rawNodeList,nodeNum,x,y,bounding);
                    
                    if(nid[1-n]!=-1){
                    
                        arr.w = pathNodeSize;
                        arr.h = pathNodeSize;
                        arr.x = graphicPoints[nid[1-n]].x;
                        arr.y = graphicPoints[nid[1-n]].y;
                        SDL_RenderDrawRect(renderer,&arr);
                        SDL_SetRenderDrawColor(renderer,0,0,0,255);
                        SDL_RenderFillRect(renderer,&arr);
                        printf("At %s pos: (%d, %d)\n",strArr[1-n], graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
                        char text[200];
                        sprintf(text, "At %s pos: (%d, %d)", strArr[1-n], graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);

                        // printText(300, 100, 100, text);
                        // printf("related point of %s: id=%d lon=%lf, lat=%lf\n", strArr[1-n],rawNodeList[nid[1-n]].id,rawNodeList[nid[1-n]].lon,rawNodeList[nid[1-n]].lat);
                    }else{
                        
                        printf("related point of %s: None \n", strArr[1-n]);
                        n=(n+1)%2;
                    }
                    if(nid[1-n]!=-1&&nid[n]!=-1){
                        int* pd=(int*)calloc(nodeNum,sizeof(int));
                        dijk(adjList,nid[1-n],pd);
                        backtrackPath(pd, pathList, nid[1], nid[0], nodeNum);
                        showTask1Path(pathList, rawNodeList, bounding, pathNodeSize, validPathNode);
                    }
                    
                    SDL_RenderPresent(renderer);


            }
                
            }
        }

    }

    
    return 0;
}


