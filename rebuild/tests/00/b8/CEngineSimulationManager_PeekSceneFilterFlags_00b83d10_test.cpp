#include <cstdio>

struct CEngineSimulationManager;

unsigned long __fastcall CEngineSimulationManager_PeekSceneFilterFlags(CEngineSimulationManager* self);

struct CEngineSimulationManager { int dummy; };

int main()
{
    CEngineSimulationManager obj;
    unsigned long r = CEngineSimulationManager_PeekSceneFilterFlags(&obj);
    if (r == 3UL)
        std::printf("00b83d10_TEST PASS\n");
    else
        std::printf("FAIL %lu\n", r);
    return 0;
}