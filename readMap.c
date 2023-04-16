#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "routeplan.h"

int getNodeNum(char* mapFileName){
    int nodeNum=0;
    char str[500];
    FILE *fp=fopen(mapFileName,"r");
    while (fgets(str,500, fp) != NULL) {
        if (strstr(str, "<node id=") != NULL) {
            nodeNum++;
        }
    }
    return nodeNum;
}
int getLinkNum(char* mapFileName){
    int linkNum=0;
    char str[500];
    FILE *fp=fopen(mapFileName,"r");
    while (fgets(str, 500, fp) != NULL) {
        if (strstr(str, "<link id=") != NULL) {
            linkNum++;
        }
    }
    return linkNum;
}
int extractData(FILE* fp){
    // Read the map once a line
    char str[500];
    while (fgets(str, sizeof(str), fp)) {
        // get newline without "\n" from the buffer string 
        char *p = str;
        char *q = str;
        while (*p != '\0') {
            if (*p != '\n') {
                *q++ = *p;
            }
            p++;
        }
        *q = '\0';

        // Parse the link element if it exists
        if (strncmp(str, "<link", 5) == 0) {
            char id[20], node1[20], node2[20], way[20], poi[20];
            double len, veg, arch, land;
            int count = sscanf(str, "<link id=%s node=%s node=%s way=%s length=%lf veg=%lf arch=%lf land=%lf POI=%s;/link>",
                               id, node1, node2, way, &len, &veg, &arch, &land, poi);

            if (count == 9) {
                printf("Link id: %s\n", id);
                printf("Node 1: %s\n", node1);
                printf("Node 2: %s\n", node2);
                printf("Way: %s\n", way);
                printf("Length: %lf\n", len);
                printf("Veg: %lf\n", veg);
                printf("Arch: %lf\n", arch);
                printf("Land: %lf\n", land);
                printf("POI: %s\n\n", poi);

            } else {
                printf("Failed to parse the link element.\n");
                
            }
        }
        if(strncmp(str, "<node", 5) == 0) {
             char id[20];
             double lat,lon;
             int count = sscanf(str, "<node id=%s lat=%lf lon=%lf /link>",
                               id, &lat, &lon);
            if(count==3){
                printf("Node id: %s\n", id);
                printf("lat: %lf\n", lat);
                printf("lon: %lf\n\n", lon);

            }else {
                printf("Failed to parse the node element.\n");
                
            }
         }
    }
    fclose(fp);
    return 0;
}
/*
int main(){
    FILE *fp = fopen("leeds.map", "r");
    if (fp == NULL) {
        fprintf(stderr, "File not found\n");
        return -1;
    }
    //     printf("%d\n",getNodeNum("myMap.map"));
    //     printf("%d\n",getLinkNum("myMap.map"));
    extractData(fp);

    return 0;
}*/
