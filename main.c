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
#define Y_SIZE 760
#define RADIO 0.8


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
    dijk(adjList, 0, pd);
    // printf("\nlineNum: %d\n",adjList->lineNum);
    
    // for(int i=0;i<adjList->lineNum;i++){
    //     printf("new id: %d original id: %d\n", i, find_key_by_value(pairs2, nodeNum, i));
    //     dijk(adjList, i, pairs2);
    // }
    backtrackPath(pd,pathList, 0, 100,nodeNum);
    
    mySdl(mapFileName,rawNodeList,bounding, adjList,pathList);

    return 0;


}