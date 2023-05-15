#include <stdlib.h>
#include <stdio.h>
#include "errorExit.h"

void errorExit(int errorCode) {
    switch (errorCode) {
        case 1:
            fprintf(stderr, "Bad argument count\n");
            break;
        case 2:
            fprintf(stderr, "ERROR: Errors exist in opening map file\n");
            break;
        case 3:
            fprintf(stderr, "ERROR: NodeNum=0\n");
            break;
        case 4:
            fprintf(stderr, "ERROR: LinkNum=0\n");
            break;
        case 5:
            fprintf(stderr, "ERROR: Bounding not found\n");
            break;
        case 6:
            fprintf(stderr, "ERROR: =0\n");
            break;
        default:
            fprintf(stderr, "ERROR: Unknown error\n");
            break;
    }

    exit(errorCode);
}
