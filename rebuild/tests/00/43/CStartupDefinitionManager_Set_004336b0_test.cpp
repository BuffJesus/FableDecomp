// Behaviour of the CStartupDefinitionManager singleton accessors against a recording CCountedPointer::Reset.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

// PClass is defined by the source unit.

static int g_resetCalls;
static const void* g_resetThis;
static const void* g_resetArg;
template<typename T> void CCountedPointer<T>::Reset(T* data)
{
    ++g_resetCalls; g_resetThis = this; g_resetArg = data;
    Data = data; Info = 0;
}

template void CCountedPointer<CStartupDefinitionManager>::Reset(CStartupDefinitionManager*);

struct Peek : public CStartupDefinitionManager
{
    static CCountedPointer<CStartupDefinitionManager>& P() { return PClass; }
};

static unsigned char g_a[sizeof(CStartupDefinitionManager)];
static unsigned char g_b[sizeof(CStartupDefinitionManager)];

int main()
{
    CStartupDefinitionManager* a = reinterpret_cast<CStartupDefinitionManager*>(g_a);
    CStartupDefinitionManager* b = reinterpret_cast<CStartupDefinitionManager*>(g_b);
    CCountedPointer<CStartupDefinitionManager>& p = Peek::P();
    CStartupDefinitionManager::Set(a);
    if (g_resetCalls != 1 || g_resetThis != &p || g_resetArg != a || p.Data != a) { printf("FAIL set a\n"); return 1; }
    CStartupDefinitionManager::Set(0);
    if (g_resetCalls != 2 || g_resetThis != &p || g_resetArg != 0 || p.Data != 0) { printf("FAIL set null\n"); return 2; }
    CStartupDefinitionManager::Set(b);
    if (g_resetCalls != 3 || g_resetArg != b) { printf("FAIL set b\n"); return 3; }
    p.Data = 0; p.Info = 0;  // the static dtor must not release fixture storage
    printf("PASS_004336b0\n");
    return 0;
}