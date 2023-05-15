#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
/* my header files */
#include "readMap.h"
#define DELTA 1e-7
#define DBL_MAX_ 1e15 

/* utilities
 *
 * int getRawNodeNum(char* mapFileName);
 * int getRawLinkNum(char* mapFileName);
 * int compare_pair_key(const void *a, const void *b);
 * int find_value_by_key(Pair* pairs, int n, int key);
 * int find_key_by_value(Pair* pairs2, int n, int value)
 * int getRawNode(char* mapFileName, RawNode* rawNodeList, int nodeNum, Pair* pairs, Pair* pairs2);
 * int getRawLink(char* mapFileName, RawEdge* rawEdgeList, int edgeNum, Pair* pairs, int nodeNum);
*/

/* for debugging
 *
 * void printRawEdgeList(RawEdge* rawEdgeList, int numEdges);
 * void printRawNodeList(RawNode* rawNodeList, int numNodes);
 * void printPairs(Pair* pairs, int numNodes);
*/
int getRawNodeNum(char* mapFileName) {
    int nodeNum=0;
    char str[500];
    FILE *fp=fopen(mapFileName, "r");
    while (fgets(str,500, fp) != NULL) {
        if (strstr(str, "<node id=") != NULL) {
            nodeNum++;
    
        }
    }
    fclose(fp);
    return nodeNum;
}

int getRawLinkNum(char* mapFileName){
    int linkNum=0;
    char str[500];
    FILE *fp=fopen(mapFileName, "r");
    while (fgets(str, 500, fp) != NULL) {
        if (strstr(str, "<link id=") != NULL) {
            linkNum++;
        }
    }
    fclose(fp);
    return linkNum;
}

int getRawWayNum(char* mapFileName){
    int wayNum=0;
    char str[500];
    FILE *fp=fopen(mapFileName, "r");
    while (fgets(str, 500, fp) != NULL) {
        if (strstr(str, "<way id=") != NULL) {
            wayNum++;
        }
    }
    fclose(fp);
    return wayNum;
}
// int getRawNode(char* mapFileName, RawNode* rawNodeList, int nodeNum, Pair* pairs, Pair* pairs2){
int getWayEachNodeNum(char* mapFileName, Way* wayList){
    FILE* fp=fopen(mapFileName,"r");
    char str[1000];
    int i=0;
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

        
        if(strncmp(str, "<way", 4) == 0) {
            wayList[i].nodeNum=0;
            char* token=strtok(str," ");
            token=strtok(NULL, " ");
            while(token!=NULL){
                token=strtok(NULL," ");
                wayList[i].nodeNum++;

            }
            wayList[i].nodeNum--;
            wayList[i].nodeNum--;
            // printf("!!\n");
            i++;
        }
        
    }
    return 0;
}

int getRawWay(char* mapFileName, Way* wayList, int wayNum, Pair* pairs,int nodeNum){
    for(int j=0;j<wayNum;j++){
        wayList[j].nodeList=(int*)malloc(sizeof(int)*wayList[j].nodeNum);
        wayList[j].newNodeList=(int*)malloc(sizeof(int)*wayList[j].nodeNum);
    } 
    FILE* fp=fopen(mapFileName,"r");
    char str[1000];
    int i=0;
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

        
        if(strncmp(str, "<way", 4) == 0) {
            char* token=strtok(str," ");
            token=strtok(NULL, " ");
            sscanf(token, "id=%d",&wayList[i].id);

            int j=0;
            while(j<wayList[i].nodeNum){
                token=strtok(NULL, " ");
                sscanf(token,"node=%d",&wayList[i].nodeList[j]);
                wayList[i].newNodeList[j]=find_value_by_key(pairs,nodeNum,wayList[i].nodeList[j]);
                j++;
            }
            i++;
        }
    }
    return 0;
}

int getRawGeomNum(char* mapFileName){
    int geomNum=0;
    char str[500];
    FILE *fp=fopen(mapFileName, "r");
    while (fgets(str, 500, fp) != NULL) {
        if (strstr(str, "<geom id=") != NULL) {
            geomNum++;
        }
    }
    fclose(fp);
    return geomNum;
}

int getGeomEachNodeNum(char* mapFileName, Geom* geomList){
    FILE* fp=fopen(mapFileName,"r");
    char str[1000];
    int i=0;
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

        
        if(strncmp(str, "<geom", 5) == 0) {
            geomList[i].nodeNum=0;
            char* token=strtok(str," ");
            token=strtok(NULL, " ");
            while(token!=NULL){
                token=strtok(NULL," ");
                geomList[i].nodeNum++;
                

            }
            geomList[i].nodeNum--;
            geomList[i].nodeNum--;
            

            // printf("!!\n");
            i++;
        }
        
    }
    return 0;
}

int getRawGeom(char* mapFileName, Geom* geomList, int geomNum, Pair* pairs,int nodeNum){
    for(int j=0;j<geomNum;j++){
        geomList[j].nodeList=(int*)malloc(sizeof(int)*geomList[j].nodeNum);
        geomList[j].newNodeList=(int*)malloc(sizeof(int)*geomList[j].nodeNum);
    } 
    FILE* fp=fopen(mapFileName,"r");
    char str[1000];
    int i=0;
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

        
        if(strncmp(str, "<geom", 5) == 0) {
            char* token=strtok(str," ");
            token=strtok(NULL, " ");
            sscanf(token, "id=%d",&geomList[i].id);

            int j=0;
            while(j<geomList[i].nodeNum){
                token=strtok(NULL, " ");
                sscanf(token,"node=%d",&geomList[i].nodeList[j]);
                geomList[i].newNodeList[j]=find_value_by_key(pairs,nodeNum,geomList[i].nodeList[j]);
                j++;
            }
            i++;
        }
    }
    return 0;
}
int getSpeedNum(char* mapFileName){
    int speedNum=0;
    char str[500];
    FILE *fp=fopen(mapFileName, "r");
    while (fgets(str,500, fp) != NULL) {
        if (strstr(str, "<spd") != NULL) {
            speedNum++;
    
        }
    }
    fclose(fp);
    return speedNum;
}



int compare_pair_key(const void *a, const void *b) {
    Pair *p1 = (Pair *)a;
    Pair *p2 = (Pair *)b;
    return p1->key - p2->key;
}

int find_value_by_key(Pair* pairs, int n, int key) {
    for(int i=0;i<n;i++){
        if(pairs[i].key==key)
            return pairs[i].value;
    }
    return -1; // not found
}

int find_key_by_value(Pair* pairs2, int n, int value) {
    for(int i=0;i<n;i++){
        if(pairs2[i].value==value)
            return pairs2[i].key;
    }
    return -1; //  not found
}

int getRawNode(char* mapFileName, RawNode* rawNodeList, int nodeNum, Pair* pairs, Pair* pairs2){

    int index=0;
    
    FILE* fp=fopen(mapFileName, "r");
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

        
        if(strncmp(str, "<node", 5) == 0) {
             int id;
             double lat,lon;
             int count = sscanf(str, "<node id=%i lat=%lf lon=%lf /node>",
                               &id, &lat, &lon);
            if(count==3){
                // printf("Node id: %d\n", id);
                // printf("lat: %lf\n", lat);
                // printf("lon: %lf\n\n", lon);
                rawNodeList[index].id=id;
                pairs[index].key=id;
                pairs2[index].key=id;
                rawNodeList[index].newId=index;
                pairs[index].value=index;
                pairs2[index].value=index;
                rawNodeList[index].lat=lat;
                rawNodeList[index].lon=lon;
                if(index<=nodeNum)
                    index++;
                
            }else {
                printf("Failed to parse the node element.\n");
                exit(6);
            }
         }
    }
    fclose(fp);
    return 0;
}

int getRawBound(char* mapFileName, double *bounding){
    
    FILE* fp=fopen(mapFileName, "r");
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
        
        
        if(strncmp(str, "<bounding", 9) == 0) {
            
            double minLat,minLon,maxLon,maxLat;
            int count = sscanf(str, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>",
                   &minLat, &minLon, &maxLat, &maxLon);
            if(count==4){
                bounding[0]=minLat;
                // printf("min lat: %lf\n", minLat);
                bounding[1]=minLon;
                // printf("min lat: %lf\n", minLon);
                bounding[2]=maxLat;
                // printf("min lat: %lf\n", minLat);
                bounding[3]=maxLon;
                // printf("min lat: %lf\n", minLon);                
            }else {
                printf("Failed to parse the bounding element.\n");
                exit(60);
            }
         }
    }
    fclose(fp);
    return 0;
}
//  int n = sscanf(xml, "<bounding minLat=\"%lf\" minLon=\"%lf\" maxLat=\"%lf\" maxLon=\"%lf\"/>",
//                    &bounding.minLat, &bounding.minLon, &bounding.maxLat, &bounding.maxLon);
int getRawLink(char* mapFileName, RawEdge* rawEdgeList, int edgeNum, Pair* pairs, int nodeNum){

    int index=0;
    
    FILE* fp=fopen(mapFileName, "r");
    // Read the map once a line
    char str[1000];
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
            int id, node1, node2, way;
            char poi[30];
            double len, veg, arch, land;
            int count = sscanf(str, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf POI=%s;/link>",
                               &id, &node1, &node2, &way, &len, &veg, &arch, &land, poi);
            
            if (count == 9) {
                // printf("Link id: %d\n", id);
                rawEdgeList[index].id=id;
                rawEdgeList[index].newId=index;
                rawEdgeList[index].node1=node1;
                rawEdgeList[index].newNode1=find_value_by_key(pairs, nodeNum, node1);
                // printf("%d ",rawEdgeList[index].newNode1);
                rawEdgeList[index].node2=node2;
                rawEdgeList[index].newNode2=find_value_by_key(pairs, nodeNum, node2);
                // printf("%d\n",rawEdgeList[index].newNode2);
                rawEdgeList[index].len=len;
                rawEdgeList[index].way=way;
                rawEdgeList[index].veg=veg;
                rawEdgeList[index].arch=arch;
                rawEdgeList[index].land=land;
                if(len>60){
                    rawEdgeList[index].speed=30;
                }else if(len>20){
                    rawEdgeList[index].speed=20;
                }else{
                    rawEdgeList[index].speed=5;
                }
                rawEdgeList[index].isDefault=true;
                int poiLen = strlen(poi) + 1; 
                rawEdgeList[index].poi = (char*) malloc(poiLen * sizeof(char));
                strcpy(rawEdgeList[index].poi, poi);
                if(index<=edgeNum)
                    index++;
                // printRawEdgeList(rawEdgeList, index);        
            } else {
                printf("Failed to parse the link element%d.\n",index);
                exit(6);
            }
        }
        
    }
    fclose(fp);
    return 0;
}
// int getWayNum(char* mapFileName){

// }
// int getRawWay(char* mapFileName, Way* way, Pair* pairs, int nodeNum);

int getRawSpeed(char* mapFileName, Speed* speedList, int speedNum){
    int index=0;
    
    FILE* fp=fopen(mapFileName, "r");
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

        
        if(strncmp(str, "<spd", 4) == 0) {
             int linkId;
             double spd;
             int count = sscanf(str, "<spd linkId=%d speed=%lf /spd>",
                                            &linkId, &spd);
            if(count==2){
                
                speedList[index].linkId=linkId;
                speedList[index].value=spd;
                
                if(index<=speedNum)
                    index++;
                
            }else {
                printf("Failed to parse the node element.\n");
                exit(6);
            }
         }
    }
    fclose(fp);
    return 0;
}

void checkLinkByNode(RawEdge* rawEdgeList, RawNode* rawNodeList,int nodeNum,int linkNum,bool* validLink){
    
    for(int i; i<linkNum;i++){
        validLink[i]=false;
        for (int j; j<nodeNum; j++)
            if(rawEdgeList[i].node1==rawNodeList[j].id)
                validLink[i]=true;

    }
}


void printRawEdgeList(RawEdge* rawEdgeList, int numEdges) {
    for (int i = 0; i < numEdges; i++) {
        printf("id: %d, newId: %d, node1: %d, node2: %d, way: %d, len: %lf, veg: %lf, arch: %lf, land: %lf, poi: %s\n", 
            rawEdgeList[i].id, rawEdgeList[i].newId, rawEdgeList[i].node1, rawEdgeList[i].node2, rawEdgeList[i].way, 
            rawEdgeList[i].len, rawEdgeList[i].veg, rawEdgeList[i].arch, rawEdgeList[i].land, 
            rawEdgeList[i].poi);
    }
    puts("");
}

void printRawWayList(Way* wayList, int wayNum) {
    for (int i = 0; i < wayNum; i++) {
        printf("id: %d num=%d",
        wayList[i].id, wayList[i].nodeNum);
        for(int j=0;j<wayList[i].nodeNum;j++){
            printf("node%d=%d/%d ",j+1,wayList[i].newNodeList[j],wayList[i].nodeList[j]);

        }
        puts("");
    }
    puts("");
}

void printRawNodeList(RawNode* rawNodeList, int numNodes) {
    for (int i = 0; i < numNodes; i++) {
        printf("id: %d, newId: %d, lat: %lf, lon: %lf\n",
        rawNodeList[i].id, rawNodeList[i].newId, rawNodeList[i].lat, rawNodeList[i].lon);
    }
    puts("");
}

void printPairs(Pair* pairs, int numNodes){
    puts("");
    for(int i=0;i<numNodes;i++){
        printf("index: %d, key: %d, value: %d\n",i, pairs[i].key, pairs[i].value);
    }
    puts("");
}

