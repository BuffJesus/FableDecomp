// Behaviour fixture for CMainGameComponent's PostChangeResolution, event
// package file setup (loading and saving) and ProcessEventPackage. The members
// are the real compiled providers; the component is raw storage addressed by
// retail offsets (PWorld +0x24, PDisplayEngine +0x28, EventPackageFile
// +0x16154 with a recording vtable, Loading/Saving flags +0x16170/+0x16171,
// header +0x16174). Callees are recording stand-ins.
#pragma optimize("s", on)
#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_world.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static bool Load(CMainGameComponent* c, const wchar_t* f) { return static_cast<MainGameProbe*>(c)->InitEventPackageLoading(f); }
    static bool Save(CMainGameComponent* c, const wchar_t* f) { return static_cast<MainGameProbe*>(c)->InitEventPackageSaving(f); }
    static void Process(CMainGameComponent* c, const CGameEventPackage* p) { static_cast<MainGameProbe*>(c)->ProcessEventPackage(p); }
};

static unsigned char comp[0x161F0];
template <class T> static T& At(unsigned offset) { return *reinterpret_cast<T*>(comp + offset); }
static char g_trace[64]; static int g_traceSize;
static void Record(char c) { g_trace[g_traceSize++] = c; g_trace[g_traceSize] = 0; }
static unsigned char displayEngine[16], world[16];
static unsigned long g_args[3];

void CDisplayEngine::PostChangeResolution(unsigned long w, unsigned long h, unsigned long d)
{
    Record(this == reinterpret_cast<CDisplayEngine*>(displayEngine) ? 'D' : '?'); g_args[0] = w; g_args[1] = h; g_args[2] = d;
}
void CWorld::ChangeResolution(unsigned long w, unsigned long h, unsigned long d)
{
    Record(this == reinterpret_cast<CWorld*>(world) ? 'W' : '?'); if (w != g_args[0] || h != g_args[1] || d != g_args[2]) Record('!');
}
static const wchar_t* g_constructedFrom; static int g_strings;
CWideString::CWideString(const wchar_t* text) { g_constructedFrom = text; ++g_strings; storage_ = 0; }
CWideString::~CWideString() { --g_strings; }
static int g_events[8]; static unsigned long g_noEvents, g_noEventsCalls; static int g_processed[8], g_processedCount;
unsigned long CGameEventPackage::GetNoEvents() const { ++g_noEventsCalls; return g_noEvents; }
const CGameEvent* CGameEventPackage::GetEvent(unsigned long index) const { return reinterpret_cast<const CGameEvent*>(&g_events[index]); }
void CMainGameComponent::ProcessEvent(const CGameEvent* event) { g_processed[g_processedCount++] = (int)(reinterpret_cast<const int*>(event) - g_events); }

// The package file's vtable: slot 1 Open, slot 3 Read, slot 4 Write.
static int g_opens, g_reads, g_writes; static FileAccessType g_access; static unsigned long g_flags, g_size; static const void* g_data; static int g_stringsDuringOpen; static long g_headerAtWrite;
static void FABLE_FASTCALL FileOpen(void* self, void*, const CWideString&, FileAccessType access, unsigned long flags)
{ ++g_opens; g_access = access; g_flags = flags; g_stringsDuringOpen = g_strings; if (self != &At<char>(0x16154)) Record('?'); }
static void FABLE_FASTCALL FileRead(void*, void*, void* data, long size, CASuspendableProcess*) { ++g_reads; g_data = data; g_size = size; *static_cast<long*>(data) = 0x55; }
static void FABLE_FASTCALL FileWrite(void*, void*, const void* data, long size, CASuspendableProcess*) { ++g_writes; g_data = data; g_size = size; g_headerAtWrite = *static_cast<const long*>(data); }
static void* g_fileTable[16];

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("MAIN_GAME_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)

int main()
{
    int cases = 0;
    g_fileTable[1] = (void*)FileOpen; g_fileTable[3] = (void*)FileRead; g_fileTable[4] = (void*)FileWrite;
    static unsigned char before[sizeof(comp)];
    // PostChangeResolution: display engine first, then the world, same three values.
    for (unsigned long w = 320; w <= 1600; w += 640) for (unsigned long d = 16; d <= 32; d += 16) {
        memset(comp, 0x4D, sizeof(comp)); At<void*>(0x24) = world; At<void*>(0x28) = displayEngine; memcpy(before, comp, sizeof(comp));
        g_traceSize = 0; g_trace[0] = 0;
        reinterpret_cast<CMainGameComponent*>(comp)->PostChangeResolution(w, w * 3 / 4, d);
        CHECK(strcmp(g_trace, "DW") == 0 && g_args[0] == w && g_args[1] == w * 3 / 4 && g_args[2] == d && memcmp(before, comp, sizeof(comp)) == 0, 1);
        ++cases;
    }
    // Event package files: the name lives only for the Open call.
    static const wchar_t name[] = L"events.pkg";
    for (int saving = 0; saving < 2; ++saving) for (int round = 0; round < 3; ++round) {
        memset(comp, 0x4D + round, sizeof(comp)); *reinterpret_cast<void**>(&At<char>(0x16154)) = g_fileTable; memcpy(before, comp, sizeof(comp));
        g_opens = g_reads = g_writes = 0; g_strings = 0; g_traceSize = 0; g_trace[0] = 0;
        bool result = saving ? MainGameProbe::Save(reinterpret_cast<CMainGameComponent*>(comp), name) : MainGameProbe::Load(reinterpret_cast<CMainGameComponent*>(comp), name);
        CHECK(result && g_opens == 1 && g_constructedFrom == name && g_stringsDuringOpen == 1 && g_strings == 0 && g_flags == 2 && g_trace[0] == 0, 2);
        CHECK(g_data == &At<char>(0x16174) && g_size == 4, 3);
        if (saving) CHECK(g_access == FILE_ACCESS_CREATE && g_writes == 1 && g_reads == 0 && g_headerAtWrite == 1 && At<bool>(0x16171) && At<long>(0x16174) == 1, 4);
        else CHECK(g_access == FILE_ACCESS_READ && g_reads == 1 && g_writes == 0 && At<bool>(0x16170) && At<long>(0x16174) == 0x55, 5);
        // Only the flag and header change.
        memcpy(&before[0x16174], &At<char>(0x16174), 4); before[saving ? 0x16171 : 0x16170] = 1;
        CHECK(memcmp(before, comp, sizeof(comp)) == 0, 6);
        ++cases;
    }
    // ProcessEventPackage: every event in order; the count is asked before each.
    for (unsigned long n = 0; n <= 6; ++n) {
        g_noEvents = n; g_noEventsCalls = 0; g_processedCount = 0;
        MainGameProbe::Process(reinterpret_cast<CMainGameComponent*>(comp), reinterpret_cast<const CGameEventPackage*>(g_events));
        CHECK(g_processedCount == (int)n && g_noEventsCalls == n + 1, 7);
        for (unsigned long i = 0; i < n; ++i) CHECK(g_processed[i] == (int)i, 8);
        ++cases;
    }
    if (g_fail) return 1;
    printf("MAIN_GAME_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
