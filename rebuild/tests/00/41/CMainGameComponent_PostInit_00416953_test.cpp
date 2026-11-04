// Behaviour fixture for CMainGameComponent::PostInit @ 0x00416953.
// Raw retail-sized component: PWorld +0x24, PDisplayEngine +0x28,
// InitStructure +0x161D0 (InitialWorldName +0, InitialWorldHolySiteName +4,
// InitialQuestName +8, SaveGameName +0xC). CWorld is a test-local stand-in
// whose vtable slot 7 plays CWorld::PostInit(CDisplayEngine&); its
// non-virtual members carry the real mangled names. Strings are tracked by
// address, so every path the function builds is checked by value.
#include <stdio.h>
#include <string.h>
#include "fable_game.h"
#include "fable_profile.h"
#include "fable_progress_display.h"
#include "fable_quest_manager.h"
#include "fable_resource_directories.h"
#include "test_support/base_class_stubs.h"

// ---- shared recording stand-ins (CCharString + NProgressDisplay) ----
namespace
{
    int g_seq;
    struct Event { int at; const char* what; const void* self; const void* a; long b; };
    Event g_ev[128];
    int g_nev;
    void Ev(const char* what, const void* self, const void* a, long b)
    {
        if (g_nev < 128)
        {
            Event& e = g_ev[g_nev++];
            e.at = ++g_seq; e.what = what; e.self = self; e.a = a; e.b = b;
        }
    }
    // Live narrow strings: address -> literal text.
    const void* g_strSelf[64];
    const char* g_strText[64];
    int g_nstr;
    void StrBind(const void* self, const char* text)
    {
        for (int i = 0; i < g_nstr; ++i)
            if (g_strSelf[i] == self) { g_strText[i] = text; return; }
        g_strSelf[g_nstr] = self; g_strText[g_nstr] = text; ++g_nstr;
    }
    const char* StrText(const void* self)
    {
        for (int i = 0; i < g_nstr; ++i)
            if (g_strSelf[i] == self) return g_strText[i];
        return "<unbound>";
    }
    int Fail(int line)
    {
        printf("FAIL line %d\n", line);
        return 1;
    }
    bool Is(int i, const char* what) { return i < g_nev && strcmp(g_ev[i].what, what) == 0; }
}
#define CHECK(c) do { if (!(c)) return Fail(__LINE__); } while (0)

CCharString::CCharString(const char* text, long length)
{
    StrBind(this, text);
    Ev("CCharString(const char*)", this, text, length);
}
CCharString::CCharString(const CCharString& other)
{
    StrBind(this, StrText(&other));
    Ev("CCharString(copy)", this, &other, 0);
}
CCharString::~CCharString()
{
    Ev("~CCharString", this, StrText(this), 0);
}

float g_lastFraction;
void FABLE_FASTCALL NProgressDisplay::DisplayProgress(
    const CCharString& text, float fraction, bool forceRender, bool isDebug)
{
    g_lastFraction = fraction;
    Ev("DisplayProgress", &text, StrText(&text),
       (fraction == -1.0f ? 1 : 0) | (forceRender ? 2 : 0) | (isDebug ? 4 : 0));
}


// ---- wide strings, tracked by address ----
namespace
{
    struct WSlot { const void* self; wchar_t text[96]; };
    WSlot g_w[64];
    int g_nw;
    WSlot* WFind(const void* self, bool create)
    {
        for (int i = 0; i < g_nw; ++i)
            if (g_w[i].self == self) return &g_w[i];
        if (!create || g_nw >= 64) return 0;
        g_w[g_nw].self = self; g_w[g_nw].text[0] = 0;
        return &g_w[g_nw++];
    }
    void WSet(const void* self, const wchar_t* text)
    {
        WSlot* s = WFind(self, true);
        int i = 0;
        for (; text[i] && i < 95; ++i) s->text[i] = text[i];
        s->text[i] = 0;
    }
    void WSetN(const void* self, const char* text)
    {
        WSlot* s = WFind(self, true);
        int i = 0;
        for (; text[i] && i < 95; ++i) s->text[i] = (wchar_t)text[i];
        s->text[i] = 0;
    }
    const wchar_t* WText(const void* self)
    {
        WSlot* s = WFind(self, false);
        static wchar_t empty[1] = { 0 };
        return s ? s->text : empty;
    }
    bool WEq(const void* self, const char* text)
    {
        const wchar_t* w = WText(self);
        int i = 0;
        for (; text[i]; ++i) if (w[i] != (wchar_t)text[i]) return false;
        return w[i] == 0;
    }
    long WLen(const wchar_t* w) { long n = 0; while (w[n]) ++n; return n; }
    // Silences the stand-ins' own return-by-value locals.
    int g_quiet;
    struct Quiet { Quiet() { ++g_quiet; } ~Quiet() { --g_quiet; } };
}

CWideString::CWideString() { WSet(this, L""); }
CWideString::CWideString(const CWideString& other) { WSet(this, WText(&other)); }
CWideString::~CWideString() { if (!g_quiet) Ev("~CWideString", this, 0, 0); WSet(this, L""); }
long CWideString::GetLength() const { return WLen(WText(this)); }
CWideString& CWideString::operator=(const CWideString& other)
{
    Ev("CWideString::operator=", this, &other, 0);
    WSet(this, WText(&other));
    return *this;
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right)
{
    Ev("operator+(W,W)", &left, &right, 0);
    wchar_t buf[96];
    long n = 0;
    for (const wchar_t* p = WText(&left); *p; ++p) buf[n++] = *p;
    for (const wchar_t* p = WText(&right); *p; ++p) buf[n++] = *p;
    buf[n] = 0;
    Quiet q;
    CWideString r;
    WSet(&r, buf);
    return r;
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right)
{
    Ev("operator+(W,wchar_t*)", &left, right, 0);
    wchar_t buf[96];
    long n = 0;
    for (const wchar_t* p = WText(&left); *p; ++p) buf[n++] = *p;
    for (const wchar_t* p = right; *p; ++p) buf[n++] = *p;
    buf[n] = 0;
    Quiet q;
    CWideString r;
    WSet(&r, buf);
    return r;
}
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetLevelDir()
{
    Ev("GetLevelDir", 0, 0, 0);
    Quiet q;
    CWideString r;
    WSet(&r, L"Data\\Levels\\");
    return r;
}

bool CCharString::operator!=(const char* other) const
{
    Ev("CCharString::operator!=", this, other, 0);
    return strcmp(StrText(this), other) != 0;
}

void FABLE_FASTCALL NProfileTimer::StartProfile(const CCharString& label, long profileId)
{
    Ev("StartProfile", &label, StrText(&label), profileId);
}
void FABLE_FASTCALL NProfileTimer::EndProfile(long profileId)
{
    Ev("EndProfile", 0, 0, profileId);
}

// ---- CWorld stand-in (slot 7 = PostInit) ----
class CWorld
{
public:
    virtual void V0() {}
    virtual void V1() {}
    virtual void V2() {}
    virtual void V3() {}
    virtual void V4() {}
    virtual void V5() {}
    virtual void V6() {}
    virtual void PostInit(CDisplayEngine& displayEngine);
    virtual void V8() {}

    void Load(const CWideString& worldName);
    bool LoadGameState(const CWideString& saveName, bool flag);
    void ActivateWorld(bool flag);
    CWideString ConvertWorldToStaticMapPath(const CWideString& worldName) const;
    CWideString ConvertWorldToStaticMapPath(const CWideString& worldName, bool flag1, bool flag2) const;
    CWideString ConvertWorldToISMThingyPath(const CWideString& worldName) const;
    CWideString ConvertWorldToISMThingyPath(const CWideString& worldName, bool flag1, bool flag2) const;
};

namespace
{
    bool g_loadGameStateResult;
    wchar_t g_loadedWorld[96];
    const void* g_loadedWorldObj;
    CWideString PathOf(const char* prefix, const CWideString& world, bool f1, bool f2, int nArgs)
    {
        char tag[16];
        if (nArgs == 1) sprintf(tag, "%s:", prefix);
        else sprintf(tag, "%s%d%d:", prefix, f1 ? 1 : 0, f2 ? 1 : 0);
        wchar_t buf[96];
        long n = 0;
        for (const char* p = tag; *p; ++p) buf[n++] = (wchar_t)*p;
        for (const wchar_t* p = WText(&world); *p; ++p) buf[n++] = *p;
        buf[n] = 0;
        Quiet q;
        CWideString r;
        WSet(&r, buf);
        return r;
    }
}

void CWorld::PostInit(CDisplayEngine& displayEngine) { Ev("CWorld::PostInit", this, &displayEngine, 0); }
void CWorld::Load(const CWideString& worldName)
{
    Ev("CWorld::Load", this, &worldName, 0);
    g_loadedWorldObj = &worldName;
    const wchar_t* t = WText(&worldName);
    int i = 0;
    for (; t[i]; ++i) g_loadedWorld[i] = t[i];
    g_loadedWorld[i] = 0;
}
bool CWorld::LoadGameState(const CWideString& saveName, bool flag)
{
    Ev("CWorld::LoadGameState", this, &saveName, flag ? 1 : 0);
    return g_loadGameStateResult;
}
void CWorld::ActivateWorld(bool flag) { Ev("CWorld::ActivateWorld", this, 0, flag ? 1 : 0); }
CWideString CWorld::ConvertWorldToStaticMapPath(const CWideString& w) const
{ Ev("StaticMapPath/1", this, &w, 0); return PathOf("SM", w, false, false, 1); }
CWideString CWorld::ConvertWorldToStaticMapPath(const CWideString& w, bool f1, bool f2) const
{ Ev("StaticMapPath/3", this, &w, (f1 ? 1 : 0) | (f2 ? 2 : 0)); return PathOf("SM", w, f1, f2, 3); }
CWideString CWorld::ConvertWorldToISMThingyPath(const CWideString& w) const
{ Ev("ISMPath/1", this, &w, 0); return PathOf("ISM", w, false, false, 1); }
CWideString CWorld::ConvertWorldToISMThingyPath(const CWideString& w, bool f1, bool f2) const
{ Ev("ISMPath/3", this, &w, (f1 ? 1 : 0) | (f2 ? 2 : 0)); return PathOf("ISM", w, f1, f2, 3); }

// ---- files ----
class CFileInformation { public: int unused; };
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CAFile::CAFile() {}

CFileInformation CAFile::GetFileInformation() { CFileInformation f; f.unused = 0; return f; }
namespace { wchar_t g_filePath[8][96]; const void* g_fileSelf[8]; int g_nfile; }
CDiskFileWin32::CDiskFileWin32(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    Ev("CDiskFileWin32", this, &pathname, (long)accessType * 16 + (long)flags);
    g_fileSelf[g_nfile] = this;
    const wchar_t* t = WText(&pathname);
    int i = 0;
    for (; t[i]; ++i) g_filePath[g_nfile][i] = t[i];
    g_filePath[g_nfile][i] = 0;
    ++g_nfile;
}
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() { Ev("CDiskFileWin32::Close", this, 0, 0); }
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { CFileInformation f; f.unused = 0; return f; }
void FABLE_FASTCALL CAFile::CopyFileW(CAFile& source, CAFile& destination)
{
    Ev("CopyFileW", &source, &destination, 0);
}

// ---- quests, LUA, globals ----
template<typename T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template<>
void CCountedPointer<CQuestManager>::Reset(CQuestManager* data) { Data = data; }
bool CQuestManager::ActivateQuest(const CCharString& questName, bool flag1, bool flag2)
{
    Ev("ActivateQuest", this, &questName, (flag1 ? 1 : 0) | (flag2 ? 2 : 0));
    return true;
}
void CMainGameComponent::InitLUA(const CCharString& scriptName)
{
    Ev("InitLUA", this, &scriptName, 0);
}

CWideString g_FableForceLevel_013B8668;
CWideString g_FableForceStartingHolySite_013B866C;
bool g_FableSaveGameLaunchFailed_013B89C0;
bool GUseRetailBanks;
CCharString g_FableRunLUAScript_013B8674("script.lua", -1);
fable_u8 g_FableCompileFrontendDefinitions_013B8648;

struct Access : CMainGameComponent
{
    static void Run(CMainGameComponent* c) { static_cast<Access*>(c)->CMainGameComponent::PostInit(); }
};

namespace
{
    unsigned long g_storage[sizeof(CMainGameComponent) / 4];
    unsigned char g_before[sizeof(g_storage)];
    unsigned char* g_base = reinterpret_cast<unsigned char*>(g_storage);
    CWorld g_world;
    unsigned char g_displayEngine[16];
    CQuestManager g_questManager;

    const void* Init(int off) { return g_base + 0x161D0 + off; }

    void Setup(const char* save, const char* world, const char* holy, const char* quest,
               const char* forceLevel, const char* forceHoly, bool loadOk, bool build, bool retailBanks)
    {
        memset(g_storage, 0, sizeof(g_storage));
        CWorld* w = &g_world;
        void* d = g_displayEngine;
        memcpy(g_base + 0x24, &w, 4);
        memcpy(g_base + 0x28, &d, 4);
        g_nw = 0; g_nstr = 0; g_nev = 0; g_seq = 0; g_nfile = 0;
        WSetN(Init(0x0), world);
        WSetN(Init(0x4), holy);
        StrBind(Init(0x8), quest);
        WSetN(Init(0xC), save);
        WSetN(&g_FableForceLevel_013B8668, forceLevel);
        WSetN(&g_FableForceStartingHolySite_013B866C, forceHoly);
        StrBind(&g_FableRunLUAScript_013B8674, "script.lua");
        g_loadGameStateResult = loadOk;
        g_FableSaveGameLaunchFailed_013B89C0 = false;
        g_FableCompileFrontendDefinitions_013B8648 = build ? 1 : 0;
        GUseRetailBanks = retailBanks;
        g_loadedWorld[0] = 0;
        g_loadedWorldObj = 0;
        CTBaseSingletonNoDefaultAlloc<CQuestManager>::Set(&g_questManager);
        memcpy(g_before, g_storage, sizeof(g_storage));
        Access::Run(reinterpret_cast<CMainGameComponent*>(g_storage));
    }

    int ProgressAt(int& i, const char* text)
    {
        CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, text) == 0 && g_ev[i].b == -1);
        const void* s = g_ev[i].self; ++i;
        CHECK(Is(i, "DisplayProgress") && g_ev[i].self == s && g_ev[i].b == 1); ++i;
        CHECK(Is(i, "~CCharString") && g_ev[i].self == s); ++i;
        return 0;
    }

    int Find(int from, const char* what)
    {
        for (int i = from; i < g_nev; ++i) if (strcmp(g_ev[i].what, what) == 0) return i;
        return -1;
    }
    int Count(const char* what)
    {
        int n = 0;
        for (int i = 0; i < g_nev; ++i) if (strcmp(g_ev[i].what, what) == 0) ++n;
        return n;
    }
    bool StorageUntouched()
    {
        return memcmp(g_storage, g_before, sizeof(g_storage)) == 0;
    }
    bool WorldLoaded(const char* expected)
    {
        int i = 0;
        for (; expected[i]; ++i) if (g_loadedWorld[i] != (wchar_t)expected[i]) return false;
        return g_loadedWorld[i] == 0;
    }
}

static int CaseSave(bool loadOk)
{
    Setup("hero.sav", "Oakvale.wld", "", "Q1", "", "", loadOk, false, false);
    int i = 0;
    CHECK(Is(i, "CWorld::PostInit") && g_ev[i].self == &g_world && g_ev[i].a == g_displayEngine); ++i;
    if (ProgressAt(i, "Loading save")) return 1;
    CHECK(Is(i, "CWorld::LoadGameState") && g_ev[i].self == &g_world && g_ev[i].a == Init(0xC) && g_ev[i].b == 1); ++i;
    CHECK(Is(i, "InitLUA") && g_ev[i].self == g_base && g_ev[i].a == &g_FableRunLUAScript_013B8674); ++i;
    CHECK(i == g_nev);
    CHECK(g_FableSaveGameLaunchFailed_013B89C0 == !loadOk);
    CHECK(WEq(&g_FableForceLevel_013B8668, ""));
    CHECK(StorageUntouched());
    return 0;
}

static int CaseWorld(const char* world, const char* holy, const char* forceLevel, const char* quest,
                     const char* expectedWorld, const char* expectedForceLevel, const char* expectedForceHoly)
{
    Setup("", world, holy, quest, forceLevel, "OldHoly", true, false, false);
    int i = 0;
    CHECK(Is(i, "CWorld::PostInit") && g_ev[i].self == &g_world && g_ev[i].a == g_displayEngine); ++i;
    if (ProgressAt(i, "Loading world")) return 1;
    CHECK(Is(i, "GetLevelDir")); ++i;
    const bool fromInit = world[0] != 0;
    const bool fromForce = !fromInit && forceLevel[0] != 0;
    if (fromInit)
    {
        CHECK(Is(i, "operator+(W,W)") && g_ev[i].b == 0 && g_ev[i].a == Init(0x0)); ++i;
    }
    else if (fromForce)
    {
        CHECK(Is(i, "operator+(W,W)") && g_ev[i].a == &g_FableForceLevel_013B8668); ++i;
    }
    else
    {
        CHECK(Is(i, "operator+(W,wchar_t*)")); ++i;
    }
    // worldName = <sum>; the two temporaries die.
    int assign = Find(i, "CWideString::operator=");
    CHECK(assign >= 0);
    const void* worldName = g_ev[assign].self;
    CHECK(worldName != &g_FableForceLevel_013B8668 && worldName != &g_FableForceStartingHolySite_013B866C);
    i = assign + 1;
    CHECK(Is(i, "~CWideString")); ++i;
    CHECK(Is(i, "~CWideString")); ++i;
    if (fromInit)
    {
        CHECK(Is(i, "CWideString::operator=") && g_ev[i].self == &g_FableForceLevel_013B8668 && g_ev[i].a == Init(0x0)); ++i;
        if (holy[0])
        {
            CHECK(Is(i, "CWideString::operator=") && g_ev[i].self == &g_FableForceStartingHolySite_013B866C && g_ev[i].a == Init(0x4)); ++i;
        }
    }
    CHECK(Is(i, "CWorld::Load") && g_ev[i].self == &g_world && g_ev[i].a == worldName); ++i;
    CHECK(WorldLoaded(expectedWorld));
    CHECK(Is(i, "CWorld::ActivateWorld") && g_ev[i].self == &g_world && g_ev[i].b == 0); ++i;
    CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, "Activate Initial Quests") == 0);
    const void* label = g_ev[i].self; ++i;
    CHECK(Is(i, "StartProfile") && g_ev[i].self == label && g_ev[i].b == 0); ++i;
    CHECK(Is(i, "~CCharString") && g_ev[i].self == label); ++i;
    CHECK(Is(i, "CCharString::operator!=") && g_ev[i].self == Init(0x8) && strcmp((const char*)g_ev[i].a, "") == 0); ++i;
    if (quest[0])
    {
        CHECK(Is(i, "ActivateQuest") && g_ev[i].self == &g_questManager && g_ev[i].a == Init(0x8) && g_ev[i].b == 2); ++i;
    }
    CHECK(Is(i, "EndProfile") && g_ev[i].b == 0); ++i;
    CHECK(Is(i, "~CWideString") && g_ev[i].self == worldName); ++i;
    CHECK(Is(i, "InitLUA") && g_ev[i].self == g_base && g_ev[i].a == &g_FableRunLUAScript_013B8674); ++i;
    CHECK(i == g_nev);
    CHECK(WEq(&g_FableForceLevel_013B8668, expectedForceLevel));
    CHECK(WEq(&g_FableForceStartingHolySite_013B866C, expectedForceHoly));
    CHECK(!g_FableSaveGameLaunchFailed_013B89C0);
    CHECK(Count("CopyFileW") == 0 && Count("CWorld::LoadGameState") == 0);
    CHECK(StorageUntouched());
    return 0;
}

static int CaseBuild(bool retailBanks)
{
    Setup("", "Oakvale.wld", "", "Q1", "", "", true, true, retailBanks);
    const long flags3 = 1 | (retailBanks ? 2 : 0);
    int i = Find(0, "CWorld::Load");
    CHECK(i > 0);
    const void* worldName = g_ev[i].a;
    CHECK(WorldLoaded("Data\\Levels\\Oakvale.wld"));
    ++i;
    CHECK(Is(i, "StaticMapPath/1") && g_ev[i].self == &g_world && g_ev[i].a == worldName); ++i;
    CHECK(Is(i, "CDiskFileWin32") && g_ev[i].b == 1 * 16 + 2); const void* smSrc = g_ev[i].self; ++i;
    CHECK(Is(i, "StaticMapPath/3") && g_ev[i].a == worldName && g_ev[i].b == flags3); ++i;
    CHECK(Is(i, "CDiskFileWin32") && g_ev[i].b == 4 * 16 + 2); const void* smDst = g_ev[i].self; ++i;
    CHECK(Is(i, "CopyFileW") && g_ev[i].self == smSrc && g_ev[i].a == smDst); ++i;
    CHECK(Is(i, "ISMPath/1") && g_ev[i].self == &g_world && g_ev[i].a == worldName); ++i;
    CHECK(Is(i, "CDiskFileWin32") && g_ev[i].b == 1 * 16 + 2); const void* ismSrc = g_ev[i].self; ++i;
    CHECK(Is(i, "ISMPath/3") && g_ev[i].a == worldName && g_ev[i].b == flags3); ++i;
    CHECK(Is(i, "CDiskFileWin32") && g_ev[i].b == 4 * 16 + 2); const void* ismDst = g_ev[i].self; ++i;
    CHECK(Is(i, "CopyFileW") && g_ev[i].self == ismSrc && g_ev[i].a == ismDst); ++i;
    // Files open on the converted paths.
    char expect[64];
    sprintf(expect, "SM1%d:Data\\Levels\\Oakvale.wld", retailBanks ? 1 : 0);
    {
        const char* e[4] = { "SM:Data\\Levels\\Oakvale.wld", expect, "ISM:Data\\Levels\\Oakvale.wld", 0 };
        char ism3[64];
        sprintf(ism3, "ISM1%d:Data\\Levels\\Oakvale.wld", retailBanks ? 1 : 0);
        e[3] = ism3;
        CHECK(g_nfile == 4);
        for (int f = 0; f < 4; ++f)
        {
            int k = 0;
            for (; e[f][k]; ++k) CHECK(g_filePath[f][k] == (wchar_t)e[f][k]);
            CHECK(g_filePath[f][k] == 0);
        }
    }
    // Everything dies in reverse, each file's Close before its path string.
    const void* order[4] = { ismDst, ismSrc, smDst, smSrc };
    for (int f = 0; f < 4; ++f)
    {
        int c = Find(i, "CDiskFileWin32::Close");
        CHECK(c >= 0 && g_ev[c].self == order[f]);
        i = c + 1;
    }
    // The last event is worldName's destructor: no ActivateWorld, no quests, no InitLUA.
    CHECK(Is(g_nev - 1, "~CWideString") && g_ev[g_nev - 1].self == worldName);
    CHECK(Count("CWorld::ActivateWorld") == 0 && Count("StartProfile") == 0);
    CHECK(Count("ActivateQuest") == 0 && Count("InitLUA") == 0);
    CHECK(StorageUntouched());
    return 0;
}

int main()
{
    if (CaseSave(true)) return 1;
    if (CaseSave(false)) return 1;
    // Component world with a holy site: both console globals are overwritten.
    if (CaseWorld("Oakvale.wld", "HolySite", "Forced.wld", "Q1",
                  "Data\\Levels\\Oakvale.wld", "Oakvale.wld", "HolySite")) return 1;
    // Component world without a holy site: the forced holy site stays.
    if (CaseWorld("Oakvale.wld", "", "", "",
                  "Data\\Levels\\Oakvale.wld", "Oakvale.wld", "OldHoly")) return 1;
    // No component world: the console-forced level.
    if (CaseWorld("", "HolySite", "Forced.wld", "Q2",
                  "Data\\Levels\\Forced.wld", "Forced.wld", "OldHoly")) return 1;
    // Nothing requested: updatedscenic.wld.
    if (CaseWorld("", "", "", "",
                  "Data\\Levels\\updatedscenic.wld", "", "OldHoly")) return 1;
    if (CaseBuild(false)) return 1;
    if (CaseBuild(true)) return 1;
    puts("FABLETLC_MAINGAME_POSTINIT_00416953_PASS");
    return 0;
}
