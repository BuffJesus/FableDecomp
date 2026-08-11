#include <cstdio>

struct CEngineResourceManager
{
    char pad0[0x30];
    void* field30;   // +0x30

    void Sub_c95370();
    void WorldFrameUpdate();
};

static int g_subCalls = 0;
static int g_afterCalls = 0;

void CEngineResourceManager::Sub_c95370()
{
    g_subCalls++;
}

void (__cdecl *g_afterUpdate)(CEngineResourceManager*) = 0;

static void __cdecl myAfter(CEngineResourceManager*)
{
    g_afterCalls++;
}

void CEngineResourceManager::WorldFrameUpdate()
{
    if (this->field30 != 0)
    {
        this->Sub_c95370();
        g_afterUpdate(this);
    }
}

int main()
{
    g_afterUpdate = myAfter;

    CEngineResourceManager a;
    a.field30 = 0;
    a.WorldFrameUpdate();
    // field30 == 0 -> nothing called
    if (g_subCalls != 0 || g_afterCalls != 0) { printf("FAIL0\n"); return 1; }

    CEngineResourceManager b;
    b.field30 = (void*)1;
    b.WorldFrameUpdate();
    if (g_subCalls != 1 || g_afterCalls != 1) { printf("FAIL1\n"); return 1; }

    printf("WORLDFRAMEUPDATE_OK\n");
    return 0;
}