#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "routeplan.h"

int main(int argc, char* argv[]){

    char* mapFileName="";
    if (argc==1){
        printf("usage: ./main mapFile\n");
        return 0; //functionally succeed but remind how to use 
    }else if(argc==2){
        mapFileName=argv[1];
        printf("using %s for route planning\n",mapFileName);
    }else {
        printf("error arguments count\n");
        exit(1); //error case 1: bad input arguments,
    }
    FILE *fp=fopen(mapFileName,"r");
    if (fp==NULL){
        fprintf(stderr, "errors exist in openning map file %s\n", mapFileName);
        exit(2); //error case 2: bad map file reading,
    }
    int nodeNum=getNodeNum(mapFileName);
    int linkNum=getLinkNum(mapFileName);
    // printf("%d\n%d\n",nodeNum,linkNum);
}