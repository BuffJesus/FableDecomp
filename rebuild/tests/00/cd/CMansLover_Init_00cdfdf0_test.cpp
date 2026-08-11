#include <cstdio>
#include <cstring>

struct VecThing;

// Same layout as the authored source; Init defined there and linked in.
struct ITargetT {
    void* vtbl;
};

struct CMansLover {
    int    m0;
    void*  m4;      // -> object whose first word is a vtable ptr
    int    f8[5];
    char   b1c;
    void Init();    // provided by s.cpp
};

// Observed side effects.
static void*  g_gotPtr = 0;
static int    g_gotFlag = -999;
static int    g_called = 0;

// slot-511 handler; ecx = ITarget object, stack args = (ptr, flag)
static void __fastcall slot511(void* /*ecxSelf*/, void* /*edx*/, void* p, int flag)
{
    g_called = 1;
    g_gotPtr = p;
    g_gotFlag = flag;
}

int main()
{
    void* vtbl[512];
    memset(vtbl, 0, sizeof(vtbl));
    vtbl[511] = (void*)&slot511;

    void* itargetVtblPtr = &vtbl[0];
    void* itargetObj = &itargetVtblPtr;

    CMansLover obj;
    memset(&obj, 0xAB, sizeof(obj));
    obj.m4 = itargetObj;
    obj.b1c = 0x55;

    obj.Init();

    bool ok = g_called
           && g_gotPtr == (void*)&obj.f8[0]
           && g_gotFlag == 0
           && obj.b1c == 0;

    if (ok) printf("MANSLOVER_INIT_OK\n");
    else    printf("FAIL called=%d ptr=%p expect=%p flag=%d b1c=%d\n",
                   g_called, g_gotPtr, (void*)&obj.f8[0], g_gotFlag, (int)obj.b1c);
    return ok ? 0 : 1;
}