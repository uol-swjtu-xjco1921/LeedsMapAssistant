#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define DELTA 1e-7
#define DBL_MAX_ 1e15 
  
typedef struct _RawEdge { 
    // store the info of edges(links) in map assign the new id to edge's newId
    int id; // id in the 'link' label
    int newId; // assigned id for future usage
    int node1; // id of the start node
    int node2; // id of the end node
    int newNode1; // assigned and new id of the start node
    int newNode2; // assigned and new id of the end node
    int way; // way info in the 'link' label
    double len; // length value in the 'link' label
    double veg; // veg value in the 'link' label
    double arch; // arch value in the 'link' label
    double land; // land value in the 'link' label
    char* poi; // poi info in the 'link' label
}RawEdge;

typedef struct _RawNode { 
    //store the info of nodes in map and assign the new id to node's newId 
    int id; // id in the 'node' label
    int newId; // assigned and new id in the 'node' label
    double lat; //  lat value in the 'node' label
    double lon; //  lon value in the 'node' label
}RawNode;

typedef struct _Edge {
    int vertex; // the id of the node that this edge points at
    double len; //length or cost(i.e. time) of going through the edge
    struct _Edge* next; // link list of edges
} Edge;

typedef struct _AdjList {
    int lineNum; // number of lines(each node has a line) of adjacent list
    Edge** adjLines; // list to store each node's edges 
    // adjList with n nodes in the form like:
    // node  reachable nodes
    // 0   -->   1 2 3 
    // 1   -->   0 2 4 6 
    // 2   -->   x y ...
    // :   -->   : : : :
    // n-1 -->   a b c d
} AdjList;

typedef struct _Pair {
    // bridge the original id with new id 
    int key; // original id
    int value; // new id
    // related functions:
    // 1.find_value_by_key
    // 2.find_key_by_value
}Pair;

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

void printRawEdgeList(RawEdge* rawEdgeList, int numEdges) {
    for (int i = 0; i < numEdges; i++) {
        printf("id: %d, newId: %d, node1: %d, node2: %d, way: %d, len: %lf, veg: %lf, arch: %lf, land: %lf, poi: %s\n", 
            rawEdgeList[i].id, rawEdgeList[i].newId, rawEdgeList[i].node1, rawEdgeList[i].node2, rawEdgeList[i].way, 
            rawEdgeList[i].len, rawEdgeList[i].veg, rawEdgeList[i].arch, rawEdgeList[i].land, 
            rawEdgeList[i].poi);
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

int compare_pair_key(const void *a, const void *b) {
    Pair *p1 = (Pair *)a;
    Pair *p2 = (Pair *)b;
    return p1->key - p2->key;
}
//value has been ordered, it doesn't need sort or compare.  

int find_value_by_key(Pair* pairs, int n, int key) {
    // int left = 0;
    // int right = n - 1;
    // while (left <= right) {
    //     int mid = left + (right - left) / 2;
    //     if (pairs[mid].key == key) {
    //         return pairs[mid].value;
    //     } else if (pairs[mid].key < key) {
    //         left = mid + 1;
    //     } else {
    //         right = mid - 1;
    //     }
    // }
    for(int i=0;i<n;i++){
        if(pairs[i].key==key)
            return pairs[i].value;
    }
    return -1; // not found
}

int find_key_by_value(Pair* pairs2, int n, int value) {
    // int left = 0;
    // int right = n - 1;
    // while (left <= right) {
    //     int mid = left + (right - left) / 2;
    //     if (pairs2[mid].value == value) {
    //         return pairs2[mid].key;
    //     } else if (pairs2[mid].value < value) {
    //         left = mid + 1;
    //     } else {
    //         right = mid - 1;
    //     }
    // }
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
                index++;
                
            }else {
                printf("Failed to parse the node element.\n");
                
            }
         }
    }
    fclose(fp);
    return 0;
}

int getRawLink(char* mapFileName, RawEdge* rawEdgeList, int edgeNum, Pair* pairs, int nodeNum){

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
                printf("%d ",rawEdgeList[index].newNode1);
                rawEdgeList[index].node2=node2;
                rawEdgeList[index].newNode2=find_value_by_key(pairs, nodeNum, node2);
                printf("%d\n",rawEdgeList[index].newNode2);
                rawEdgeList[index].len=len;
                rawEdgeList[index].way=way;
                rawEdgeList[index].veg=veg;
                rawEdgeList[index].arch=arch;
                rawEdgeList[index].land=land;
                int poiLen = strlen(poi) + 1; 
                rawEdgeList[index].poi = (char*) malloc(poiLen * sizeof(char));
                strcpy(rawEdgeList[index].poi, poi);
                index++;
                // printRawEdgeList(rawEdgeList, index);        
            } else {
                printf("Failed to parse the link element.\n");
                
            }
        }
        
    }
    fclose(fp);
    return 0;
}

AdjList* initAdjList(int nodeNum) {
    // malloc new adjList and edges inside
    AdjList* adjList = (AdjList*)malloc(sizeof(AdjList));
    adjList->lineNum = nodeNum;
    adjList->adjLines = (Edge**)malloc(nodeNum * sizeof(Edge*));

    for (int i = 0; i < nodeNum; i++) {
        adjList->adjLines[i] = NULL;
    }

    return adjList;
}

void addEdge(AdjList* adjList, int start, int dest, double len) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->vertex = dest;
    // printf("%d ",edge->vertex);
    edge->len = len;
    edge->next = adjList->adjLines[start];
    
    adjList->adjLines[start] = edge;
    // printf("%d\n",adjList->lineNum);
}

void addTotalEdge(AdjList* adjList, RawEdge* rawEdgeList, int edgeNum){
    // quickly construct adjList
    for (int i=0;i<edgeNum;i++){
        // Example: addEdge(adjList, start, dest, length)
        // !Note: the edge here has the direction!
        // addEdge(adjList,0,1,2);
        addEdge(adjList, rawEdgeList[i].newNode1, rawEdgeList[i].newNode2, rawEdgeList[i].len);
        addEdge(adjList, rawEdgeList[i].newNode2, rawEdgeList[i].newNode1, rawEdgeList[i].len);
        
    }
}

int minDist(double *dist, bool *sptSet, int nodeNum) {
    // var to store the index of min dist
    int idMin;
    //init the address and value
    int n=0;
    bool* spt=sptSet;
    double* d=dist;
    // init the min dist
    double min = (double)DBL_MAX_;
    // compare 
    
    while(n<nodeNum){
        if (*spt==false && *d<min){
            idMin = n;
            min = *d;
        }
        // iterate and update
        n++;
        spt++;
        d++;
    }
    

    return idMin;
}

void printDist(double* dist, int nodeNum, Pair* pair2) {
    printf("new id\t    original id\t    min dist\n");
    for (int i = 0; i < nodeNum; i++){
        // if(fabs(DBL_MAX_-dist[i])>DELTA){
        //     printf("%d\t    %d\t   %lf\n", i, find_key_by_value(pair2, nodeNum, i), dist[i]);
        // }else{
        //     printf("%d\t    %d\t   Inf\n", i, find_key_by_value(pair2, nodeNum, i));
        // }
        printf("%d\t    %d\t   %lf\n", i, find_key_by_value(pair2, nodeNum, i), dist[i]);
    }
}

void dijk(AdjList* adjList, int start, Pair* pairs2) {
    // init dijkstra
    int lineNum = adjList->lineNum;
    bool* travelSet=(bool*)malloc(lineNum*sizeof(bool)); //present set of traveled points 
    double* dist=(double*)malloc(lineNum*sizeof(double)); //distance of each point
    
    for (int i = 0; i < lineNum; i++){
        travelSet[i] = false;
        dist[i] = DBL_MAX_;
    }
    dist[start] = 0;
    // perform dijk
    for (int count = 0; count < lineNum - 1; count++) {
        int idMin = minDist(dist, travelSet, lineNum);
        travelSet[idMin] = true;
        Edge* tmp;
        for (tmp = adjList->adjLines[idMin]; tmp != NULL; tmp = tmp->next) {
            double tmpLen = tmp->len;
            int tmpId = tmp->vertex;
            if ( fabs(DBL_MAX_-dist[idMin]) > DELTA && travelSet[tmpId]==false && dist[tmpId]-dist[idMin]-tmpLen > DELTA ) {
                dist[tmpId] = dist[idMin] + tmpLen;
            }
        }
    }

    printDist(dist, lineNum, pairs2);
}

int main() {
    // int linkNum = 6;
    // AdjList* adjList = createAdjList(linkNum);

    char* mapFileName="leeds.map";
    int linkNum=getRawLinkNum(mapFileName);
    int nodeNum=getRawNodeNum(mapFileName);
    Pair *pairs = (Pair *)malloc(nodeNum * sizeof(Pair));
    Pair *pairs2 = (Pair *)malloc(nodeNum * sizeof(Pair));

    AdjList* adjList = initAdjList(nodeNum);

    RawNode* rawNodeList=(RawNode*)malloc(nodeNum*sizeof(RawNode));
    getRawNode(mapFileName, rawNodeList, nodeNum, pairs, pairs2);

    // printRawNodeList(rawNodeList, nodeNum);
    // printPairs(pairs,nodeNum);

    // sort by original id for quicker searching
    qsort(pairs, nodeNum, sizeof(Pair), compare_pair_key);

    printPairs(pairs, nodeNum);
    puts("");
    printPairs(pairs2, nodeNum);

    RawEdge* rawEdgeList=(RawEdge*)malloc(linkNum*sizeof(RawEdge));
    getRawLink(mapFileName, rawEdgeList, linkNum, pairs, nodeNum);
    // printRawEdgeList(rawEdgeList, linkNum);
    
    for (int i=0;i<linkNum;i++){
        printf("node1: %d node2: %d len: %lf\n",rawEdgeList[i].newNode1,rawEdgeList[i].newNode2, rawEdgeList[i].len);
        addEdge(adjList, rawEdgeList[i].newNode1, rawEdgeList[i].newNode2, rawEdgeList[i].len);
        addEdge(adjList, rawEdgeList[i].newNode2, rawEdgeList[i].newNode1, rawEdgeList[i].len);
        printf("%d\n",i);
    }


    // Example: addEdge(adjList, start, dest, length)
    // addEdge(adjList,0,1,2);
    // addEdge(adjList,1,0,2);
    
    
    puts("");
    for(int i=0;i<adjList->lineNum;i++){
        Edge* tmp=adjList->adjLines[i];
        printf("%d --> ", i);
        while(tmp!=NULL){
            printf("%d ",tmp->vertex);
            tmp=tmp->next;
        }
        puts("");
    }
    dijk(adjList, 0, pairs2);
    printf("\nlineNum: %d\n",adjList->lineNum);
    // for(int i=0;i<adjList->lineNum;i++){
    //     printf("new id: %d original id: %d\n", i, find_key_by_value(pairs2, nodeNum, i));
    //     dijk(adjList, i, pairs2);
    // }
    
    return 0;
}