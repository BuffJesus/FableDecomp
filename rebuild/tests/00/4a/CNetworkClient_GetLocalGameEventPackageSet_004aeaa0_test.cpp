// Behaviour of CNetworkClient::GetLocalGameEventPackageSet against recording stand-ins:
// receiver/argument of every callee, call order, the synthesised event's bytes,
// LocalFrame/TimeStamp writes, and the not-yet-time branch.
#include "fable_game.h"
#include <stdio.h>
#include <string.h>

// Reach the private member through explicit instantiation (access is not checked there).
template<typename Tag, typename Tag::type M> struct Rob { friend typename Tag::type get(Tag) { return M; } };
struct GetLocalTag { typedef bool (CNetworkClient::*type)(CGameEventPackageSet*); friend type get(GetLocalTag); };
template struct Rob<GetLocalTag, &CNetworkClient::GetLocalGameEventPackageSet>;

static char g_log[256];
static int g_logLen;
static void Log(char c) { if (g_logLen < 255) g_log[g_logLen++] = c; g_log[g_logLen] = 0; }

static bool g_timeForUpdate;
static const void* g_componentThis;
static long g_frameAsked;
static unsigned long g_added[8]; static int g_addedCount; static const void* g_addThis;
static CGameEvent g_eventSeen; static const void* g_eventThis; static const void* g_eventArg;
static long g_stampAtAdd;
static const void* g_setClearThis; static const void* g_addPackageThis; static const void* g_addPackageArg;
static const void* g_pkgClearThis;
static long g_stampAtAddPackage;

bool CMainGameComponent::IsTimeForServerUpdate(long frame) const
{ Log('T'); g_componentThis = this; g_frameAsked = frame; return g_timeForUpdate; }
void CGameEvent::AddData(long size, const void* data)
{ Log('D'); g_addThis = this; if (size == 4 && g_addedCount < 8) g_added[g_addedCount++] = *static_cast<const unsigned long*>(data); }
void CGameEventPackage::AddEvent(const CGameEvent* event)
{ Log('E'); g_eventThis = this; g_eventArg = event; g_eventSeen = *event; g_stampAtAdd = TimeStamp; }
void CGameEventPackage::Clear() { Log('C'); g_pkgClearThis = this; }
void CGameEventPackageSet::Clear() { Log('S'); g_setClearThis = this; }
void CGameEventPackageSet::AddPackage(const CGameEventPackage* package)
{ Log('P'); g_addPackageThis = this; g_addPackageArg = package; g_stampAtAddPackage = package->TimeStamp; }

static unsigned char g_client[sizeof(CNetworkClient)];
static unsigned char g_component[16];
static unsigned char g_set[16];

static void Reset(bool time)
{
    memset(g_client, 0xCC, sizeof(g_client));
    *reinterpret_cast<long*>(g_client + 0x266C) = 41;           // LocalFrame
    *reinterpret_cast<unsigned long*>(g_client + 0x2670) = 0xC0DE0001;  // Checksum1
    *reinterpret_cast<unsigned long*>(g_client + 0x2674) = 0xC0DE0002;  // Checksum2
    *reinterpret_cast<void**>(g_client + 0x2678) = g_component; // GameComponent
    *reinterpret_cast<long*>(g_client + 0x2010) = 7;            // LocalEventPackage.TimeStamp
    g_timeForUpdate = time; g_logLen = 0; g_log[0] = 0; g_addedCount = 0;
    g_componentThis = g_eventThis = g_eventArg = g_setClearThis = g_addPackageThis = g_addPackageArg = g_pkgClearThis = 0;
}

int main()
{
    CNetworkClient* client = reinterpret_cast<CNetworkClient*>(g_client);
    CGameEventPackageSet* set = reinterpret_cast<CGameEventPackageSet*>(g_set);
    const void* package = g_client + 0x2010;

    // Not yet time: only the query, nothing written.
    Reset(false);
    if ((client->*get(GetLocalTag()))(set) != false) { printf("FAIL idle result\n"); return 1; }
    if (strcmp(g_log, "T") != 0 || g_componentThis != g_component || g_frameAsked != 41) { printf("FAIL idle calls %s\n", g_log); return 2; }
    if (*reinterpret_cast<long*>(g_client + 0x266C) != 41 || *reinterpret_cast<long*>(g_client + 0x2010) != 7) { printf("FAIL idle writes\n"); return 3; }

    // Time for the next server frame.
    Reset(true);
    if ((client->*get(GetLocalTag()))(set) != true) { printf("FAIL result\n"); return 4; }
    if (strcmp(g_log, "TDDDESPC") != 0) { printf("FAIL order %s\n", g_log); return 5; }
    if (g_addedCount != 3 || g_added[0] != 0xC0DE0001 || g_added[1] != 0xC0DE0002 || g_added[2] != 41) { printf("FAIL payload\n"); return 6; }
    if (g_eventThis != package || g_eventArg != g_addThis) { printf("FAIL AddEvent receiver\n"); return 7; }
    if (g_eventSeen.Type != GAME_EVENT_UPDATE_FRAME || g_eventSeen.Player != -1 || g_eventSeen.Valid != true || g_eventSeen.Replacement != false || g_eventSeen.EndPos != 0)
    { printf("FAIL event header\n"); return 8; }
    if (g_stampAtAdd != 7) { printf("FAIL stamp before add\n"); return 9; }
    if (*reinterpret_cast<long*>(g_client + 0x266C) != 42) { printf("FAIL LocalFrame\n"); return 10; }
    if (g_stampAtAddPackage != 42 || *reinterpret_cast<long*>(g_client + 0x2010) != 42) { printf("FAIL TimeStamp\n"); return 11; }
    if (g_setClearThis != set || g_addPackageThis != set || g_addPackageArg != package || g_pkgClearThis != package) { printf("FAIL set/package receivers\n"); return 12; }
    if (*reinterpret_cast<unsigned long*>(g_client + 0x2670) != 0xC0DE0001 || *reinterpret_cast<unsigned long*>(g_client + 0x2674) != 0xC0DE0002 || g_client[0x2662] != 0xCC)
    { printf("FAIL neighbours\n"); return 13; }

    printf("NETWORK_GET_LOCAL_PACKAGE_SET_PASS\n");
    return 0;
}