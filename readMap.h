#ifndef readMap_h
#define readMap_h

#include <stdio.h>
int getNodeNum(char* mapFileName);
int getLinkNum(char* mapFileName);
int extractData(FILE* fp);
#endif