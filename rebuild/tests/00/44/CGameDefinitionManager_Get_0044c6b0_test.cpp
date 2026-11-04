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
    p.Data = a; p.Info = reinterpret_cast<CCPPointerInfo*>(0x1234);
    if (&CGameDefinitionManager::Get() != a) { printf("FAIL a\n"); return 1; }
    p.Data = b;
    if (&CGameDefinitionManager::Get() != b) { printf("FAIL b\n"); return 2; }
    if (g_resetCalls != 0 || p.Info != reinterpret_cast<CCPPointerInfo*>(0x1234)) { printf("FAIL side effect\n"); return 3; }
    p.Data = 0; p.Info = 0;  // the static dtor must not release fixture storage
    printf("_global_0044c6b0_TEST PASS\n");
    return 0;
}