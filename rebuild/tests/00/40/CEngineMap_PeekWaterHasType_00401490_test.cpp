#include <cstdio>

struct CEngineMap;

static int g_ret = 0;
extern "C" int __cdecl Water_QueryType(CEngineMap* self, int type)
{
    (void)self; (void)type;
    return g_ret;
}

bool __fastcall CEngineMap_PeekWaterHasType(CEngineMap* self, int type)
{
    return Water_QueryType(self, type) == 0;
}

int main()
{
    CEngineMap* self = (CEngineMap*)0x1234;
    g_ret = 0;
    bool r0 = CEngineMap_PeekWaterHasType(self, 7);
    g_ret = 5;
    bool r1 = CEngineMap_PeekWaterHasType(self, 7);
    g_ret = -3;
    bool r2 = CEngineMap_PeekWaterHasType(self, 9);
    if (r0 == true && r1 == false && r2 == false) {
        std::printf("CEngineMap_00401490_TEST PASS\n");
        return 0;
    }
    std::printf("CEngineMap_00401490_TEST FAIL r0=%d r1=%d r2=%d\n", r0, r1, r2);
    return 1;
}