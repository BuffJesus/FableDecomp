struct CEngineSceneGrid;

extern bool __fastcall Inner(CEngineSceneGrid* self, int edx, void* arg1, void* arg2, char* flag);

bool __fastcall CEngineSceneGrid_GetCellBoundingBox(CEngineSceneGrid* self, int edx, void* arg1, void* arg2)
{
    char flag = 0;
    return Inner(self, (int)arg2, arg1, arg2, &flag);
}