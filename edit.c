#include "edit.h"

int editLinkVal(int mode, RawEdge* rawEdgeList, double changeVal, int linkId){
    int i=linkId;
    switch (mode)
    {
        case 1:
        {   printf("link[%d].veg from %.3lf to %.3lf\n",i,rawEdgeList[i].veg, changeVal);
            rawEdgeList[i].veg=changeVal;
            // printf("link[%d].veg from %.3lf to %.3lf\n",i,);
            break;
        }
        case 2:
        {
            printf("link[%d].arch from %.3lf to %.3lf\n",i,rawEdgeList[i].arch, changeVal);

            rawEdgeList[i].arch=changeVal;
            break;
        }
        case 3:
        {
            printf("link[%d].land from %.3lf to %.3lf\n",i ,rawEdgeList[i].land, changeVal);
            rawEdgeList[i].land=changeVal;
            break;
        }
        case 4:
        {
            printf("link[%d].speed from %.3lf to %.3lf\n",i,rawEdgeList[i].speed, changeVal);

            rawEdgeList[i].speed=changeVal;
            break;
        }
        case 5:
        {   
            printf("link[%d].len from %.3lf to %.3lf\n",i,rawEdgeList[i].len, changeVal);
            
            rawEdgeList[i].len=changeVal;
            break;
        }
    }
    return 0;
}
// int editNodeVal(int mode, RawEdge* rawEdgeList, double changeVal, int linkId){
//     int i=linkId;
//     switch (mode)
//     {
//         case 1:
//         {
//             rawEdgeList[i].veg=changeVal;
//             break;
//         }
//         case 2:
//         {
//             rawEdgeList[i].arch=changeVal;
//             break;
//         }
//         case 3:
//         {
//             rawEdgeList[i].land=changeVal;
//             break;
//         }
//         case 4:
//         {
//             rawEdgeList[i].speed=changeVal;
//             break;
//         }
//     }
//     return 0;
// }
