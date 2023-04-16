#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "routeplan.h"

int getNodeNum(char* mapFileName){
    int nodeNum=0;
    char str[200];
    FILE *fp=fopen(mapFileName,"r");
    while (fgets(str,200, fp) != NULL) {
        if (strstr(str, "<node id=") != NULL) {
            nodeNum++;
        }
    }
    return nodeNum;
}
int getLinkNum(char* mapFileName){
    int linkNum=0;
    char str[200];
    FILE *fp=fopen(mapFileName,"r");
    while (fgets(str,200, fp) != NULL) {
        if (strstr(str, "<link id=") != NULL) {
            linkNum++;
        }
    }
    return linkNum;
}

// int main(){
//     printf("%d\n",getNodeNum("myMap.map"));
//     printf("%d\n",getLinkNum("myMap.map"));
// }
