// Behaviour of CStartupDefinitionManager(const CWideString&) 0x00433693: the
// CDefinitionManager base gets this object, the caller's file name (by
// reference) and ErrorCallback; afterwards the object dispatches through the
// CStartupDefinitionManager vtable; nothing past the vptr is written.
#include "fable_definition_manager.h"
#include "test_support/resource_list_stubs.h"
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
#include <new>
#include <stdio.h>
#include <string.h>

static int g_baseCalls;
static const void* g_baseThis;
static const CWideString* g_baseName;
static CDefinitionManager::ErrorCallbackFunction g_baseCallback;
static int g_initAndCompile;
static const void* g_initThis;

// Recording stand-in for the base ctor (retail 0x009B0470): a member-free
// fastcall body under its mangled name (ecx = this, stack = name, callback),
// so nothing past the vptr the derived ctor stores is written.
extern "C" void* __fastcall DefinitionManagerCtorStandIn(void* self, int, const CWideString* filename,
                                                         CDefinitionManager::ErrorCallbackFunction errorCallback)
{
    ++g_baseCalls;
    g_baseThis = self;
    g_baseName = filename;
    g_baseCallback = errorCallback;
    return self;
}
#pragma comment(linker, "/alternatename:??0CDefinitionManager@@QAE@ABVCWideString@@P6IXABVCCharString@@@Z@Z=@DefinitionManagerCtorStandIn@16")
CDefinitionManager::~CDefinitionManager() {}
CWideString::~CWideString() {}
CCharString::~CCharString() {}
void CStartupDefinitionManager::InitAndCompile() { ++g_initAndCompile; g_initThis = this; }
void FABLE_FASTCALL CStartupDefinitionManager::ErrorCallback(const CCharString&) {}

struct Probe : CStartupDefinitionManager
{
    static ErrorCallbackFunction Callback() { return &ErrorCallback; }
};

int main()
{
    unsigned char storage[sizeof(CStartupDefinitionManager)];
    memset(storage, 0xA5, sizeof(storage));
    unsigned long nameRaw = 0x12345678;
    const CWideString& name = *reinterpret_cast<const CWideString*>(&nameRaw);

    CStartupDefinitionManager* manager = new (storage) CStartupDefinitionManager(name);
    if (manager != reinterpret_cast<CStartupDefinitionManager*>(storage)) return 1;
    if (g_baseCalls != 1 || g_baseThis != storage || g_baseName != &name ||
        g_baseCallback != Probe::Callback()) { puts("base"); return 1; }
    for (size_t i = 4; i < sizeof(storage); ++i)
        if (storage[i] != 0xA5) { puts("pad written"); return 1; }
    CDefinitionManager* base = manager;
    base->InitAndCompile();
    if (g_initAndCompile != 1 || g_initThis != storage) { puts("vtable"); return 1; }
    puts("FABLETLC_CSTARTUPDEFINITIONMANAGER_CTOR_00433693_PASS");
    return 0;
}