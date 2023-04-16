#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){
    char* mapFileName="";
    if (argc==1){
        printf("usage: ./main mapFile\n");
    }else if(argc==2){
        mapFileName=argv[1];
        printf("using %s for route planning\n",mapFileName);
    }else {
        printf("error arguments count\n");
    }
}