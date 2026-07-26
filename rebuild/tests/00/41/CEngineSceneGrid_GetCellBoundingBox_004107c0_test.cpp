#include <cstdio>

struct CEngineSceneGrid;

static char g_flag_seen = 1;
static void* g_a1;
static void* g_a2;
static const void* g_self;

bool __fastcall Inner(CEngineSceneGrid* self, int edx, void* arg1, void* arg2, char* flag)
{
    g_self = self;
    g_a1 = arg1;
    g_a2 = arg2;
    g_flag_seen = *flag;   /* should be 0 */
    return true;
}

bool __fastcall CEngineSceneGrid_GetCellBoundingBox(CEngineSceneGrid* self, int edx, void* arg1, void* arg2)
{
    char flag = 0;
    return Inner(self, (int)arg2, arg1, arg2, &flag);
}

int main()
{
    int a = 11, b = 22;
    CEngineSceneGrid* self = (CEngineSceneGrid*)0x1234;
    bool r = CEngineSceneGrid_GetCellBoundingBox(self, 0, &a, &b);
    if (r && g_flag_seen == 0 && g_self == (const void*)self && g_a1 == &a && g_a2 == &b) {
        std::printf("CEngineSceneGrid_004107c0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r=%d flag=%d\n", (int)r, (int)g_flag_seen);
    return 1;
}