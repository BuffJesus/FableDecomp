#include <cstdio>

struct Def { char pad[8]; long classIndex; };
struct CDefinitionManager { char pad[0xa4]; Def** table; };

long __fastcall CDefinitionManager_GetDefClassIndexFromGlobalIndex(CDefinitionManager* self, int edx_unused, int globalIndex)
{
    return self->table[globalIndex]->classIndex;
}

int main()
{
    Def a, b, c;
    a.classIndex = 100; b.classIndex = 200; c.classIndex = 300;
    Def* ptrs[3]; ptrs[0] = &a; ptrs[1] = &b; ptrs[2] = &c;
    CDefinitionManager mgr;
    mgr.table = ptrs;
    if (CDefinitionManager_GetDefClassIndexFromGlobalIndex(&mgr, 0, 0) != 100) { std::printf("FAIL 0\n"); return 1; }
    if (CDefinitionManager_GetDefClassIndexFromGlobalIndex(&mgr, 0, 1) != 200) { std::printf("FAIL 1\n"); return 1; }
    if (CDefinitionManager_GetDefClassIndexFromGlobalIndex(&mgr, 0, 2) != 300) { std::printf("FAIL 2\n"); return 1; }
    std::printf("CDefinitionManager_009acca0_TEST PASS\n");
    return 0;
}