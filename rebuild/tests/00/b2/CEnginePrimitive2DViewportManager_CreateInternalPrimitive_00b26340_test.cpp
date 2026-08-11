#include <cstdio>
#include <cstring>

struct CEngineInternalPrimitiveBase
{
    CEngineInternalPrimitiveBase* Construct();
};

static int g_ctorCalls = 0;
static void* g_lastSelf = 0;
static char g_pool[512];
static bool g_allocOk = true;

void* operator_new_stub(unsigned int size)
{
    if (!g_allocOk) return 0;
    memset(g_pool, 0, sizeof(g_pool));
    return g_pool;
}

CEngineInternalPrimitiveBase* CEngineInternalPrimitiveBase::Construct()
{
    g_ctorCalls++;
    g_lastSelf = (void*)this;
    return this;
}

struct CEnginePrimitive2DViewportManager
{
    CEngineInternalPrimitiveBase* CreateInternalPrimitive();
};

CEngineInternalPrimitiveBase*
CEnginePrimitive2DViewportManager::CreateInternalPrimitive()
{
    CEngineInternalPrimitiveBase* p =
        (CEngineInternalPrimitiveBase*)operator_new_stub(0x178);
    if (p)
        return p->Construct();
    return 0;
}

int main()
{
    CEnginePrimitive2DViewportManager mgr;

    g_allocOk = true;
    g_ctorCalls = 0;
    void* r1 = mgr.CreateInternalPrimitive();
    if (r1 != (void*)g_pool) { printf("FAIL r1\n"); return 1; }
    if (g_ctorCalls != 1) { printf("FAIL ctorcount\n"); return 1; }
    if (g_lastSelf != (void*)g_pool) { printf("FAIL self\n"); return 1; }

    g_allocOk = false;
    g_ctorCalls = 0;
    void* r2 = mgr.CreateInternalPrimitive();
    if (r2 != 0) { printf("FAIL r2\n"); return 1; }
    if (g_ctorCalls != 0) { printf("FAIL noctor\n"); return 1; }

    printf("CREATEINTERNALPRIMITIVE_OK\n");
    return 0;
}