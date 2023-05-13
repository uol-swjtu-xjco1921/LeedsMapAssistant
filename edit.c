#include "edit.h"

int editLinkVal(int mode, RawEdge* rawEdgeList, double changeVal, int linkId){
    int i=linkId;
    switch (mode)
    {
        case 1:
        {
            rawEdgeList[i].veg=changeVal;
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
