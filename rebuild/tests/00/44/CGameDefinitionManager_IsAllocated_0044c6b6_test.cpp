// Behaviour of the CGameDefinitionManager singleton accessors against a recording CCountedPointer::Reset.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

CCountedPointer<CGameDefinitionManager> CGameDefinitionManager::PClass;

static int g_resetCalls;
static const void* g_resetThis;
static const void* g_resetArg;
template<typename T> void CCountedPointer<T>::Reset(T* data)
{
    ++g_resetCalls; g_resetThis = this; g_resetArg = data;
    Data = data; Info = 0;
}

struct Peek : public CGameDefinitionManager
{
    static CCountedPointer<CGameDefinitionManager>& P() { return PClass; }
};

static unsigned char g_a[sizeof(CGameDefinitionManager)];
static unsigned char g_b[sizeof(CGameDefinitionManager)];

int main()
{
    CGameDefinitionManager* a = reinterpret_cast<CGameDefinitionManager*>(g_a);
    CGameDefinitionManager* b = reinterpret_cast<CGameDefinitionManager*>(g_b);
    CCountedPointer<CGameDefinitionManager>& p = Peek::P();
    p.Data = 0; p.Info = reinterpret_cast<CCPPointerInfo*>(0x55);
    if (CGameDefinitionManager::IsAllocated() != false) { printf("FAIL null\n"); return 1; }
    p.Data = a; p.Info = 0;
    if (CGameDefinitionManager::IsAllocated() != true) { printf("FAIL set\n"); return 2; }
    if (g_resetCalls != 0 || p.Data != a) { printf("FAIL side effect\n"); return 3; }
    (void)b;
    p.Data = 0; p.Info = 0;  // the static dtor must not release fixture storage
    printf("FABLETLC_GAMEDEFMGR_ISALLOCATED PASS\n");
    return 0;
}