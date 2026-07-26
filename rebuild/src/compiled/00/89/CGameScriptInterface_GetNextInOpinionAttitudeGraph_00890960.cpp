typedef int EOpinionAttitudeType;
extern EOpinionAttitudeType g_OpinionAttitudeGraph[];
EOpinionAttitudeType __stdcall CGameScriptInterface_GetNextInOpinionAttitudeGraph(EOpinionAttitudeType attitude)
{
    return g_OpinionAttitudeGraph[attitude];
}