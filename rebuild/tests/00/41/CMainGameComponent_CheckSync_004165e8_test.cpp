// Behaviour fixture for CMainGameComponent::CheckSync @ 0x004165E8.
// Checks: the world checksum is read once up front; the package's event
// count is re-read every iteration; events are fetched in index order; only
// GAME_EVENT_UPDATE_FRAME events are unpacked, into one data package (built
// zeroed) with three 4-byte reads each; nothing on the component changes.
// Cases: mixed event types, no events.
#include "fable_game.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c, const CGameEventPackage* p) { static_cast<MainGameProbe*>(c)->CheckSync(p); }
};

static int seq;
static bool ok;
static unsigned char compStorage[0x161E8 + 16];
static unsigned char* comp = compStorage;
static unsigned char packageStorage[16];

static CGameEvent events[8];
static unsigned long noEvents;
static int checksumCalls, checksumAt, countCalls, getEventCalls, packageCalls, dataCalls;
static unsigned long nextIndex;
static const CGameEventDataPackage* packageAddr;
static const void* lastEvent;
static void* dataDest[3];

unsigned long CMainGameComponent::PeekWorldChecksum() const
{
    ++checksumCalls;
    checksumAt = ++seq;
    ok = ok && static_cast<const void*>(this) == comp && countCalls == 0;
    return 0xABCD;
}
unsigned long CGameEventPackage::GetNoEvents() const
{
    ++countCalls;
    ok = ok && static_cast<const void*>(this) == packageStorage;
    return noEvents;
}
const CGameEvent* CGameEventPackage::GetEvent(unsigned long index) const
{
    ++getEventCalls;
    ok = ok && static_cast<const void*>(this) == packageStorage && index == nextIndex;
    ++nextIndex;
    lastEvent = &events[index & 7];
    return &events[index & 7];
}
void CGameEvent::GetDataPackage(CGameEventDataPackage* package) const
{
    ++packageCalls;
    ok = ok && this == lastEvent && Type == GAME_EVENT_UPDATE_FRAME;
    if (packageAddr == 0)
    {
        packageAddr = package;
        ok = ok && package->Length == 0 && package->StartPos == 0 && package->EndPos == 0;
    }
    ok = ok && package == packageAddr && dataCalls % 3 == 0;
}
void CGameEventDataPackage::GetData(long size, void* data)
{
    ok = ok && this == packageAddr && size == 4 && data != 0;
    dataDest[dataCalls % 3] = data;
    if (dataCalls % 3 == 2)
        ok = ok && dataDest[0] != dataDest[1] && dataDest[1] != dataDest[2] && dataDest[0] != dataDest[2];
    *static_cast<unsigned long*>(data) = 0x100 + dataCalls;
    ++dataCalls;
}

static bool Run(const long* types, unsigned long n)
{
    memset(compStorage, 0xA5, sizeof(compStorage));
    unsigned char before[sizeof(compStorage)];
    memcpy(before, compStorage, sizeof(compStorage));
    int frames = 0;
    for (unsigned long i = 0; i < n; ++i)
    {
        events[i].Type = types[i];
        if (types[i] == GAME_EVENT_UPDATE_FRAME) ++frames;
    }
    noEvents = n;
    seq = checksumCalls = checksumAt = countCalls = getEventCalls = packageCalls = dataCalls = 0;
    nextIndex = 0;
    packageAddr = 0;
    lastEvent = 0;
    ok = true;

    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp),
        reinterpret_cast<const CGameEventPackage*>(packageStorage));

    if (checksumCalls != 1 || checksumAt != 1) ok = false;
    if (countCalls != (int)n + 1 || getEventCalls != (int)n) ok = false;
    if (packageCalls != frames || dataCalls != frames * 3) ok = false;
    if (memcmp(before, compStorage, sizeof(compStorage)) != 0) ok = false;
    return ok;
}

int main()
{
    static const long mixed[] = { GAME_EVENT_UPDATE_FRAME, 5, GAME_EVENT_UPDATE_FRAME, 0, 7 };
    bool pass = Run(mixed, 5);
    pass = Run(mixed, 0) && pass;
    static const long frameOnly[] = { GAME_EVENT_UPDATE_FRAME };
    pass = Run(frameOnly, 1) && pass;
    if (pass)
        printf("FABLETLC_MAINGAME_CHECKSYNC PASS\n");
    else
        printf("FABLETLC_MAINGAME_CHECKSYNC FAIL\n");
    return pass ? 0 : 1;
}