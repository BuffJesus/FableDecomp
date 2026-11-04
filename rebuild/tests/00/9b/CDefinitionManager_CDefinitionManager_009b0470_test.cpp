#include "../src/compiled/00/4a/CSymbolMap_ContainerLifetime_004ad4a0.cpp"
// Behaviour of CDefinitionManager(const CWideString&, ErrorCallbackFunction)
// 0x009B0470: CResourceList base first; empty path vectors, vector maps (Dirty
// false), def data and instantiated-def list; the error callback stored; the
// compiled file name is CompiledDefDirectory + filename; RecompiledDefs has a
// self-linked header node; two fresh CSymbolMaps go to PDefinitionSymbolMap
// then PSymbolMap, and only PSymbolMap learns "NULL" = 0 and is updated; the
// unload delay ends 0. PDefTypeDebugInfoMap (+0x3C) and DependencyCheckCRC
// (+0xBC) are left alone.
#include "fable_definition_manager.h"
#include "test_support/resource_list_stubs.h"
#include <new>
#include <stdio.h>
#include <string.h>

void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}

// Recording stand-ins (fixture scaffolding) for the ctor's callees.
static char g_log[512];
static void Log(const char* text) { strcat(g_log, text); }
static const void* g_plusLeft;
static const void* g_plusRight;
static const void* const kJoinedStorage = (const void*)0x13572468;
static const char* g_charText;
static long g_charLength;
static CSymbolMap* g_maps[2];
static int g_mapCount;
static const void* g_resetThis[2];
static CSymbolMap* g_resetData[2];
static int g_resetCount;
static const void* g_addThis;
static const char* g_addName;
static long g_addVal;
static const void* g_updateThis;

CCharString::CCharString() { memset(this, 0, sizeof(*this)); Log("cs();"); }
CCharString::CCharString(const char* text, long length)
{
    memset(this, 0, sizeof(*this));
    g_charText = text;
    g_charLength = length;
    Log("cs(text);");
}
CCharString::~CCharString() { Log("~cs;"); }
CWideString::CWideString() { memset(this, 0, sizeof(*this)); }
CWideString::CWideString(const CWideString& other) { memcpy(this, &other, sizeof(*this)); }
CWideString::~CWideString() {}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right)
{
    g_plusLeft = &left;
    g_plusRight = &right;
    Log("plus;");
    CWideString joined;
    memcpy(&joined, &kJoinedStorage, sizeof(kJoinedStorage));
    return joined;
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CSymbolMap::CSymbolMap() { g_maps[g_mapCount++] = this; Log("map();"); }

void CSymbolMap::AddSymbolDirty(const CCharString& name, long val)
{
    g_addThis = this;
    g_addName = (g_charText && &name) ? g_charText : 0;
    g_addVal = val;
    Log("add;");
}
void CSymbolMap::UpdateDirty() const { g_updateThis = this; Log("update;"); }
template<> void CCountedPointer<CSymbolMap>::Reset(CSymbolMap* data)
{
    g_resetThis[g_resetCount] = this;
    g_resetData[g_resetCount++] = data;
    Data = data;
    Info = 0;
    Log("reset;");
}
CDefinitionManager::~CDefinitionManager() {}

static void FABLE_FASTCALL Callback(const CCharString&) {}

// CDefinitionManager is abstract (InitAndCompile is pure); the probe adds
// nothing but that slot.
struct Probe : CDefinitionManager
{
    Probe(const CWideString& filename, ErrorCallbackFunction callback)
        : CDefinitionManager(filename, callback)
    {
    }
    virtual void InitAndCompile() {}
};

static unsigned long U32(const unsigned char* base, unsigned int offset)
{
    return *reinterpret_cast<const unsigned long*>(base + offset);
}

int main()
{
    static unsigned char storage[sizeof(Probe)];
    memset(storage, 0xA5, sizeof(storage));
    unsigned long nameRaw = 0;
    const CWideString& name = *reinterpret_cast<const CWideString*>(&nameRaw);

    Probe* manager = new (storage) Probe(name, Callback);
    if ((void*)manager != storage) { puts("this"); return 1; }
    if (strcmp(g_log, "cs();plus;map();reset;map();reset;cs(text);add;~cs;update;") != 0) { puts(g_log); return 1; }

    // CResourceList base (unload delay then cleared to 0).
    if (U32(storage, 0x10) != (unsigned long)(storage + 4) || U32(storage, 0x28) != 0x7FFFFFFF ||
        U32(storage, 0x34) != 0) { puts("resource list"); return 1; }
    if (U32(storage, 0x3C) != 0xA5A5A5A5 || U32(storage, 0xBC) != 0xA5A5A5A5) { puts("untouched"); return 1; }
    static const unsigned int zeroed[] = { 0x40, 0x44, 0x48, 0x4C, 0x50, 0x54, 0x68, 0x6C, 0x70, 0x78, 0x7C,
        0x80, 0x88, 0x8C, 0x90, 0x94, 0x98, 0x9C, 0xA4, 0xA8, 0xAC, 0xC8 };
    for (unsigned int i = 0; i < sizeof(zeroed) / sizeof(zeroed[0]); ++i)
        if (U32(storage, zeroed[i]) != 0) { printf("zero %x\n", zeroed[i]); return 1; }
    if (storage[0x75] != 0 || storage[0x85] != 0 || storage[0xA1] != 0 || storage[0xC0] != 0) { puts("flags"); return 1; }
    if (U32(storage, 0xB0) != (unsigned long)&Callback) { puts("callback"); return 1; }
    if (g_plusRight != &name || U32(storage, 0xB8) != (unsigned long)kJoinedStorage) { puts("file name"); return 1; }

    // RecompiledDefs: a malloc'd header node linked to itself.
    const unsigned char* header = reinterpret_cast<const unsigned char*>(U32(storage, 0xC4));
    if (!header || header[0] != 0 || U32(header, 4) != 0 || U32(header, 8) != (unsigned long)header ||
        U32(header, 12) != (unsigned long)header) { puts("recompiled defs"); return 1; }

    // Symbol maps: the first new map to PDefinitionSymbolMap (+0x60), the second
    // to PSymbolMap (+0x58), which alone gets NULL = 0 and an update.
    if (g_mapCount != 2 || g_resetCount != 2 || g_resetThis[0] != storage + 0x60 || g_resetData[0] != g_maps[0] ||
        g_resetThis[1] != storage + 0x58 || g_resetData[1] != g_maps[1]) { puts("maps"); return 1; }
    if (g_addThis != g_maps[1] || g_addVal != 0 || !g_addName || strcmp(g_addName, "NULL") != 0 ||
        g_charLength != -1 || g_updateThis != g_maps[1]) { puts("symbols"); return 1; }

    puts("FABLETLC_CDEFINITIONMANAGER_CTOR_009B0470_PASS");
    return 0;
}

