struct CEngineMap;
extern int __cdecl Water_QueryType(CEngineMap* self, int type);

bool __fastcall CEngineMap_PeekWaterHasType(CEngineMap* self, int type)
{
    return Water_QueryType(self, type) == 0;
}