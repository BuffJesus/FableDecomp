// Behaviour of CNewFrontendGameComponent::CompileDefsPlatformSpecific (0x0042F5A9)
// against recording stand-ins: file name choice, manager construction and
// install, def-class registration order (name + allocator), the PC-only
// control scheme, the final compile call and every temporary's lifetime.
#include "fable_game.h"
#include "fable_definition_manager.h"
#include "test_support/resource_list_stubs.h"
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

static char g_log[4096];
static bool ok = true;
static void Log(const char* text) { strcat(g_log, text); strcat(g_log, ";"); }

static CWideString* g_file;
static const wchar_t* g_fileText;
static void* g_block;
static CStartupDefinitionManager* g_manager;
static CCharString* g_temp;
static CCharString* g_copy;
static unsigned long g_wideDataRaw_[4];
static CWideStringData& g_wideData = *reinterpret_cast<CWideStringData*>(g_wideDataRaw_);
static CCharStringData g_charData;

CDefClassBase* FABLE_FASTCALL CFrontEndDef::Alloc() { return 0; }
CDefClassBase* FABLE_FASTCALL NUISystem::CUIMiscThingsDef::Alloc() { return 0; }
CDefClassBase* FABLE_FASTCALL NUISystem::CUIIconsDef::Alloc() { return 0; }
CDefClassBase* FABLE_FASTCALL CControlsDef::Alloc() { return 0; }

CDefinitionManager::~CDefinitionManager() {}
void CStartupDefinitionManager::InitAndCompile() { ok = false; }

CWideString::CWideString() : storage_(0) { g_file = this; Log("ws"); }
const CWideString& CWideString::operator=(const wchar_t* text)
{
    if (this != g_file) ok = false;
    g_fileText = text;
    storage_ = &g_wideData;
    Log("ws=");
    return *this;
}
CWideString::~CWideString() { if (this != g_file) ok = false; Log("~ws"); }

void* operator new(size_t size)
{
    void* p = malloc(size < 0x100 ? 0x100 : size);
    if (size == 0xD0) { g_block = p; Log("new"); }
    return p;
}
void operator delete(void* p) { free(p); }

// Stand-in base ctor (retail 0x009B0470) so the stand-in manager ctor links:
// a member-free fastcall body under the ctor's mangled name (ecx = this,
// stack = name, callback), so no CDefinitionManager member is constructed.
extern "C" void* __fastcall DefinitionManagerCtorStandIn(void* self, int, const void*, void*) { return self; }
#pragma comment(linker, "/alternatename:??0CDefinitionManager@@QAE@ABVCWideString@@P6IXABVCCharString@@@Z@Z=@DefinitionManagerCtorStandIn@16")
CStartupDefinitionManager::CStartupDefinitionManager(const CWideString& filename)
    : CDefinitionManager(filename, 0)
{
    if (this != g_block || &filename != g_file || g_fileText == 0) ok = false;
    Log("ctor");
}
void FABLE_FASTCALL CStartupDefinitionManager::Set(CStartupDefinitionManager* manager)
{
    if (manager != g_block) ok = false;
    g_manager = manager;
    Log("set");
}
CStartupDefinitionManager& FABLE_FASTCALL CStartupDefinitionManager::Get()
{
    Log("get");
    return *g_manager;
}
CCharString::CCharString(const char* text, long length) : storage_(&g_charData)
{
    if (length != -1) ok = false;
    g_temp = this;
    Log(text);
}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_)
{
    if (&other != g_temp) ok = false;
    g_copy = this;
    Log("copy");
}
CCharString::~CCharString()
{
    if (this == g_copy) { g_copy = 0; Log("~copy"); }
    else if (this == g_temp) { g_temp = 0; Log("~name"); }
    else { ok = false; Log("~?"); }
}
void CDefinitionManager::AddDefClass(const CDefClassInit& init)
{
    if (this != g_manager || &init.Name != g_copy) ok = false;
    CDefClassInit::AllocFunction f = init.AllocFunc;
    if (f == CFrontEndDef::Alloc) Log("add:frontend");
    else if (f == NUISystem::CUIMiscThingsDef::Alloc) Log("add:misc");
    else if (f == NUISystem::CUIIconsDef::Alloc) Log("add:icons");
    else if (f == CControlsDef::Alloc) Log("add:controls");
    else Log("add:?");
}
void CStartupDefinitionManager::InitAndCompilePlatformSpecific(bool xbox)
{
    if (this != g_manager) ok = false;
    Log(xbox ? "compile:1" : "compile:0");
}

static const char* const kCommon =
    "ws;ws=;new;ctor;set;get;"
    "FRONT_END;copy;add:frontend;~copy;~name;"
    "UI_MISC_THINGS_DEF;copy;add:misc;~copy;~name;"
    "UI_ICONS_DEF;copy;add:icons;~copy;~name;";

static bool Run(bool xbox)
{
    char expected[1024];
    strcpy(expected, kCommon);
    if (!xbox) strcat(expected, "CONTROL_SCHEME;copy;add:controls;~copy;~name;");
    strcat(expected, xbox ? "compile:1;~ws;" : "compile:0;~ws;");
    g_log[0] = 0;
    g_fileText = 0;
    ok = true;
    CNewFrontendGameComponent::CompileDefsPlatformSpecific(xbox);
    const wchar_t* want = xbox ? L"xfrontend.bin" : L"frontend.bin";
    if (!ok || g_fileText == 0 || wcscmp(g_fileText, want) != 0 || strcmp(g_log, expected) != 0)
    {
        printf("xbox=%d ok=%d\n got %s\nwant %s\n", xbox, ok, g_log, expected);
        return false;
    }
    return true;
}

int main()
{
    if (!Run(false) || !Run(true)) return 1;
    puts("FABLETLC_COMPILEDEFSPLATFORMSPECIFIC_0042F5A9_PASS");
    return 0;
}