#include "edit.h"



int editLinkVal(int mode, RawEdge* rawEdgeList, double changeVal, int linkId){
    if (!(mode==1||mode==2||mode==3||mode==4||mode==5||mode==0)){
        fprintf(stderr, "Bad argument input!\n");
        return -10;
    }
    // FILE* fp=fopen("editLog.txt","a");
    int i=linkId;
    switch (mode)
    {   
        case 0:
        {   
            FILE* fp=fopen("editLog.txt","w");
            // printf("clear editLog.txt\n");
            fprintf(fp,"clear editLog.txt. Done!\n");
            fclose(fp);
            break;
        }
        case 1:
        {   
            FILE* fp=fopen("editLog.txt","a");
            printf("link[%d](%d/%d<->%d/%d).veg from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].veg, changeVal);
            fprintf(fp,"link[%d](%d/%d<->%d/%d).veg from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].veg, changeVal);
            rawEdgeList[i].veg=changeVal;  
            fclose(fp);
            break;
        }
        case 2:
        {
            FILE* fp=fopen("editLog.txt","a");
            printf("link[%d](%d/%d<->%d/%d).arch from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].arch, changeVal);
            fprintf(fp,"link[%d](%d/%d<->%d/%d).arch from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].arch, changeVal);
            rawEdgeList[i].arch=changeVal;
            fclose(fp);
            break;
        }
        case 3:
        {
            FILE* fp=fopen("editLog.txt","a");
            printf("link[%d](%d/%d<->%d/%d).land from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].land, changeVal);
            fprintf(fp,"link[%d](%d/%d<->%d/%d).land from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].land, changeVal);
            rawEdgeList[i].land=changeVal;
            fclose(fp);
            break;
        }
        case 4:
        {
            FILE* fp=fopen("editLog.txt","a");
            printf("link[%d](%d/%d<->%d/%d).speed from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].speed, changeVal);
            fprintf(fp,"link[%d](%d/%d<->%d/%d).speed from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].speed, changeVal);
            rawEdgeList[i].speed=changeVal;
            rawEdgeList[i].isDefault=false;
            fclose(fp);
            break;
        }
        case 5:
        {   
            FILE* fp=fopen("editLog.txt","a");
            printf("link[%d](%d/%d<->%d/%d).len from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].len, changeVal);
            fprintf(fp,"link[%d](%d/%d<->%d/%d).len from %.3lf to %.3lf\n",rawEdgeList[i].id,rawEdgeList[i].node1,rawEdgeList[i].newNode1,rawEdgeList[i].node2,rawEdgeList[i].newNode2,rawEdgeList[i].len, changeVal);
            rawEdgeList[i].len=changeVal;
            fclose(fp);
            break;
        }
    }
    
    return 0;
}

int addLink(RawEdge* rawEdgeForAdd, int addId, int addNodeId1, int addNodeId2, int addWay, double addLen, char* addPoi, bool* isAddLink,Pair* pairs2,int nodeNum){
    if(addId!=-1&&addWay!=-1&&addLen>=0){
        *isAddLink=true;
        rawEdgeForAdd->id=addId;
        rawEdgeForAdd->node1=find_key_by_value(pairs2,nodeNum,addNodeId1);
        rawEdgeForAdd->node2=find_key_by_value(pairs2,nodeNum,addNodeId2);
        rawEdgeForAdd->newNode1=addNodeId1;
        rawEdgeForAdd->newNode2=addNodeId2;
        rawEdgeForAdd->way=addWay;
        rawEdgeForAdd->len=addLen;
        rawEdgeForAdd->veg=0;
        rawEdgeForAdd->land=0;
        rawEdgeForAdd->arch=0;
        if(addLen>60){
            rawEdgeForAdd->speed=30;
        }else if(addLen>20){
            rawEdgeForAdd->speed=20;
        }else{
            rawEdgeForAdd->speed=5;
        }
        rawEdgeForAdd->isDefault=true;
        
        
        char tmp[20]={};
        sprintf(tmp,"%s,;/link>",addPoi);
        // printf("%s||%s\n",tmp,rawEdgeForAdd->poi);

        strcpy(rawEdgeForAdd->poi,tmp);
        
        

        // printf("poi: %s\n", rawEdgeForAdd->poi);

        
    }
    return 0;
}

int writeEditedMap(char* newMapFileName,double* bounding, RawEdge* rawEdgeList, RawNode* rawNodeList, Way* wayList, Geom* geomList, int linkNum,int nodeNum,int wayNum, int geomNum,bool isAddLink, RawEdge*rawEdgeForAdd){
    FILE* fp=fopen(newMapFileName,"w");
    // write bounding tag
    // double minLat = bounding[0];
    // double minLon = bounding[1];
    // double maxLat = bounding[2];
    // double maxLon = bounding[3];
    fprintf(fp,"<bounding minLat=%.6lf minLon=%.6lf maxLat=%.6lf maxLon=%.6lf /bounding>\n",bounding[0],bounding[1],bounding[2],bounding[3]);
    // <link id=-2143392622 node=-8847 node=-8849 way=-8850 length=11.006410 veg=0.000000 arch=0.000000 land=0.000000 POI=;/link>
    for(int i=0;i<linkNum;i++){
        fprintf(fp,"<link id=%d node=%d node=%d way=%d length=%.6lf veg=%.6lf arch=%.6lf land=%.6lf POI=%s\n",
            rawEdgeList[i].id,
            rawEdgeList[i].node1,
            rawEdgeList[i].node2,
            rawEdgeList[i].way,
            rawEdgeList[i].len, 
            rawEdgeList[i].veg,
            rawEdgeList[i].arch,
            rawEdgeList[i].land,
            rawEdgeList[i].poi
        );
    }
    if (isAddLink){
        fprintf(fp,"<link id=%d node=%d node=%d way=%d length=%.6lf veg=%.6lf arch=%.6lf land=%.6lf POI=%s\n",
            rawEdgeForAdd->id,
            rawEdgeForAdd->node1,
            rawEdgeForAdd->node2,
            rawEdgeForAdd->way,
            rawEdgeForAdd->len, 
            rawEdgeForAdd->veg,
            rawEdgeForAdd->arch,
            rawEdgeForAdd->land,
            rawEdgeForAdd->poi
        );
        FILE* fpLog=fopen("editLog.txt","a");
        fprintf(fpLog,"Add a link:\n<link id=%d node=%d node=%d way=%d length=%.6lf veg=%.6lf arch=%.6lf land=%.6lf POI=%s\n",
            rawEdgeForAdd->id,
            rawEdgeForAdd->node1,
            rawEdgeForAdd->node2,
            rawEdgeForAdd->way,
            rawEdgeForAdd->len, 
            rawEdgeForAdd->veg,
            rawEdgeForAdd->arch,
            rawEdgeForAdd->land,
            rawEdgeForAdd->poi
        );
        printf("Add a link:\n<link id=%d node=%d/%d node=%d/%d way=%d length=%.6lf veg=%.6lf arch=%.6lf land=%.6lf POI=%s\n",
            rawEdgeForAdd->id,
            rawEdgeForAdd->newNode1,
            rawEdgeForAdd->node1,

            rawEdgeForAdd->newNode2,
            rawEdgeForAdd->node2,

            rawEdgeForAdd->way,
            rawEdgeForAdd->len, 
            rawEdgeForAdd->veg,
            rawEdgeForAdd->arch,
            rawEdgeForAdd->land,
            rawEdgeForAdd->poi
        );
    }
    // <node id=247958702 lat=53.803574 lon=-1.554796 /node>
    for(int i=0;i<nodeNum;i++){
        fprintf(fp,"<node id=%d lat=%.6lf lon=%.6lf /node>\n",
        rawNodeList[i].id,
        rawNodeList[i].lat,
        rawNodeList[i].lon
        );
    }
    // <way id=142903078 node=1563622354 node=1563622351 /way>
    for(int i=0;i<wayNum;i++){
        fprintf(fp, "<way id=%d",wayList[i].id);
        for(int j=0;j<wayList[i].nodeNum;j++){
            fprintf(fp, " node=%d", wayList[i].nodeList[j]);
        }
        fprintf(fp, " /way>\n");
    }
    // <geom id=23157689 node=250218819 node=1564466410 node=-2143392621 node=-2143392574 node=-2143392616 node=-2143392593 node=250218820 node=1564466414 node=-2143392594 node=-2143392596 node=-2143392597 node=-2143392571 node=250218821 node=250218822 node=250218819 /geom>
    for(int i=0;i<geomNum;i++){
        fprintf(fp, "<geom id=%d",geomList[i].id);
        for(int j=0;j<geomList[i].nodeNum;j++){
            fprintf(fp, " node=%d", geomList[i].nodeList[j]);
        }
        fprintf(fp, " /geom>\n");
    }
    // <spd linkId=-2143392622 speed=100 /spd>
    for(int j=0;j<linkNum;j++){
        if(rawEdgeList[j].isDefault){
            continue;
        }else{
            fprintf(fp,"<spd linkId=%d speed=%.6lf /spd>\n",rawEdgeList[j].id,rawEdgeList[j].speed);
        }


        // fprintf(fp,"<spd linkId=%d speed=%.6lf /spd>\n",speedList[i].linkId,speedList[i].value);}
        
    }
    fclose(fp);
    return 0;
    
}
