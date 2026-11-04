// Behaviour fixture for CMainGameComponent::UpdateFromEventPackageSet @ 0x0041726D.
// Retail offsets: CurrentServerFrame +0x48, InputServerFrame +0x4C, PWorld
// +0x24, PDisplayEngine +0x28, Client +0x13AB8, WorldSeed +0x1613C,
// AllowRender +0x161E1. Starting with InputServerFrame 10 and
// CurrentServerFrame 12, packages stamped 8, 11, 12, 13, 13, 20 must:
// skip 8 (not newer); process 11 and 12 without render permission (input
// behind the server frame); process 13 with render permission (input caught
// up), setting AllowRender; skip the second 13; process 20 (input and server
// frame both 13 by then, so again with permission). For each processed
// package the order is ProcessEventPackage, CWorld::Update(package, allow),
// ConfirmFeedbackGameEventPackage, PeekWorldChecksum,
// CNetworkClient::Update(max(stamp, server frame), checksum, WorldSeed),
// WorldUpdate. The package count is re-read every iteration. An empty set
// makes no calls beyond one count. Nothing else in the component changes.
#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_game_event.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c, const CGameEventPackageSet* s) { static_cast<MainGameProbe*>(c)->UpdateFromEventPackageSet(s); }
};

static unsigned char comp[0x161F0];
static unsigned char packageSetStorage[16];
static unsigned char world[0x198];
static unsigned char display[0x100];
static CGameEventPackage packages[8];
static unsigned long noPackages;
static unsigned long nextIndex;
static int countCalls;
static bool ok;

static char log[512];
static int logLen;
static void Log(const char* s)
{
    size_t n = strlen(s);
    if (logLen + n < sizeof(log)) { memcpy(log + logLen, s, n); logLen += (int)n; log[logLen] = 0; }
}
static long Rd(unsigned off) { long v; memcpy(&v, comp + off, 4); return v; }
static void Wr(unsigned off, long v) { memcpy(comp + off, &v, 4); }

static const CGameEventPackage* current;

unsigned long CGameEventPackageSet::GetNoPackages() const
{
    ok = ok && static_cast<const void*>(this) == packageSetStorage;
    ++countCalls;
    return noPackages;
}
const CGameEventPackage* CGameEventPackageSet::GetPackage(unsigned long index) const
{
    ok = ok && static_cast<const void*>(this) == packageSetStorage && index == nextIndex;
    ++nextIndex;
    current = &packages[index];
    return current;
}
void CMainGameComponent::ProcessEventPackage(const CGameEventPackage* package)
{
    ok = ok && static_cast<void*>(this) == comp && package == current;
    char b[32]; sprintf(b, "P%ld ", package->TimeStamp); Log(b);
}
void CWorld::Update(const CGameEventPackage& package, bool allowRender)
{
    ok = ok && static_cast<void*>(this) == world && &package == current;
    // AllowRender is already set when permission is granted.
    ok = ok && (!allowRender || comp[0x161E1] == 1);
    char b[32]; sprintf(b, "W%d ", allowRender ? 1 : 0); Log(b);
}
void CDisplayEngine::ConfirmFeedbackGameEventPackage(const CGameEventPackage* package)
{
    ok = ok && static_cast<void*>(this) == display && package == current;
    // InputServerFrame / CurrentServerFrame are not advanced yet.
    ok = ok && Rd(0x4C) != package->TimeStamp;
    Log("C ");
}
unsigned long CMainGameComponent::PeekWorldChecksum() const
{
    ok = ok && static_cast<const void*>(this) == comp;
    ok = ok && Rd(0x4C) == current->TimeStamp;
    Log("K ");
    return 0x5EED0000 + current->TimeStamp;
}
void CNetworkClient::Update(long serverFrame, unsigned long worldChecksum, unsigned long worldSeed)
{
    ok = ok && static_cast<void*>(this) == comp + 0x13AB8 && serverFrame == Rd(0x48)
        && worldChecksum == 0x5EED0000 + (unsigned long)current->TimeStamp && worldSeed == 0x13579BDF;
    char b[32]; sprintf(b, "U%ld ", serverFrame); Log(b);
}
void CDisplayEngine::WorldUpdate()
{
    ok = ok && static_cast<void*>(this) == display;
    Log("D ");
}

static bool Run(const long* stamps, unsigned long n, const char* expectLog, long expectInput, long expectServer, bool expectAllow)
{
    memset(comp, 0x00, sizeof(comp));
    void* w = world; memcpy(comp + 0x24, &w, 4);
    void* d = display; memcpy(comp + 0x28, &d, 4);
    Wr(0x48, 12);
    Wr(0x4C, 10);
    Wr(0x1613C, 0x13579BDF);
    comp[0x161E1] = 0;
    static unsigned char expect[sizeof(comp)];
    memcpy(expect, comp, sizeof(comp));
    memcpy(expect + 0x48, &expectServer, 4);
    memcpy(expect + 0x4C, &expectInput, 4);
    expect[0x161E1] = expectAllow ? 1 : 0;
    for (unsigned long i = 0; i < n; ++i) packages[i].TimeStamp = stamps[i];
    noPackages = n;
    nextIndex = 0;
    countCalls = 0;
    logLen = 0; log[0] = 0;
    ok = true;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp),
        reinterpret_cast<const CGameEventPackageSet*>(packageSetStorage));
    ok = ok && strcmp(log, expectLog) == 0 && nextIndex == n && countCalls == (int)n + 1
        && memcmp(expect, comp, sizeof(comp)) == 0;
    if (!ok) printf("case n=%lu failed: log '%s'\n", n, log);
    return ok;
}

int main()
{
    static const long stamps[] = { 8, 11, 12, 13, 13, 20 };
    bool pass = Run(stamps, 6,
        "P11 W0 C K U12 D "
        "P12 W0 C K U12 D "
        "P13 W1 C K U13 D "
        "P20 W1 C K U20 D ",
        20, 20, true);
    pass = Run(stamps, 0, "", 10, 12, false) && pass;
    static const long stale[] = { 3, 10 };
    pass = Run(stale, 2, "", 10, 12, false) && pass;
    printf(pass ? "FABLETLC_MAINGAME_UPDATEFROMEVENTPACKAGESET_PASS\n" : "FABLETLC_MAINGAME_UPDATEFROMEVENTPACKAGESET_FAIL\n");
    return pass ? 0 : 1;
}