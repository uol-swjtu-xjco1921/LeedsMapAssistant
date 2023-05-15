#include <stdlib.h>
#include <stdio.h>
// the head file dealing with exits with error
#include "ErrorExit.h"

char *errorCandidateStrings[NUM_ERROR_STATUS]=
    {
        "SUCCESS: No Error\n",
        "ERROR: BAD Argument Count\n",
        "ERROR: Bad File Name (%s)\n",
        "ERROR: Bad Magic Number (%s)\n",
        "ERROR: Bad Comment Line (%s)\n",
        "ERROR: Bad Dimensions (%s)\n",
        "ERROR: Bad Max Gray Value (%s)\n",
        "ERROR: Image Malloc Failed\n",
        "ERROR: Bad Data (%s)\n",
        "ERROR: Output Failed (%s)\n",
        "ERROR: Miscellaneous (%s)\n",
        "ERROR: Unknown (%s)\n"
    };
void ErrorExit(int errorCode, char *errStringInsert){
    if((errorCode<0) || (errorCode>=NUM_ERROR_STATUS)){
        errorCode=ERROR_UNKNOWN-1;
    }
    
    if(errStringInsert==NULL){
        printf(errorCandidateStrings[NUM_ERROR_STATUS],"Bad Error String!");
    }
    printf(errorCandidateStrings[errorCode],errStringInsert);
    
    exit(errorCode);
}