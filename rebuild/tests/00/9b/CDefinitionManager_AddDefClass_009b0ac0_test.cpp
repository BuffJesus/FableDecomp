// Behaviour of CDefinitionManager::AddDefClass(const CDefClassInit&) 0x009B0AC0:
// shows "Add Def Class" progress (-1, false, false); appends (CRC of the name,
// {name, allocator, copied index list}) to DefClassInfo and sorts it (map
// clean afterwards). Only when CreateChecksums is set or UsingCompiledDefs is
// not: instantiates "NULLDEF_" + name of that class, loads the class's first
// global index and, holding a reference to that def, transfers it through a
// clear-mode persist context (the def is released after, and deleted if that
// was its last reference).
#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_persist_context.h"
#include "fable_progress_display.h"
#include <stdio.h>
#include <string.h>

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
typedef CKeyPairCompareLess<unsigned long, Info> Less;
static char g_log[1024];
static void Log(const char* text) { strcat(g_log, text); }
static void* Raw(const CCharString& s) { return *reinterpret_cast<void* const*>(&s); }

// Fixture scaffolding: a string is its storage pointer.
char g_FableEmptyNarrowString_0129AAF4[1];
CCharString::CCharString() { *reinterpret_cast<void**>(this) = 0; }
CCharString::CCharString(const char* text, long length)
{
    *reinterpret_cast<const char**>(this) = text;
    if (length != -1) Log("len!;");
}
CCharString::CCharString(const CCharString& other) { *reinterpret_cast<void**>(this) = Raw(other); }
CCharString::~CCharString() {}
CCharString& CCharString::operator=(const CCharString& other) { *reinterpret_cast<void**>(this) = Raw(other); return *this; }

static const void* g_crcData;
static fable_u32 g_crcLength;
fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(fable_u32 seed, const void* data, fable_u32 byteCount)
{
    g_crcData = data;
    g_crcLength = byteCount;
    return seed == 0 ? 0xC0DE : 0;
}
void FABLE_FASTCALL NProgressDisplay::DisplayProgress(const CCharString& text, float fraction, bool a, bool b)
{
    if (strcmp((const char*)Raw(text), "Add Def Class") == 0 && fraction == -1.0f && !a && !b)
        Log("progress;");
}
static void* const kJoined = (void*)0x7777;
static const void* g_plusRight;
CCharString FABLE_FASTCALL operator+(const char* left, const CCharString& right)
{
    if (strcmp(left, "NULLDEF_") == 0) Log("plus;");
    g_plusRight = &right;
    CCharString joined;
    *reinterpret_cast<void**>(&joined) = kJoined;
    return joined;
}
static unsigned long g_key;
static void* g_entryName;
static CDefClassInit::AllocFunction g_entryAlloc;
template<> void _STL::vector<Entry>::push_back(const Entry& x)
{
    Log("insert;");
    g_key = x.first;
    g_entryName = Raw(x.second.Name);
    g_entryAlloc = x.second.AllocFunc;
}
template<> _STL::vector<unsigned long>::vector(const _STL::vector<unsigned long>& x)
    : _STL::_Vector_base<unsigned long, _STL::allocator<unsigned long> >(_STL::allocator<unsigned long>())
{
    Log("copyidx;");
}
static const void* g_sortFirst;
static const void* g_sortLast;
template<> void FABLE_FASTCALL _STL::sort(Entry* first, Entry* last, Less)
{
    Log("sort;");
    g_sortFirst = first;
    g_sortLast = last;
}

// The manager's own callees.
static const CCharString* g_expectName;
static unsigned long g_indexStore[2] = { 2, 9 };
static unsigned char g_infoStore[sizeof(Info)];
bool CDefinitionManager::InstantiateDef(const CCharString& className, const CCharString& instName, bool flag)
{
    if (&className == g_expectName && Raw(instName) == kJoined && !flag) Log("inst;");
    return true;
}
CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString& className)
{
    if (&className == g_expectName) Log("info;");
    Info* info = reinterpret_cast<Info*>(g_infoStore);
    info->GlobalIndices._M_start = g_indexStore;
    info->GlobalIndices._M_finish = g_indexStore + 2;
    return info;
}
void CDefinitionManager::LoadDef(unsigned long globalIndex) const
{
    if (globalIndex == 2) Log("load;");
}
static const void* g_clearContext;
void CPersistContext::InitClearMode() { g_clearContext = this; Log("clear;"); }

// A hand-built def: a vtable with Unlock's slot 1 and Transfer's slot 18.
struct FakeDef { void** vptr; long RefCount; };
static FakeDef g_def;
static void* g_vtable[25];
static void FABLE_FASTCALL FakeDelete(FakeDef* self) { if (self == &g_def) Log("delete;"); }
static void FABLE_FASTCALL FakeTransfer(FakeDef* self, void*, CPersistContext* context)
{
    if (self == &g_def && context == g_clearContext) { char t[32]; sprintf(t, "transfer%ld;", self->RefCount); Log(t); }
}

struct Probe : CDefinitionManager
{
    static void Flags(bool usingCompiled, bool checksums) { UsingCompiledDefs = usingCompiled; CreateChecksums = checksums; }
    CVectorMap<unsigned long, CDefClassInfo, CKeyPairCompareLess<unsigned long, CDefClassInfo> >& Map() { return DefClassInfo; }
    _STL::vector<CInstantiatedDefInfo*>& Defs() { return InstantiatedDefs; }
};

static int Run(Probe* m, const CDefClassInit& init, const char* want)
{
    g_log[0] = 0;
    m->Map().Dirty = true;
    m->AddDefClass(init);
    if (strcmp(g_log, want) != 0) { printf("got  %s\nwant %s\n", g_log, want); return 1; }
    if (m->Map().Dirty || g_sortFirst != m->Map()._M_start || g_sortLast != m->Map()._M_finish) { puts("map"); return 1; }
    if (g_key != 0xC0DE || g_entryName != Raw(init.Name) || g_entryAlloc != init.AllocFunc) { puts("entry"); return 1; }
    return 0;
}

static CDefClassBase* FABLE_FASTCALL Alloc() { return 0; }

int main()
{
    static unsigned char storage[sizeof(Probe)];
    memset(storage, 0, sizeof(storage));
    Probe* m = reinterpret_cast<Probe*>(storage);

    CCharStringData data;
    data.Data.PData = (char*)"FRONT_END";
    data.Data.StringLength = 9;
    unsigned char initStore[sizeof(CDefClassInit)];
    CDefClassInit* init = reinterpret_cast<CDefClassInit*>(initStore);
    *reinterpret_cast<CCharStringData**>(&init->Name) = &data;
    init->AllocFunc = &Alloc;
    g_expectName = &init->Name;

    static unsigned char defInfoStore[3][sizeof(CDefinitionManager::CInstantiatedDefInfo)];
    CDefinitionManager::CInstantiatedDefInfo* defs[3];
    for (int i = 0; i < 3; ++i) defs[i] = reinterpret_cast<CDefinitionManager::CInstantiatedDefInfo*>(defInfoStore[i]);
    memset(defInfoStore, 0, sizeof(defInfoStore));
    defs[2]->PDef.Object = &g_def;
    m->Defs()._M_start = defs;
    m->Defs()._M_finish = defs + 3;
    g_vtable[1] = (void*)&FakeDelete;
    g_vtable[18] = (void*)&FakeTransfer;
    g_def.vptr = g_vtable;

    // Compiled defs, no checksums: register only.
    Probe::Flags(true, false);
    if (Run(m, *init, "progress;copyidx;insert;sort;")) return 1;
    if (g_crcData != data.Data.PData || g_crcLength != 9) { puts("crc"); return 1; }

    // Compiling from text: the NULLDEF_ default is built and transferred.
    g_def.RefCount = 1;
    Probe::Flags(false, false);
    if (Run(m, *init, "progress;copyidx;insert;sort;plus;inst;info;load;clear;transfer2;")) return 1;
    if (g_plusRight != &init->Name || g_def.RefCount != 1) { puts("reference"); return 1; }

    // Checksumming with compiled defs: the same, and the last reference deletes.
    g_def.RefCount = 0;
    Probe::Flags(true, true);
    if (Run(m, *init, "progress;copyidx;insert;sort;plus;inst;info;load;clear;transfer1;delete;")) return 1;
    puts("FABLETLC_CDEFINITIONMANAGER_ADDDEFCLASS_009B0AC0_PASS");
    return 0;
}