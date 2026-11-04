// Behaviour of CWorld::ActivateWorld against recording stand-ins: call order and
// receivers (player manager +0x0C, environment +0x18 -> interpolator +0x1964, GUI and
// quest singletons, the main game component), the hero re-init only when the
// controlled creature is missing or dead (flag forwarded), the quests array at
// +0xAC with (false, true), the type-9 event (payload byte 0, main player number),
// and FrameStarted3DRendering (+0x8C) = CWorld::Frame.
#include "fable_world.h"
#include "fable_game.h"
#include "fable_player_manager.h"
#include "fable_environment.h"
#include "fable_player_gui.h"
#include "fable_quest_manager.h"
#include "fable_profile.h"
#include <stdio.h>
#include <string.h>

long CWorld::Frame = 0;
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;
template<typename T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template<typename T> void CCountedPointer<T>::Reset(T* data) { Data = data; Info = 0; }

struct WorldPeek : public CWorld { static void SetFrame(long f) { Frame = f; } };
struct ComponentPeek : public CMainGameComponent { static void Set(CMainGameComponent* c) { PMainGameComponent = c; } };

static char g_log[64]; static int g_n;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

// CCharString stand-in: remembers the literal.
static const char* g_labels[4]; static int g_labelCount;
CCharString::CCharString(const char* text, long length) { *reinterpret_cast<const char**>(this) = text; (void)length; }
CCharString::~CCharString() {}
void FABLE_FASTCALL NProfileTimer::StartProfile(const CCharString& label, long profileId)
{ Log('P'); if (g_labelCount < 4) g_labels[g_labelCount++] = *reinterpret_cast<const char* const*>(&label); if (profileId != 0) Log('!'); }
void FABLE_FASTCALL NProfileTimer::EndProfile(long profileId) { Log('p'); if (profileId != 0) Log('!'); }

static unsigned char g_world[sizeof(CWorld)];
static unsigned char g_playerManager[sizeof(CPlayerManager)];
static unsigned char g_player[16];
static unsigned char g_creature[0x100];
static unsigned char g_environment[0x1970];
static unsigned char g_interpolator[16];
static unsigned char g_gui[0x200];
static unsigned char g_quests[16];
static unsigned char g_component[16];
static bool g_hasCreature;
static const void* g_this[16]; static int g_thisCount;
static bool g_initFlag; static const void* g_questArray; static bool g_questFlag1, g_questFlag2;
static unsigned char g_eventPayload; static CGameEvent g_eventSeen;

static void Seen(const void* p) { if (g_thisCount < 16) g_this[g_thisCount++] = p; }
CPlayer& CPlayerManager::GetMainPlayer() { Log('M'); Seen(this); return *reinterpret_cast<CPlayer*>(g_player); }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const
{ Log('C'); Seen(this); return g_hasCreature ? reinterpret_cast<CThingPlayerCreature*>(g_creature) : 0; }
void CPlayerManager::InitPlayerCharacter(bool flag) { Log('I'); Seen(this); g_initFlag = flag; }
long CPlayerManager::GetMainPlayerNumber() const { Log('N'); Seen(this); return 3; }
void CEnvironmentSpellEffectInterpolator::ClearScreenFilters() { Log('F'); Seen(this); }
void CPlayerGui::InitialActivate() { Log('G'); Seen(this); }
bool CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& names, bool f1, bool f2)
{ Log('Q'); Seen(this); g_questArray = &names; g_questFlag1 = f1; g_questFlag2 = f2; return true; }
void CQuestManager::PostBoastTransfer() { Log('B'); Seen(this); }
void CGameEvent::AddData(long size, const void* data) { Log('D'); if (size == 1) g_eventPayload = *static_cast<const unsigned char*>(data); }
void CMainGameComponent::AddGameEvent(const CGameEvent& event) { Log('E'); Seen(this); g_eventSeen = event; }

static int Run(bool hasCreature, bool dead, bool flag, const char* expectLog)
{
    memset(g_world, 0xCD, sizeof(g_world));
    *reinterpret_cast<void**>(g_world + 0x0C) = g_playerManager;
    *reinterpret_cast<void**>(g_world + 0x18) = g_environment;
    *reinterpret_cast<void**>(g_environment + 0x1964) = g_interpolator;
    memset(g_creature, 0, sizeof(g_creature));
    g_creature[0x91] = dead ? 0x01 : 0xFE;          // only bit 0 is Dead
    g_hasCreature = hasCreature;
    WorldPeek::SetFrame(1234);
    g_n = 0; g_log[0] = 0; g_labelCount = 0; g_thisCount = 0; g_initFlag = !flag; g_eventPayload = 0x55;
    reinterpret_cast<CWorld*>(g_world)->ActivateWorld(flag);
    if (strcmp(g_log, expectLog) != 0) { printf("  log %s want %s\n", g_log, expectLog); return 1; }
    if (g_labelCount != 3 || strcmp(g_labels[0], "Init Characters") != 0 || strcmp(g_labels[1], "Init GUI") != 0 || strcmp(g_labels[2], "Init Quests") != 0) return 2;
    if (g_questArray != g_world + 0xAC || g_questFlag1 != false || g_questFlag2 != true) return 3;
    if (g_eventSeen.Type != 9 || g_eventSeen.Player != 3 || g_eventPayload != 0 || g_eventSeen.Valid != true) return 4;
    if (*reinterpret_cast<long*>(g_world + 0x8C) != 1234) return 5;
    if (g_world[0x88] != 0xCD || g_world[0x90] != 0xCD) return 6;
    return 0;
}

int main()
{
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(reinterpret_cast<CPlayerGui*>(g_gui));
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Set(reinterpret_cast<CQuestManager*>(g_quests));
    ComponentPeek::Set(reinterpret_cast<CMainGameComponent*>(g_component));
    int r;

    // Hero alive: no re-init.
    if ((r = Run(true, false, true, "PMCFPGPQBpDNE")) != 0) { printf("FAIL alive %d\n", r); return 1; }
    if (g_this[0] != g_playerManager || g_this[1] != g_player || g_this[2] != g_interpolator || g_this[3] != g_gui ||
        g_this[4] != g_quests || g_this[5] != g_quests || g_this[6] != g_playerManager || g_this[7] != g_component)
    { printf("FAIL receivers\n"); return 2; }

    // Hero dead: re-init with the caller's flag.
    if ((r = Run(true, true, true, "PMCIFPGPQBpDNE")) != 0 || g_initFlag != true) { printf("FAIL dead %d\n", r); return 3; }
    // No controlled creature: re-init (flag false forwarded).
    if ((r = Run(false, false, false, "PMCIFPGPQBpDNE")) != 0 || g_initFlag != false) { printf("FAIL missing %d\n", r); return 4; }

    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(0);
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Set(0);
    printf("FABLETLC_WORLD_ACTIVATEWORLD PASS\n");
    return 0;
}