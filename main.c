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
    printText(1250, 40, 20, "Menu for route planning:");
    printText(1300, 100, 20, "1.Min Distance");
    printText(1300, 200, 20, "2.Min Time");
    printText(1300, 300, 20, "3.Pass Ordered Nodes");
    printText(1300, 400, 20, "4.Edit Node");
    printText(1300, 500, 20, "5.Info of Link");
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
        if((rawEdgeList[i].newNode1==start&&rawEdgeList[i].newNode2==end)
        ||(rawEdgeList[i].newNode1==start&&rawEdgeList[i].newNode2==end)){
            return rawEdgeList[i].id;
        }
    }
    return -1;
}

void initSDL(int nodeNum, SDL_Rect* graphicPoints, AdjList* adjList,RawNode* rawNodeList,double* bounding, bool* validNode)    
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

    

    

    textBox = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    textBox->h=100;
    textBox->w=360;
    textBox->x=1220;
    textBox->y=100;
    SDL_Event ev;
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
    // char text4cp[50]={};
    // int textPos=-1;
    bool isFirst=true;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    //初始化
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
                                    printText(1300,120,20,"no link or way !!!");
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
                                sprintf(text[1], " (%lf, %lf)", rawNodeList[nid[1-n]].lon, rawNodeList[nid[1-n]].lat);
                                sprintf(text[2], " (%d, %d)", graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
                                sprintf(text[3], "Click at end pos (id = %d/%d)",nid[n], find_key_by_value(pairs2, nodeNum,nid[n]));
                                sprintf(text[4], " (%lf, %lf)", rawNodeList[nid[n]].lon, rawNodeList[nid[n]].lat);
                                sprintf(text[5], " (%d, %d)", graphicPoints[nid[n]].x, graphicPoints[nid[n]].y);
                                sprintf(text[6], " min dist=%lf\n", dist[nid[n]]);
                                
                                
                                
                                if(!isPrint)
                                {
                                    printText(1300,h_tmp,20,text[0]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);
                                    
                                    printText(1300,h_tmp,20,text[1]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);
                                    

                                    printText(1300,h_tmp,20,text[2]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    printText(1300,h_tmp,20,text[3]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    printText(1300,h_tmp,20,text[4]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    printText(1300,h_tmp,20,text[5]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    printText(1300,h_tmp,20,text[6]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);
                                    isPrint=true;
                                }else{
                                    ;
                                }
                                printf("min dist=%lf\n", dist[nid[n]]);
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
                            initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                            printText(1250,50,20,"Planning for Min Distance");
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
                                    printText(1300,120,20,"no link or way !!!");
                                    SDL_RenderPresent(renderer);
                                    nid[0]=-1;
                                    nid[1]=-1;
                                    n=0;
                                    h_tmp=300;
                                    break;
                                }
                                backtrackPath(pd, pathList, nid[1-n], nid[n], nodeNum);
                                pathPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect)*pathList->pathNum);
                                char text[7][200];
                                
                                sprintf(text[0], "Click at start pos (id = %d/%d)", nid[1-n],find_key_by_value(pairs2, nodeNum,nid[1-n]));
                                sprintf(text[1], " (%lf, %lf)", rawNodeList[nid[1-n]].lon, rawNodeList[nid[1-n]].lat);
                                sprintf(text[2], " (%d, %d)", graphicPoints[nid[1-n]].x, graphicPoints[nid[1-n]].y);
                                sprintf(text[3], "Click at end pos (id = %d/%d)", nid[n], find_key_by_value(pairs2, nodeNum,nid[n]));
                                sprintf(text[4], " (%lf, %lf)", rawNodeList[nid[n]].lon, rawNodeList[nid[n]].lat);
                                sprintf(text[5], " (%d, %d)", graphicPoints[nid[n]].x, graphicPoints[nid[n]].y);
                                sprintf(text[6], " Min Time=%lf\n", dist[nid[n]]);
                                
                                
                                
                                if(!isPrint)
                                {
                                    printText(1300,h_tmp,20,text[0]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);
                                    
                                    printText(1300,h_tmp,20,text[1]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);
                                    

                                    printText(1300,h_tmp,20,text[2]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    printText(1300,h_tmp,20,text[3]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    printText(1300,h_tmp,20,text[4]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    printText(1300,h_tmp,20,text[5]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    printText(1300,h_tmp,20,text[6]);
                                    h_tmp+=20;
                                    printf("%d\n",h_tmp);

                                    isPrint=true;
                                }else{
                                    ;
                                }
                                printf("min time=%lf\n", dist[nid[n]]);
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
                            initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                            printText(1250,50,20,"Planning for Min time");
                            h_tmp=300;
                            SDL_RenderPresent(renderer);
                            n=(n+1)%2;
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
                            initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);

                            printText(1250,50,20,"Planning for Min Distance");
                            printText(1250,70,20,"(Passing ordered points)");

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
                        if(stopSelect){

                        }
                        // SDL_RenderPresent(renderer);
                        break;
                        }
                        case 4:
                        {   
                            SDL_Point pos={ev.button.x, ev.button.y};
                            if(SDL_PointInRect(&pos, textBox)){
                                
                                
                                SDL_StartTextInput();
                                // SDL_RenderPresent(renderer);

                            }else{
                                SDL_StopTextInput();
                                
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
                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        keyOfMenu=1;
                        printText(1250,50,20,"Planning for Min Distance");
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
                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        keyOfMenu=2;
                        printText(1250,50,20,"Planning for Min Time");
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
                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        keyOfMenu=3;
                        printText(1250,50,20,"Planning for Min Distance");
                        printText(1250,70,20,"(passing order points)");

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
                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        printText(1250,50,20,"Edit Node");
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

                        // add attribute (speed）
                        break;  
                    }
                    else if(key == SDLK_6&&keyOfMenu==-1)
                    {
                        keyOfMenu=6;

                        // edit exist attribute
                    }else if(key == SDLK_ESCAPE){
                        if(keyOfMenu==4){
                            isFirst=true;
                        }
                        keyOfMenu=-1;
                        
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);
                        initSDL(nodeNum, graphicPoints, adjList, rawNodeList, bounding, validNode);
                        printMenu();
                        SDL_RenderPresent(renderer);
                        break;
                        
                    }else if(key == SDLK_RETURN){
                        // keyOfMenu=10;
                        
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
                        // printf("%s%s%s%s\n",text4[0],text4[1],text4[2],text4[3]);

                            // if(text4[0]!='\0'){
                                // printTextSize(1220,100,20,text4,30);

                            // }
                            
                            

                            // SDL_RenderPresent(renderer);
                        }
                    }else if (keyOfMenu == 4 && key==SDLK_BACKSPACE)
                    {
                        // 删除 text4 字符串的最后一个字符
                        if (strlen(text4) > 0)
                        {
                            text4[strlen(text4) - 1] = '\0';
                        }
                        // printTextSize(1220, 100, 20, text4, 30);
                        printf("%s\n", text4);
                        // SDL_RenderPresent(renderer);
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
                        if ((strcmp(ev.text.text, "4") == 0)&& (isFirst)){
                            isFirst=false;
                            memset(text4, 0, sizeof(text4));
                        }
                        else
                        {
                            strcat(text4, ev.text.text);
                            printTextSize(1220,100,20,text4,30);
                            printf("%s\n",text4);
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