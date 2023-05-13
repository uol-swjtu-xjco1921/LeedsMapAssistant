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
            rawEdgeList[i].arch=changeVal;
            break;
        }
        case 3:
        {
            rawEdgeList[i].land=changeVal;
            break;
        }
        case 4:
        {
            rawEdgeList[i].speed=changeVal;
            break;
        }
        case 5:
        {
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
