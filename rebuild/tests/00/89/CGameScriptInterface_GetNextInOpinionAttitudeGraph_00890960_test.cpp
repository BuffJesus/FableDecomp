#include <cstdio>
typedef int EOpinionAttitudeType;
EOpinionAttitudeType g_OpinionAttitudeGraph[8] = {10,20,30,40,50,60,70,80};
EOpinionAttitudeType __stdcall CGameScriptInterface_GetNextInOpinionAttitudeGraph(EOpinionAttitudeType attitude)
{
    return g_OpinionAttitudeGraph[attitude];
}
int main(){
    if (CGameScriptInterface_GetNextInOpinionAttitudeGraph(0)!=10) { std::printf("FAIL idx0\n"); return 1; }
    if (CGameScriptInterface_GetNextInOpinionAttitudeGraph(3)!=40) { std::printf("FAIL idx3\n"); return 1; }
    if (CGameScriptInterface_GetNextInOpinionAttitudeGraph(7)!=80) { std::printf("FAIL idx7\n"); return 1; }
    std::printf("CGameScriptInterface_00890960_TEST PASS\n");
    return 0;
}