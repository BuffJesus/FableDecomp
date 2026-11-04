// Behaviour of the CStartupDefinitionManager singleton accessors against a recording CCountedPointer::Reset.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

CCountedPointer<CStartupDefinitionManager> CStartupDefinitionManager::PClass;

static int g_resetCalls;
static const void* g_resetThis;
static const void* g_resetArg;
template<typename T> void CCountedPointer<T>::Reset(T* data)
{
    ++g_resetCalls; g_resetThis = this; g_resetArg = data;
    Data = data; Info = 0;
}

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
    p.Data = a; p.Info = reinterpret_cast<CCPPointerInfo*>(0x1234);
    if (&CStartupDefinitionManager::Get() != a) { printf("FAIL a\n"); return 1; }
    p.Data = b;
    if (&CStartupDefinitionManager::Get() != b) { printf("FAIL b\n"); return 2; }
    if (g_resetCalls != 0 || p.Info != reinterpret_cast<CCPPointerInfo*>(0x1234)) { printf("FAIL side effect\n"); return 3; }
    p.Data = 0; p.Info = 0;  // the static dtor must not release fixture storage
    printf("PASS_0043368d\n");
    return 0;
}