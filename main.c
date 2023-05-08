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

// int main() {
//     char* mapFileName = "oxford.osm";
//     RawNode* rawNodeList = getRawNodeList(mapFileName);
//     SDL(mapFileName, rawNodeList);
//     freeRawNodeList(rawNodeList);
//     return 0;
// }




int main(int argc, char* argv[]) {
    // int linkNum = 6;
    // AdjList* adjList = createAdjList(linkNum);
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

    // int nodeNum=getNodeNum(mapFileName);
    // int linkNum=getLinkNum(mapFileName);
    // printf("%d\n%d\n",nodeNum,linkNum);
    // char* mapFileName="leeds.map";
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

    RawNode* rawNodeList=(RawNode*)malloc(nodeNum*sizeof(RawNode));
    
    getRawNode(mapFileName, rawNodeList, nodeNum, pairs, pairs2);
    // printf("\n node:\n");
    // printRawNodeList(rawNodeList, nodeNum);
    // printPairs(pairs,nodeNum);

    // sort by original id for quicker searching
    qsort(pairs, nodeNum, sizeof(Pair), compare_pair_key);

    // printPairs(pairs, nodeNum);
    puts("");
    // printPairs(pairs2, nodeNum);

    RawEdge* rawEdgeList=(RawEdge*)malloc(linkNum*sizeof(RawEdge));
    getRawLink(mapFileName, rawEdgeList, linkNum, pairs, nodeNum);
    // printRawEdgeList(rawEdgeList, linkNum);
    
    for (int i=0;i<linkNum;i++){
        // printf("node1: %d node2: %d len: %lf\n",rawEdgeList[i].newNode1,rawEdgeList[i].newNode2, rawEdgeList[i].len);
        addEdge(adjList, rawEdgeList[i].newNode1, rawEdgeList[i].newNode2, rawEdgeList[i].len);
        addEdge(adjList, rawEdgeList[i].newNode2, rawEdgeList[i].newNode1, rawEdgeList[i].len);
        // printf("%d\n",i);
    }


    // Example: addEdge(adjList, start, dest, length)
    // addEdge(adjList,0,1,2);
    // addEdge(adjList,1,0,2);
    
    // printAdjList(adjList);


    int *pd=(int*)malloc(sizeof(int)*nodeNum);
    PathList *pathList=(PathList*)malloc(sizeof(PathList));
    dijk(adjList, 0, pairs2, pd);
    // printf("\nlineNum: %d\n",adjList->lineNum);
    
    // for(int i=0;i<adjList->lineNum;i++){
    //     printf("new id: %d original id: %d\n", i, find_key_by_value(pairs2, nodeNum, i));
    //     dijk(adjList, i, pairs2);
    // }
    backtrackPath(pd,pathList, 0, 3,nodeNum);
    
    SDL(mapFileName,rawNodeList,bounding, adjList,pathList);




}