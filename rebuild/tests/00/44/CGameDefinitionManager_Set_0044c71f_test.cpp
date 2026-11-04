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

template void CCountedPointer<CGameDefinitionManager>::Reset(CGameDefinitionManager*);

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
    CGameDefinitionManager::Set(a);
    if (g_resetCalls != 1 || g_resetThis != &p || g_resetArg != a || p.Data != a) { printf("FAIL set a\n"); return 1; }
    CGameDefinitionManager::Set(0);
    if (g_resetCalls != 2 || g_resetThis != &p || g_resetArg != 0 || p.Data != 0) { printf("FAIL set null\n"); return 2; }
    CGameDefinitionManager::Set(b);
    if (g_resetCalls != 3 || g_resetArg != b) { printf("FAIL set b\n"); return 3; }
    p.Data = 0; p.Info = 0;  // the static dtor must not release fixture storage
    printf("_global_0044c71f_TEST PASS\n");
    return 0;
}