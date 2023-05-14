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
#include "edit.h"

#define DELTA 1e-7
#define DBL_MAX_ 1e15 

#define X_SIZE 1600
#define Y_SIZE 960
#define MAP_X_SIZE 1200
// #define RADIO 0.8
SDL_Rect* graphicPoints;
SDL_Rect* pathPoints;
SDL_Rect* textBox;
SDL_Window* window;
SDL_Renderer * renderer;    


int printText(int wd, int ht, int boxSize, char* input){
    TTF_Font *font;
    font=TTF_OpenFont("times.ttf",20);
    // puts(SDL_GetError());
    SDL_Surface *textAttr;
    SDL_Color black={0,0,0,255};
    textAttr=TTF_RenderText_Solid(font,input,black);
    SDL_Texture* text_texture=SDL_CreateTextureFromSurface(renderer,textAttr);
    SDL_Rect dest = {0, 0, textAttr->w, textAttr->h};
    SDL_Rect src = {wd, ht, textAttr->w, boxSize};
    SDL_RenderCopy(renderer, text_texture, &dest, &src); 
    
    SDL_DestroyTexture(text_texture); 
    SDL_FreeSurface(textAttr); 
    return 0;
}
int printTextSize(int wd, int ht, int boxSize, char* input,int size){
    TTF_Font *font;
    font=TTF_OpenFont("times.ttf",size);
    // puts(SDL_GetError());
    SDL_Surface *textAttr;
    SDL_Color black={0,0,0,255};
    textAttr=TTF_RenderText_Solid(font,input,black);
    SDL_Texture* text_texture=SDL_CreateTextureFromSurface(renderer,textAttr);
    SDL_Rect dest = {0, 0, textAttr->w, textAttr->h};
    SDL_Rect src = {wd, ht, textAttr->w, boxSize};
    SDL_RenderCopy(renderer, text_texture, &dest, &src); 
    
    SDL_DestroyTexture(text_texture); 
    SDL_FreeSurface(textAttr); 
    return 0;
}

void printMenu(){
    printTextSize(1220, 40, 35, "Menu for route planning:",35);
    printTextSize(1240, 120, 30, "1.Min Distance",30);
    printTextSize(1240, 220, 30, "2.Min Time",30);
    printTextSize(1240, 320, 30, "3.Pass Ordered Nodes",30);
    printTextSize(1240, 420, 30, "4.Edit Link",30);
    printTextSize(1240, 520, 30, "5.Search Node",30);
    printTextSize(1240, 620, 30, "6.Add Link",30);

    // printText(1300, 300, 20, "3.Pass Certain Nodes");
    // SDL_RenderPresent(renderer);
    


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

// void initSDL(int nodeNum, SDL_Rect* graphicPoints, AdjList* adjList,RawNode* rawNodeList, Geom* geomList,int geomNum, double* bounding, bool* validNode)    
void initSDL(int nodeNum, SDL_Rect* graphicPoints, AdjList* adjList,RawNode* rawNodeList, double* bounding, bool* validNode)       
   { 
    double minLat = bounding[0];
    double minLon = bounding[1];
    double maxLat = bounding[2];
    double maxLon = bounding[3];
    
    //画点
    for(int i = 0; i < nodeNum; i++){
        if(validNode[i]){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer,&graphicPoints[i]);
        }
    }
    //画线
    for(int i=0;i<nodeNum;i++){
        Edge* tmp=adjList->adjLines[i];
        
        while(tmp!=NULL){
            
            int end=tmp->vertex;
            if(validNode[i]&&validNode[end])
            {    SDL_RenderDrawLine(renderer,
                    calXRelativePos(rawNodeList[i].lon,minLon,maxLon)* MAP_X_SIZE , 
                    (1 - calYRelativePos(rawNodeList[i].lat,minLat,maxLat)) * Y_SIZE ,  
                    calXRelativePos(rawNodeList[end].lon,minLon,maxLon) * MAP_X_SIZE , 
                (1 - calYRelativePos(rawNodeList[end].lat,minLat,maxLat)) * Y_SIZE 
                    );
                // printf("%d ",tmp->vertex);
            }
            tmp=tmp->next;
        }
        // puts("");
    }

    // for(int i=0; i<geomNum;i++){
    //     printf("check i=%d   %d\n",i,geomList[i].nodeNum);
    //     for(int j=0; j<geomList[i].nodeNum;j++)
    //     {   
    //         int end=j+1;
    //         printf("geomList[i].newNodeList[end=%d]=%d\n",end,geomList[i].newNodeList[end]);
    //         printf("id=%d/%d\n",geomList[i].newNodeList[j],geomList[i].nodeList[j]);
    //         // if(j==geomList[i].nodeNum-1){
    //         //     end=0;
    //         // }else{
    //         //     end=j+1;
    //         // }
    //         if(validNode[geomList[i].newNodeList[j]]&&validNode[geomList[i].newNodeList[end]])
    //         {       
    //                 SDL_SetRenderDrawColor(renderer, 255, 20, 0, 50);

    //                 SDL_RenderDrawLine(renderer,
    //                 calXRelativePos(rawNodeList[geomList[i].newNodeList[j]].lon,minLon,maxLon)* MAP_X_SIZE , 
    //                 (1 - calYRelativePos(rawNodeList[geomList[i].newNodeList[j]].lat,minLat,maxLat)) * Y_SIZE ,  
    //                 calXRelativePos(rawNodeList[geomList[i].newNodeList[end]].lon,minLon,maxLon) * MAP_X_SIZE , 
    //             (1 - calYRelativePos(rawNodeList[geomList[i].newNodeList[end]].lat,minLat,maxLat)) * Y_SIZE 
    //                 );
                
    //             printf("%d\n",end);
    //         }
    //     }

    // }
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);



    // printMenu();
    
    
    SDL_RenderPresent(renderer);
}



int showTaskPath(PathList* pathList, RawNode* rawNodeList, double *bounding,int pathNodeSize, bool* validPathNode){
    double minLat = bounding[0];
    double minLon = bounding[1];
    double maxLat = bounding[2];
    double maxLon = bounding[3];
    for(int i = 0; i < pathList->pathNum; i++){
        pathPoints[i].x = calXRelativePos(rawNodeList[pathList->path[i]].lon, minLon, maxLon) * MAP_X_SIZE - pathNodeSize / 2;
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
            (rawNodeList[begin].lon - minLon) / (maxLon - minLon) * MAP_X_SIZE , 
            (1 - (rawNodeList[begin].lat - minLat) / (maxLat - minLat)) * Y_SIZE ,  
            (rawNodeList[end].lon - minLon) / (maxLon - minLon) * MAP_X_SIZE , 
            (1 - (rawNodeList[end].lat - minLat) / (maxLat - minLat)) * Y_SIZE 
        );

    }
    SDL_RenderPresent(renderer);
    return 0;
}

// int editLinkVal(int mode, RawEdge* rawEdgeList, double changeVal, int linkId){
//     int i=linkId;
//     switch (mode)
//     {
//         case 1:
//         {
//             rawEdgeList[i].speed=changeVal;
//             break;
//         }
//         case 2:
//         {
//             rawEdgeList[i].arch=changeVal;
//             break;
//         }
//         case 3:
//         {
//             rawEdgeList[i].veg=changeVal;
//             break;
//         }
//         case 4:
//         {
//             rawEdgeList[i].land=changeVal;
//             break;
//         }
//     }
//     return 0;
// }


int main(int argc, char* argv[]) {
    
    char* mapFileName;
    if (argc==1){
        printf("usage: %s mapFile\n",argv[0]);
        return 0; //functionally succeed but remind how to use 
    }else if(argc==2){
        mapFileName=argv[1];
        printf("using '%s' for route planning\n",mapFileName);
    }else {
        printf("error arguments count\n");
        exit(1); //error case 1: bad input arguments,
    }

    FILE *fp=fopen(mapFileName,"r");
    if (fp==NULL){
        fprintf(stderr, 
            "errors exist in opening map file '%s'\nPlease check file again.\n",
            mapFileName
            );
        exit(2); //error case 2: bad map file reading
    }
    fclose(fp);

    
    double *bounding=(double*)malloc(sizeof(double)*4);
    printf("get bounding? %d\n",getRawBound(mapFileName, bounding));
    int linkNum=getRawLinkNum(mapFileName);
    int nodeNum=getRawNodeNum(mapFileName);
    int geomNum=getRawGeomNum(mapFileName);
    int wayNum=getRawWayNum(mapFileName);
    
    printf("linkNum: %d, nodeNum: %d, wayNum: %d, geomNum: %d\n",linkNum,nodeNum,wayNum,geomNum);

    Pair *pairs = (Pair *)malloc(nodeNum * sizeof(Pair));
    Pair *pairs2 = (Pair *)malloc(nodeNum * sizeof(Pair));

    AdjList* adjList = initAdjList(nodeNum);
    AdjList* adjListTime = initAdjList(nodeNum);



    RawNode* rawNodeList=(RawNode*)malloc(nodeNum*sizeof(RawNode));
    
    getRawNode(mapFileName, rawNodeList, nodeNum, pairs, pairs2);

    qsort(pairs, nodeNum, sizeof(Pair), compare_pair_key);

    Way*  wayList=(Way*)malloc(sizeof(Way)*wayNum);
    getWayEachNodeNum(mapFileName,wayList);
    getRawWay(mapFileName,wayList,wayNum,pairs,nodeNum);
    // printRawWayList(wayList, wayNum);
    Geom* geomList=(Geom*)malloc(sizeof(Geom)*geomNum);
    getGeomEachNodeNum(mapFileName,geomList);
    getRawGeom(mapFileName,geomList,geomNum,pairs2,nodeNum);

    RawEdge* rawEdgeList=(RawEdge*)malloc(linkNum*sizeof(RawEdge));
    getRawLink(mapFileName, rawEdgeList, linkNum, pairs, nodeNum);

    for (int i=0;i<linkNum;i++){
        addEdge(adjList, rawEdgeList[i].newNode1, rawEdgeList[i].newNode2, rawEdgeList[i].len);
        addEdge(adjList, rawEdgeList[i].newNode2, rawEdgeList[i].newNode1, rawEdgeList[i].len);
    }
    for (int i=0;i<linkNum;i++){
        addEdgeTime(adjListTime, rawEdgeList[i].newNode1, rawEdgeList[i].newNode2, rawEdgeList[i].len,rawEdgeList[i].speed);
        addEdgeTime(adjListTime, rawEdgeList[i].newNode2, rawEdgeList[i].newNode1, rawEdgeList[i].len,rawEdgeList[i].speed);
        
    }



    double* dist=(double*)malloc(nodeNum*sizeof(double));
    int *pd=(int*)malloc(sizeof(int)*nodeNum);
    PathList* pathList=(PathList*)malloc(sizeof(PathList));
    // PathList* tmpPathList=(PathList*)malloc(50*sizeof(PathList));
    int totalPaths[500];
    int totalNum=0;
    PathList* totalPathList=(PathList*)malloc(sizeof(PathList));
    
    // totalPathList->path=(int*)malloc(500*sizeof(int));

    dijk(adjList, 0, pd,dist);
    printf("\nlineNum: %d\n",adjList->lineNum);
    backtrackPath(pd,pathList, 0, 100,nodeNum);
    int basicNodeSize=6;
    int pathNodeSize=8;
    SDL_Init(SDL_INIT_EVERYTHING);
    // puts(SDL_GetError());
    if (TTF_Init() == -1)
    {
        return -1;
    }
    

    window = SDL_CreateWindow("Path visualization", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, X_SIZE, Y_SIZE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    
    graphicPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect) * nodeNum);
    
    // writeEditedMap(
    //     "newLeeds.map",
    //     bounding,
    //     rawEdgeList,
    //     rawNodeList,
    //     wayList,
    //     geomList,
    //     linkNum,
    //     nodeNum,
    //     wayNum,
    //     geomNum
    // );

    bool* validNode = (bool*)calloc(nodeNum, sizeof(bool));
    bool* validPathNode = (bool*)calloc(nodeNum, sizeof(bool));

    double minLat = bounding[0];
    double minLon = bounding[1];
    double maxLat = bounding[2];
    double maxLon = bounding[3];

    

    for(int i = 0; i < nodeNum; i++){
        
        graphicPoints[i].x = calXRelativePos(rawNodeList[i].lon,minLon,maxLon) * MAP_X_SIZE  - basicNodeSize / 2;
        graphicPoints[i].y = (1 - calYRelativePos(rawNodeList[i].lat,minLat,maxLat)) * Y_SIZE  - basicNodeSize / 2;
        graphicPoints[i].w = basicNodeSize;
        graphicPoints[i].h = basicNodeSize;
        if (withinBounding(rawNodeList[i].lon,rawNodeList[i].lat,bounding)){
            validNode[i]=true;
        }
    }

    

    
    SDL_Event ev;
    
    RawEdge* rawEdgeForAdd=(RawEdge*)malloc(sizeof(RawEdge));
    
    rawEdgeForAdd->poi=(char*)malloc(sizeof(char)*20);
    // rawEdgeForAdd->poi="";
    bool isAddLink=false;
    textBox = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    textBox->h=100;
    textBox->w=360;
    textBox->x=1220;
    textBox->y=100;

    
    int nid[2]={-1,-1};
    
    char strArr[2][10] = {"start", "end"};
    int h_tmp=300;
    bool quit=false;
    bool isPrint=false;
    int n=0;
    // int canClear=0;
    int keyOfMenu=-1;
    int oid[50];
    char text3[3][100];
    memset(oid, -1, sizeof(oid));
    bool stopSelect=false;
    double tmpDist3=0;
    char text4[50]={};
    char text5[50]={};
    char text6[100]={};
    // char text4cp[50]={};
    // int textPos=-1;
    bool isFirst=true;
    int newMapNum=1;
    bool clearByRight=false;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    //初始化
    // initSDL(nodeNum, graphicPoints, adjList, rawNodeList, geomList,geomNum, bounding, validNode);
    initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
    printMenu();
    SDL_RenderPresent(renderer);
    while(!quit){
    
        while(SDL_PollEvent(&ev)){
            

            switch(ev.type){
                case SDL_QUIT:
                     quit=true;
                     break;          
                
                case SDL_MOUSEBUTTONDOWN:
                {
               
                    switch (keyOfMenu){
                        
                        case 1:
                        {
                        
                        
                        if(ev.button.button == SDL_BUTTON_LEFT) {
                            // 处理鼠标左键按下事件
                            
                            SDL_Rect arr;
                        
                            int x = ev.button.x;
                            int y = ev.button.y;
                            n=(n+1)%2;
                            printf("click point pos: (%d ,%d)\n", x,y);
                            nid[1-n]=findNearestNode(rawNodeList,nodeNum,x,y,bounding);
                            
                            if(nid[1-n]!=-1)
                            {
                                arr.w = pathNodeSize;
                                arr.h = pathNodeSize;
                                arr.x = graphicPoints[nid[1-n]].x;
                                arr.y = graphicPoints[nid[1-n]].y;
                                SDL_RenderDrawRect(renderer,&arr);
                                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                                SDL_RenderFillRect(renderer,&arr);
                                SDL_RenderPresent(renderer);
                                printf("At %s pos(id = %d): (%d, %d)\n",strArr[1-n],nid[1-n], graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
                                

                                
                            }else{
                                printf("related point of %s: None \n", strArr[1-n]);

                                n=(n+1)%2;
                            }

                            if(nid[1-n]!=-1&&nid[n]!=-1&&n==0){
                                int* pd=(int*)calloc(nodeNum,sizeof(int));
                                dijk(adjList,nid[1-n],pd, dist);
                                
                                if(dist[nid[n]]>1e10){
                                    printf("no link or way !!!\n");
                                    printTextSize(1250,120,20,"No link or way Found!",30);
                                    SDL_RenderPresent(renderer);
                                    nid[0]=-1;
                                    nid[1]=-1;
                                    n=0;
                                    h_tmp=300;
                                    break;
                                }else{backtrackPath(pd, pathList, nid[1-n], nid[n], nodeNum);
                                pathPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect)*pathList->pathNum);
                                char text[7][200];
                                
                                sprintf(text[0], "Click at start pos (id = %d/%d)", nid[1-n], find_key_by_value(pairs2, nodeNum,nid[1-n]));
                                sprintf(text[1], " (%.3lf, %.3lf)", rawNodeList[nid[1-n]].lon, rawNodeList[nid[1-n]].lat);
                                sprintf(text[2], " (%d, %d)", graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
                                sprintf(text[3], "Click at end pos (id = %d/%d)",nid[n], find_key_by_value(pairs2, nodeNum,nid[n]));
                                sprintf(text[4], " (%.3lf, %.3lf)", rawNodeList[nid[n]].lon, rawNodeList[nid[n]].lat);
                                sprintf(text[5], " (%d, %d)", graphicPoints[nid[n]].x, graphicPoints[nid[n]].y);
                                sprintf(text[6], " Min Distance=%.3lf", dist[nid[n]]);
                                
                                
                                
                                if(!isPrint)
                                {   
                                    printTextSize(1200,h_tmp,40,text[6],30);
                                    h_tmp+=40;
                                    printf("%d\n",h_tmp);

                                    printText(1220,h_tmp,30,text[0]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);
                                    
                                    printText(1220,h_tmp,30,text[1]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);
                                    

                                    printText(1220,h_tmp,30,text[2]);
                                    h_tmp+=90;
                                    printf("%d\n",h_tmp);

                                    printText(1220,h_tmp,30,text[3]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);

                                    printText(1220,h_tmp,30,text[4]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);

                                    printText(1220,h_tmp,30,text[5]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);

                                    
                                    isPrint=true;
                                }else{
                                    ;
                                }
                                printf("Min Dist=%.3lf\n", dist[nid[n]]);
                                showTaskPath(pathList, rawNodeList, bounding, pathNodeSize, validPathNode);
                                
                                // canClear=1;
                            }
                            }
                                
                            
                            
                            if (n==0){
                            nid[0]=-1;
                            nid[1]=-1;
                            

                        }
                        }else if(ev.button.button == SDL_BUTTON_RIGHT){
                            // SDL_RenderClear(renderer);
                            //初始化
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderClear(renderer);
                            // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                            
                            initSDL(nodeNum, graphicPoints, adjList, rawNodeList,bounding, validNode);
                            printTextSize(1210,50,40,"Planning for Min Distance",35);
                            h_tmp=300;
                            SDL_RenderPresent(renderer);
                            n=0;
                            // canClear=0;
                            isPrint=false;
                        } 
                        // SDL_RenderPresent(renderer);
                        break;
                        }

                        case 2:
                        {
                            if(ev.button.button == SDL_BUTTON_LEFT) {
                            // 处理鼠标左键按下事件
                            
                            SDL_Rect arr;
                            int x = ev.button.x;
                            int y = ev.button.y;
                            n=(n+1)%2;
                            printf("click point pos: (%d ,%d)\n", x,y);
                            nid[1-n]=findNearestNode(rawNodeList,nodeNum,x,y,bounding);
                            
                            if(nid[1-n]!=-1)
                            {
                                arr.w = pathNodeSize;
                                arr.h = pathNodeSize;
                                arr.x = graphicPoints[nid[1-n]].x;
                                arr.y = graphicPoints[nid[1-n]].y;
                                SDL_RenderDrawRect(renderer,&arr);
                                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                                SDL_RenderFillRect(renderer,&arr);
                                SDL_RenderPresent(renderer);
                                printf("At %s pos(id = %d / %d): (%d, %d)\n",strArr[1-n],nid[1-n],find_key_by_value(pairs2, nodeNum,nid[1-n]) , graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
                                

                            }else{
                                printf("related point of %s: None \n", strArr[1-n]);

                                n=(n+1)%2;
                            }

                            if(nid[1-n]!=-1&&nid[n]!=-1&&n==0){
                                int* pd=(int*)calloc(nodeNum,sizeof(int));
                                dijk(adjListTime,nid[1-n],pd, dist);
                                
                                if(dist[nid[n]]>1e10){
                                    printf("no link or way !!!\n");
                                    printTextSize(1250,120,20,"No link or way!",30);
                                    SDL_RenderPresent(renderer);
                                    nid[0]=-1;
                                    nid[1]=-1;
                                    n=0;
                                    h_tmp=300;
                                    break;
                                }
                                backtrackPath(pd, pathList, nid[1-n], nid[n], nodeNum);
                                pathPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect)*pathList->pathNum);
                                // int linkId=findLink(pathList->path[0],pathList->path[1],rawEdgeList,linkNum);
                                char text[8][200];
                                // char textNode[1000]={0};
                                sprintf(text[0], "Click at start pos (id = %d/%d)", nid[1-n],find_key_by_value(pairs2, nodeNum,nid[1-n]));
                                sprintf(text[1], " (%.3lf, %.3lf)", rawNodeList[nid[1-n]].lon, rawNodeList[nid[1-n]].lat);
                                sprintf(text[2], " (%d, %d)", graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
                                sprintf(text[3], "Click at end pos (id = %d/%d)", nid[n], find_key_by_value(pairs2, nodeNum,nid[n]));
                                sprintf(text[4], " (%.3lf, %.3lf)", rawNodeList[nid[n]].lon, rawNodeList[nid[n]].lat);
                                sprintf(text[5], " (%d, %d)", graphicPoints[nid[n]].x, graphicPoints[nid[n]].y);
                                sprintf(text[6], " Min Time=%.3lf", dist[nid[n]]);
                                sprintf(text[7],"The Quickest Path from start pos (id = %d/%d) to end pos (id = %d/%d) is: \n" ,
                                            nid[1-n],find_key_by_value(pairs2, nodeNum,nid[1-n]), nid[n], find_key_by_value(pairs2, nodeNum,nid[n]));
                                FILE* outfp=fopen("routePlanQuickPath.txt", "w");
                                fprintf(outfp,"%s",text[7]);
                                for(int node=0;node<pathList->pathNum;node++){
                                    
                                    
                                    if(node==0){
                                        fprintf(outfp, "%d",pathList->path[node]);
                                    }else{
                                        fprintf(outfp, "->%d",pathList->path[node]);
                                    }
                                   
                                }
                                
                                fclose(outfp);
                                
                                
                                
                                
                                if(!isPrint)
                                {
                                   
                                    printTextSize(1200,h_tmp,40,text[6],40);
                                    h_tmp+=40;
                                    printf("%d\n",h_tmp);

                                    printText(1220,h_tmp,30,text[0]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);
                                    
                                    printText(1220,h_tmp,30,text[1]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);
                                    

                                    printText(1220,h_tmp,30,text[2]);
                                    h_tmp+=90;
                                    printf("%d\n",h_tmp);

                                    printText(1220,h_tmp,30,text[3]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);

                                    printText(1220,h_tmp,30,text[4]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);

                                    printText(1220,h_tmp,30,text[5]);
                                    h_tmp+=30;
                                    printf("%d\n",h_tmp);

                                    

                                    isPrint=true;
                                }else{
                                    ;
                                }
                                printf("Min time=%.3f\n", dist[nid[n]]);
                                showTaskPath(pathList, rawNodeList, bounding, pathNodeSize, validPathNode);
                                
                                // canClear=1;
                            }
                            
                            
                            if (n==0){
                            nid[0]=-1;
                            nid[1]=-1;

                        }
                        }else if(ev.button.button == SDL_BUTTON_RIGHT){
                            // SDL_RenderClear(renderer);
                            //初始化
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderClear(renderer);
                            // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                            initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                            printTextSize(1210,50,40,"Planning for Min Time",35);
                            
                            h_tmp=300;
                            SDL_RenderPresent(renderer);
                            n=0;
                            isPrint=false;
                            // canClear=0;
                        } 
                        // SDL_RenderPresent(renderer);
                        break;
                        }
                        case 3:
                        {
                            if(ev.button.button == SDL_BUTTON_LEFT) {
                            // 处理鼠标左键按下事件
                            
                            SDL_Rect arr;
                            int x = ev.button.x;
                            int y = ev.button.y;
                            n=n+1;
                            printf("click point pos: (%d ,%d)\n", x,y);
                            oid[n-1]=findNearestNode(rawNodeList,nodeNum,x,y,bounding);
                            
                            if(oid[n-1]!=-1&&validNode[oid[n-1]])
                            {
                                arr.w = pathNodeSize;
                                arr.h = pathNodeSize;
                                arr.x = graphicPoints[oid[n-1]].x;
                                arr.y = graphicPoints[oid[n-1]].y;
                                SDL_RenderDrawRect(renderer,&arr);
                                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                                SDL_RenderFillRect(renderer,&arr);
                                SDL_RenderPresent(renderer);
                                printf("At No.%d pos(id = %d): (%d, %d)\n",n,oid[n-1], graphicPoints[oid[n-1]].x, graphicPoints[oid[n-1]].y);
                                // sprintf(text3[0], "At No.%d pos(id = %d): (%d, %d)\n",n,oid[n-1], graphicPoints[oid[n-1]].x, graphicPoints[oid[n-1]].y);
                                // SDL_RenderPresent(renderer);

                                
                            }else{
                                printf("related point: None, skip and continue selecting No.%d \n",n);
                                
                                sprintf(text3[1],"related point: None, skip and continue selecting No.%d \n",n);
                                n=n-1;
                            }


                            

                        
                        }else if(ev.button.button == SDL_BUTTON_RIGHT){
                            // SDL_RenderClear(renderer);
                            //初始化
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderClear(renderer);
                            // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                            initSDL(nodeNum, graphicPoints, adjList, rawNodeList,bounding, validNode);

                            printTextSize(1250,50,40,"Planning for Min Distance",40);
                            printText(1250,90,20,"(Passing ordered points)");

                            h_tmp=300;
                            SDL_RenderPresent(renderer);
                            n=0;
                            for(int m=0;m<50;m++){
                                oid[m]=-1;
                            }
                            totalNum=0;
                            for(int m=0;m<500;m++){
                                totalPaths[m]=-1;
                            }
                            // totalPathList->path=NULL;
                                
                            totalPathList->pathNum=0;
                            
                            // canClear=0;
                            isPrint=false;
                        } 
                        // if(stopSelect){

                        // }
                        // SDL_RenderPresent(renderer);
                        break;
                        }
                        case 4:
                        {   
                            if(ev.button.button == SDL_BUTTON_LEFT){
                                SDL_Point pos={ev.button.x, ev.button.y};
                                
                                if(SDL_PointInRect(&pos, textBox)){
                                    SDL_StartTextInput();
                                }else{
                                    SDL_StopTextInput();
                                    SDL_Rect arr;
                                    int x = ev.button.x;
                                    int y = ev.button.y;
                                    n=n+1;
                                    printf("click point pos: (%d ,%d)\n", x,y);
                                    oid[n-1]=findNearestNode(rawNodeList,nodeNum,x,y,bounding);
                                    
                                    if(oid[n-1]!=-1&&validNode[oid[n-1]])
                                    {
                                        arr.w = pathNodeSize;
                                        arr.h = pathNodeSize;
                                        arr.x = graphicPoints[oid[n-1]].x;
                                        arr.y = graphicPoints[oid[n-1]].y;
                                        SDL_RenderDrawRect(renderer,&arr);
                                        SDL_SetRenderDrawColor(renderer,0,0,0,255);
                                        SDL_RenderFillRect(renderer,&arr);
                                        SDL_RenderPresent(renderer);
                                        printf("At No.%d pos(id = %d): (%d, %d)\n",n,oid[n-1], graphicPoints[oid[n-1]].x, graphicPoints[oid[n-1]].y);
                                        // sprintf(text3[0], "At No.%d pos(id = %d): (%d, %d)\n",n,oid[n-1], graphicPoints[oid[n-1]].x, graphicPoints[oid[n-1]].y);
                                        // SDL_RenderPresent(renderer);

                                        
                                    }else{
                                        printf("related point: None, skip and continue selecting No.%d \n",n);
                                        
                                        sprintf(text3[1],"related point: None, skip and continue selecting No.%d \n",n);
                                        n=n-1;
                                    }
                                }
                            }else if(ev.button.button==SDL_BUTTON_RIGHT){
                                 if(keyOfMenu==4){
                                    isFirst=true;
                                    clearByRight=true;
                                }
                                // keyOfMenu=-1;
                                n=0;
                                oid[0]=-1;
                                oid[1]=-1;
                                memset(text4, 0, sizeof(text4));
                                
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderClear(renderer);
                                // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                                initSDL(nodeNum, graphicPoints, adjList, rawNodeList,bounding, validNode);
                                printTextSize(1210,50,35,"Edit Link",35);
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                SDL_RenderDrawRect(renderer,textBox);
                                // printMenu();
                                SDL_RenderPresent(renderer);
                            }
                            break;
                        }
                        case 5:
                        {
                            if(ev.button.button == SDL_BUTTON_LEFT){
                                SDL_Point pos={ev.button.x, ev.button.y};
                                
                                if(SDL_PointInRect(&pos, textBox)){
                                    SDL_StartTextInput();
                                }else{
                                    SDL_StopTextInput();
                                    // SDL_Rect arr;
                                    // int x = ev.button.x;
                                    // int y = ev.button.y;
                                    // n=n+1;
                                    // printf("click point pos: (%d ,%d)\n", x,y);
                                    // oid[n-1]=findNearestNode(rawNodeList,nodeNum,x,y,bounding);
                                    
                                    // if(oid[n-1]!=-1&&validNode[oid[n-1]])
                                    // {
                                    //     arr.w = pathNodeSize;
                                    //     arr.h = pathNodeSize;
                                    //     arr.x = graphicPoints[oid[n-1]].x;
                                    //     arr.y = graphicPoints[oid[n-1]].y;
                                    //     SDL_RenderDrawRect(renderer,&arr);
                                    //     SDL_SetRenderDrawColor(renderer,0,0,0,255);
                                    //     SDL_RenderFillRect(renderer,&arr);
                                    //     SDL_RenderPresent(renderer);
                                    //     printf("At No.%d pos(id = %d): (%d, %d)\n",n,oid[n-1], graphicPoints[oid[n-1]].x, graphicPoints[oid[n-1]].y);
                                    //     // sprintf(text3[0], "At No.%d pos(id = %d): (%d, %d)\n",n,oid[n-1], graphicPoints[oid[n-1]].x, graphicPoints[oid[n-1]].y);
                                    //     // SDL_RenderPresent(renderer);

                                        
                                    // }else{
                                    //     printf("related point: None, skip and continue selecting No.%d \n",n);
                                        
                                    //     sprintf(text3[1],"related point: None, skip and continue selecting No.%d \n",n);
                                    //     n=n-1;
                                    // }

                                }
                            }else if(ev.button.button==SDL_BUTTON_RIGHT){
                                 if(keyOfMenu==5){
                                    isFirst=true;
                                    clearByRight=true;
                                }
                                // keyOfMenu=-1;
                                n=0;
                                oid[0]=-1;
                                oid[1]=-1;
                                memset(text5, 0, sizeof(text5));
                                
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderClear(renderer);
                                // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                                initSDL(nodeNum, graphicPoints, adjList, rawNodeList,bounding, validNode);
                                printTextSize(1210,50,35,"Search Node",35);
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                SDL_RenderDrawRect(renderer,textBox);
                                // printMenu();
                                SDL_RenderPresent(renderer);


                                // // ====================================
                                // // SDL_RenderClear(renderer);
                                // //初始化
                                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                // SDL_RenderClear(renderer);
                                // // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                                // initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                                // printTextSize(1210,50,40,"Planning for Min Time",35);
                                
                                // h_tmp=300;
                                // SDL_RenderPresent(renderer);
                                // n=0;
                                // isPrint=false;
                                // // canClear=0;
                                
                            }
                            break;
                        }
                        case 6:
                        {
                            if(ev.button.button == SDL_BUTTON_LEFT){
                                SDL_Point pos={ev.button.x, ev.button.y};
                                
                                if(SDL_PointInRect(&pos, textBox)){
                                    SDL_StartTextInput();
                                }else{
                                    SDL_StopTextInput();
                                    SDL_Rect arr;
                                    int x = ev.button.x;
                                    int y = ev.button.y;
                                    n=n+1;
                                    printf("click point pos: (%d ,%d)\n", x,y);
                                    oid[n-1]=findNearestNode(rawNodeList,nodeNum,x,y,bounding);
                                    
                                    if(oid[n-1]!=-1&&validNode[oid[n-1]])
                                    {
                                        arr.w = pathNodeSize;
                                        arr.h = pathNodeSize;
                                        arr.x = graphicPoints[oid[n-1]].x;
                                        arr.y = graphicPoints[oid[n-1]].y;
                                        SDL_RenderDrawRect(renderer,&arr);
                                        SDL_SetRenderDrawColor(renderer,0,0,0,255);
                                        SDL_RenderFillRect(renderer,&arr);
                                        SDL_RenderPresent(renderer);
                                        printf("At No.%d pos(id = %d): (%d, %d)\n",n,oid[n-1], graphicPoints[oid[n-1]].x, graphicPoints[oid[n-1]].y);
                                        // sprintf(text3[0], "At No.%d pos(id = %d): (%d, %d)\n",n,oid[n-1], graphicPoints[oid[n-1]].x, graphicPoints[oid[n-1]].y);
                                        // SDL_RenderPresent(renderer);

                                        
                                    }else{
                                        printf("related point: None, skip and continue selecting No.%d \n",n);
                                        
                                        sprintf(text3[1],"related point: None, skip and continue selecting No.%d \n",n);
                                        n=n-1;
                                    }
                                }
                            }else if(ev.button.button==SDL_BUTTON_RIGHT){
                                if(keyOfMenu==6){
                                    isFirst=true;
                                    clearByRight=true;
                                }
                                // keyOfMenu=-1;
                                n=0;
                                oid[0]=-1;
                                oid[1]=-1;
                                memset(text6, 0, sizeof(text4));
                                
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderClear(renderer);
                                // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                                initSDL(nodeNum, graphicPoints, adjList, rawNodeList,bounding, validNode);
                                printTextSize(1210,50,35,"Add Link",35);
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                SDL_RenderDrawRect(renderer,textBox);
                                // printMenu();
                                SDL_RenderPresent(renderer);
                            }
                            break;
                        }
                    }
                    break;
                }
                case SDL_KEYDOWN:
                {
                    int key = ev.key.keysym.sym;
                    
                    if (key == SDLK_1&&keyOfMenu==-1)
                    {   
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                        SDL_RenderClear(renderer);
                        SDL_RenderPresent(renderer);
                         //初始化
                        // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);

                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        keyOfMenu=1;
                        printTextSize(1210,50,40,"Planning for Min Distance",35);
                        SDL_RenderPresent(renderer);
                        // calculate dijk min dist

                        
                    }
                    else if(key == SDLK_2&&keyOfMenu==-1)
                    {
                        keyOfMenu=2;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                        SDL_RenderClear(renderer);
                        SDL_RenderPresent(renderer);
                         //初始化
                         // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        

                        
                        printTextSize(1210,50,40,"Planning for Min Time",35);

                        SDL_RenderPresent(renderer);
                        // calculate dijk min time
                        break;
                    }
                    else if(key == SDLK_3&&keyOfMenu==-1)
                    {
                        
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                        SDL_RenderClear(renderer);
                        SDL_RenderPresent(renderer);
                         //初始化
                            // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);

                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        keyOfMenu=3;
                        printTextSize(1210,50,30,"Planning for Min Distance",30);
                        printText(1210,70,20,"(passing ordered points)");

                        SDL_RenderPresent(renderer);
                        // calculate min dist passing certain point
                        break;
                    }
                    else if(key == SDLK_4&&keyOfMenu==-1)
                    {   
                        keyOfMenu=4;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);
                        SDL_RenderPresent(renderer);

                        // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);

                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList,bounding, validNode);
                        printTextSize(1210,50,35,"Edit Link",35);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer,textBox);

                        // printText(1250,70,20,"(passing order points)");
                        // EDIT NODE
                        SDL_RenderPresent(renderer);
                        break;
                    }
                    else if(key == SDLK_5&&keyOfMenu==-1)
                    {   
                        
                        keyOfMenu=5;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                        SDL_RenderClear(renderer);
                        SDL_RenderPresent(renderer);
                         //初始化
                         // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);
                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        

                        
                        printTextSize(1210,50,40,"Search Node",35);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer,textBox);
                        SDL_RenderPresent(renderer);
                        // search node
                        
                        break;  
                    }
                    else if(key == SDLK_6&&keyOfMenu==-1)
                    {
                        keyOfMenu=6;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);
                        SDL_RenderPresent(renderer);

                        // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);

                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList,bounding, validNode);
                        printTextSize(1210,50,35,"Add Link",35);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer,textBox);

                        
                        printf("ch0\n");
                        SDL_RenderPresent(renderer);
                        break;
                        // Add Link
                    }else if(key == SDLK_ESCAPE){
                        if(keyOfMenu==4||keyOfMenu==5||keyOfMenu==6){
                            isFirst=true;
                        }
                        keyOfMenu=-1;
                        n=0;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);
                        // initSDL(nodeNum, graphicPoints, adjList, rawNodeList,geomList,geomNum, bounding, validNode);

                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList,bounding, validNode);
                        printMenu();
                        SDL_RenderPresent(renderer);
                        break;
                        
                    }else if(key == SDLK_RETURN){
                        
                        
                        if(keyOfMenu==3&&(!stopSelect)){
                            //================================================================================================================
                            int pd[n][nodeNum];
                            
                            for(int j=0;j<n-1;j++){//n-1段小path
                                dijk(adjList, oid[j], pd[j],dist);
                                if(dist[oid[j+1]]>1e10){
                                    printf("no link or way !!!\n");
                                    break;
                                }else{
                                    tmpDist3+=dist[oid[j+1]];
                                    backtrackPath(pd[j],pathList,oid[j],oid[j+1],nodeNum);
                                    for(int k=0;k<pathList->pathNum-1;k++){
                                        
                                        totalNum++;
                                        totalPaths[totalNum-1]=pathList->path[k];
                                        
                                    }
                                }
                                if(j==n-2){
                                            totalNum++;
                                            totalPaths[totalNum-1]=oid[j+1];
                                        }

                                
                            }
                            totalPathList->pathNum=totalNum;

                            totalPathList->path=(int*)malloc(sizeof(totalNum));
                            for(int k=0;k<totalNum;k++){
                                totalPathList->path[k]=totalPaths[k];
                            }
                            pathPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect)*totalPathList->pathNum);
                            showTaskPath(totalPathList, rawNodeList, bounding, pathNodeSize, validPathNode);
                            totalNum=0;
                            for(int m=0;m<500;m++)
                                totalPaths[m]=-1;
                            n=0;
                            // if(nid[1-n]!=-1&&nid[n]!=-1&&n==0){
                            //     int* pd=(int*)calloc(nodeNum,sizeof(int));
                            //     dijk(adjListTime,nid[1-n],pd, dist);
                                
                            //     if(dist[nid[n]]>1e10){
                            //         printf("no link or way !!!\n");
                            //         printText(1300,120,20,"no link or way !!!");
                            //         SDL_RenderPresent(renderer);
                            //         nid[0]=-1;
                            //         nid[1]=-1;
                            //         n=0;
                            //         h_tmp=300;
                            //         break;
                            //     }
                                //  backtrackPath(pd, pathList, nid[1-n], nid[n], nodeNum);
                            //     pathPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect)*pathList->pathNum);
                                // char text[7][200];
                                
                                // sprintf(text[0], "Click at start pos (id = %d)", find_key_by_value(pairs2, nodeNum,nid[1-n]));
                                // sprintf(text[1], " (%lf, %lf)", rawNodeList[nid[1-n]].lon, rawNodeList[nid[1-n]].lat);
                                // sprintf(text[2], " (%d, %d)", graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
                                // sprintf(text[3], "Click at end pos (id = %d)", find_key_by_value(pairs2, nodeNum,nid[n]));
                                // sprintf(text[4], " (%lf, %lf)", rawNodeList[nid[n]].lon, rawNodeList[nid[n]].lat);
                                // sprintf(text[5], " (%d, %d)", graphicPoints[nid[n]].x, graphicPoints[nid[n]].y);
                                // sprintf(text[6], " Min Time=%lf\n", dist[nid[n]]);
                                
                                
                                
                                // if(!isPrint)
                                // {
                                //     printText(1300,h_tmp,20,text[0]);
                                //     h_tmp+=20;
                                //     printf("%d\n",h_tmp);
                                    
                                //     printText(1300,h_tmp,20,text[1]);
                                //     h_tmp+=20;
                                //     printf("%d\n",h_tmp);
                                    

                                //     printText(1300,h_tmp,20,text[2]);
                                //     h_tmp+=20;
                                //     printf("%d\n",h_tmp);

                                //     printText(1300,h_tmp,20,text[3]);
                                //     h_tmp+=20;
                                //     printf("%d\n",h_tmp);

                                //     printText(1300,h_tmp,20,text[4]);
                                //     h_tmp+=20;
                                //     printf("%d\n",h_tmp);

                                //     printText(1300,h_tmp,20,text[5]);
                                //     h_tmp+=20;
                                //     printf("%d\n",h_tmp);

                                //     printText(1300,h_tmp,20,text[6]);
                                //     h_tmp+=20;
                                //     printf("%d\n",h_tmp);

                                //     isPrint=true;
                                // }else{
                                //     ;
                                // }
                                // printf("min time=%lf\n", dist[nid[n]]);
                                // showTaskPath(pathList, rawNodeList, bounding, pathNodeSize, validPathNode);
                                
                                // canClear=1;
                            // }
                            //================================================================================================================
                        }
                        else if(keyOfMenu==4){
                                                   
                                int mode=-1;
                                double changeVal=0;
                                int check=sscanf(text4, "%d %lf",&mode, &changeVal);
                                // printf("整数: %d\n", mode);
                                // printf("浮点数: %lf\n", changeVal);
                                // printf("%d\n",check);
                                if(check==2&&oid[0]!=-1&&oid[1]!=-1){
                                    // printf("%d\noid[0]=%d/%d \n oid[1]=%d/%d\n",check,oid[0],find_key_by_value(pairs2, nodeNum, oid[0]),oid[1],find_key_by_value(pairs2, nodeNum, oid[1]));
                                    int linkId=findLink(oid[0], oid[1],rawEdgeList,linkNum);
                                    if (linkId!=-1){
                                        if(editLinkVal(mode,rawEdgeList,changeVal,linkId)==0)
                                        {
                                            if(mode!=0){
                                                
                                                
                                                printf("Edit Successfully! Try to restart and reload map file now.\n");
                                            }
                                        }else{
                                            printf("bad mode number\n");
                                        }
                                            
                                        
                                        // printf();
                                    }
                                    else{
                                        printf("No Related Link found\n");
                                    }

                                }else{
                                    printf("please click 2 points and enter 2 numbers spt by a space!");
                                }
                                // n=0;
                                // oid[0]=-1;
                                // oid[1]=-1;
                            // }else if ( key == SDLK_e){
                                
                            // }
                            

                            // SDL_RenderPresent(renderer);
                        }else if(keyOfMenu==5){
                            int mode=-1;
                            double a=-1, b=-1;
                            int nodeId=-1;
                            int checkSearch=-1; 
                            int check=sscanf(text5, "%d %lf %lf", &mode, &a, &b);
                            printf("%d %lf %lf\n", mode, a, b);
                            if (check==3){
                                
                                if (mode==1 || mode==2){
                                    // mode1: a=lon, b=lat
                                    // mode2: a=0, b= rawId
                                    

                                    checkSearch=searchNode(mode,rawNodeList,a,b,&nodeId,pairs,nodeNum);
                                    printf("%d",checkSearch);
                                }else{
                                    fprintf(stderr,"Bad argument input!\n");
                                    
                                }
                            }else{
                                fprintf(stderr,"Bad argument!\n");
                                
                            }
                            
                            if(nodeId!=-1&&checkSearch==0){
                                

                                SDL_Rect arr5;
                                arr5.w = pathNodeSize;
                                arr5.h = pathNodeSize;
                                arr5.x = graphicPoints[nodeId].x;
                                arr5.y = graphicPoints[nodeId].y;
                                SDL_RenderDrawRect(renderer,&arr5);
                                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                                SDL_RenderFillRect(renderer,&arr5);
                                SDL_RenderPresent(renderer);
                                printf("At pos(id = %d / %d): (%d, %d)\n",
                                        nodeId,find_key_by_value(pairs2, nodeNum, nodeId), 
                                        graphicPoints[nodeId].x, 
                                        graphicPoints[nodeId].y);

                            }

                           
                        }else if(keyOfMenu==6){
                            int addId=-1, addWay=-1;
                            double addLen=-1;
                            char addPoi[20]={};
                            
                            int check=sscanf(text6, "%d %d %lf %s", &addId, &addWay, &addLen, addPoi);
                                printf("%d\n整数: %d %d\n",check, addId, addWay);
                                printf("浮点数: %lf\n", addLen);
                                printf("poi= %s\n",addPoi);

                            if(check==4&&oid[0]!=-1&&oid[1]!=-1){
                                printf("ch4\n");
// int addLink(RawEdge* rawEdgeForAdd,RawEdge* rawEdgeList, int addId, int addNodeId1, int addNodeId2, int addWay, double addLen, char* addPoi, bool* isAddLink){

                                addLink(rawEdgeForAdd,rawEdgeList, addId,oid[0],oid[1] ,addWay, addLen, addPoi, &isAddLink);

                            }
                            //     // printf("%d\n",check);
                            //     if(check==2&&oid[0]!=-1&&oid[1]!=-1){
                            //         // printf("%d\noid[0]=%d/%d \n oid[1]=%d/%d\n",check,oid[0],find_key_by_value(pairs2, nodeNum, oid[0]),oid[1],find_key_by_value(pairs2, nodeNum, oid[1]));
                            //         int linkId=findLink(oid[0], oid[1],rawEdgeList,linkNum);
                            //         if (linkId!=-1){
                            //             if(editLinkVal(mode,rawEdgeList,changeVal,linkId)==0)
                            //             {
                            //                 if(mode!=0){
                                                
                                                
                            //                     printf("Edit Successfully! Try to restart and reload map file now.\n");
                            //                 }
                            //             }else{
                            //                 printf("bad mode number\n");
                            //             }
                                            
                                        
                            //             // printf();
                            //         }
                            //         else{
                            //             printf("No Related Link found\n");
                            //         }

                            //     }else{
                            //         printf("please click 2 points and enter 2 numbers spt by a space!");
                            //     }
                                n=0;
                                oid[0]=-1;
                                oid[1]=-1;
                                // rawEdgeForAdd[0].id=0;
                                // rawEdgeForAdd[0].len=0;
                                // raw
                            // // }else if ( key == SDLK_e){
                                
                            // // }
                            

                            // // SDL_RenderPresent(renderer);
                        }
                    }
                    else if(key == SDLK_LCTRL){
                        // Menu 4 only
                        if (keyOfMenu==4){
                            char newMapFileName[40]={0};
                            sprintf(newMapFileName,"newLeeds%d.map",newMapNum);

                            writeEditedMap(
                                newMapFileName,
                                bounding,
                                rawEdgeList,
                                rawNodeList,
                                wayList,
                                geomList,
                                linkNum,
                                nodeNum,
                                wayNum,
                                geomNum
                            );

                            newMapNum++;
                        }
                    }
                    

                    break;
                


                    // else if (key == SDLK_RETURN)
                    // {
                    //     SDL_StopTextInput();
                    //     int speed = atoi(text);
                    //     editEdge(speed, allNodes, shortest_path, G, Edge);
                    //     printf("%d\n", speed);
                    //     // for(int i = 0; i <G->edge_num; i++){
                    //     // if(Edge[i].speed != 0){
                    //     //     printf("%d %d\n",Edge[i].edge_id,Edge[i].speed);
                    //     // }
                    //     // }
                    //     deletePath(&necPathNodeIndex, &allNodes, shortest_path, &shortest_distance, pathPoints);
                    // }
                    // else if(key == SDLK_LALT)
                    // {
                    //     isKeyDown = 1;
                    // }
                    // else if(key  == SDLK_LCTRL){
                    //     isKeyDown = 2;
                    // }
                    // else if (key == SDLK_s && isKeyDown == 2)
                    // {
                    //     // printf("!!!");
                    //     writeIntoFile("leeds2.txt",G,Vertex,Edge,Geom,Way);
                    // }
                    break;
                }
                case SDL_TEXTINPUT:
                {   
                    if(keyOfMenu==4){
                        if (((strcmp(ev.text.text, "4") == 0||clearByRight))&& (isFirst)){
                            isFirst=false;
                            memset(text4, 0, sizeof(text4));
                        }
                        else
                        {
                            strcat(text4, ev.text.text);
                            printTextSize(1220,100,30,text4,30);
                            printf("%s\n",text4);
                            SDL_RenderPresent(renderer);
                        }
                        
                        
                    
                        
                        
                        
                        
                    }else if(keyOfMenu==5){
                        if (((strcmp(ev.text.text, "5")||clearByRight) == 0)&& (isFirst)){
                            isFirst=false;
                            memset(text5, 0, sizeof(text5));
                        }
                        else
                        {
                            strcat(text5, ev.text.text);
                            printTextSize(1220,100,25,text5,25);
                            printf("%s\n",text5);
                            SDL_RenderPresent(renderer);
                        }
                    }else if(keyOfMenu==6){
                        if (((strcmp(ev.text.text, "6") == 0||clearByRight))&& (isFirst)){
                            isFirst=false;
                            memset(text6, 0, sizeof(text6));
                        }
                        else
                        {
                            strcat(text6, ev.text.text);
                            printTextSize(1220,100,25,text6,25);
                            printf("%s\n",text6);
                            SDL_RenderPresent(renderer);
                        }
                    }
                    
                    break;
                }
                
            }

        }
    }
  
            

    

    
    return 0;


}