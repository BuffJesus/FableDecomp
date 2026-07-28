#include <cstddef>
#include <cstdint>
#include <cstdlib>

class CDisplayEngine;
class CGameDefinitionManager;
class CMainGameComponent;
class CPlayerManager;
class CThingManager;
class CThingSearchTools;
class CWorld;

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
};
}

class CThing
{
public:
    static void SetCurrentThingManager(CThingManager* thingManager);
};

extern void* PTR__vector_deleting_destructor__01260f0c;
extern void* PTR__scalar_deleting_destructor__01238c8c;
extern class CGameScriptInterface* DAT_0143e8f0;

namespace
{
struct CScriptThingCtorOverlay
{
    void* vftable;         // 0x00
    std::int32_t field_04; // 0x04
    void* sharedState;     // 0x08
};

static_assert(offsetof(CScriptThingCtorOverlay, vftable) == 0x00);
static_assert(offsetof(CScriptThingCtorOverlay, field_04) == 0x04);
static_assert(offsetof(CScriptThingCtorOverlay, sharedState) == 0x08);
static_assert(sizeof(CScriptThingCtorOverlay) == 0x0C);

struct ScriptTimerNodeOverlay
{
    std::uint8_t field_00;            // 0x00
    std::byte pad_01[0x04 - 0x01];
    std::int32_t field_04;            // 0x04
    ScriptTimerNodeOverlay* field_08; // 0x08
    ScriptTimerNodeOverlay* field_0C; // 0x0C
    std::byte pad_10[0x18 - 0x10];
};

static_assert(offsetof(ScriptTimerNodeOverlay, field_00) == 0x00);
static_assert(offsetof(ScriptTimerNodeOverlay, field_04) == 0x04);
static_assert(offsetof(ScriptTimerNodeOverlay, field_08) == 0x08);
static_assert(offsetof(ScriptTimerNodeOverlay, field_0C) == 0x0C);
static_assert(sizeof(ScriptTimerNodeOverlay) == 0x18);

struct ScriptTimersOverlay
{
    ScriptTimerNodeOverlay* field_00; // 0x00
    std::int32_t field_04;            // 0x04
    std::int32_t field_08;            // 0x08
    std::int32_t field_0C;            // 0x0C
};

static_assert(offsetof(ScriptTimersOverlay, field_00) == 0x00);
static_assert(offsetof(ScriptTimersOverlay, field_04) == 0x04);
static_assert(offsetof(ScriptTimersOverlay, field_08) == 0x08);
static_assert(offsetof(ScriptTimersOverlay, field_0C) == 0x0C);
static_assert(sizeof(ScriptTimersOverlay) == 0x10);

struct CWorldCtorOverlay
{
    std::byte pad_00[0x20];
    CThingSearchTools* ThingSearchTools; // 0x20
};

static_assert(offsetof(CWorldCtorOverlay, ThingSearchTools) == 0x20);

struct CGameScriptInterfaceCtorOverlay
{
    void* vftable;                             // 0x00
    CWorld* World;                             // 0x04
    CMainGameComponent* Component;             // 0x08
    CDisplayEngine* DisplayEngine;             // 0x0C
    CGameDefinitionManager* DefinitionManager; // 0x10
    CPlayerManager* PlayerManager;             // 0x14
    CThingSearchTools* ThingSearchTools;       // 0x18
    long CurrentPlayer;                        // 0x1C
    long CurrentLevelID;                       // 0x20
    long CurrentScriptLevelID;                 // 0x24
    long CurrentScriptID;                      // 0x28
    bool InMovieSequence;                      // 0x2C
    bool AllowScreenFadingIfAlreadyFaded;      // 0x2D
    std::byte pad_2E[0x30 - 0x2E];
    CScriptThingCtorOverlay HeroSThing;        // 0x30
    ScriptTimersOverlay ScriptTimers;          // 0x3C
    long CameraResetToViewBehindHeroCount;     // 0x4C
    long CreateCreatureDelayFrames;            // 0x50
};

static_assert(offsetof(CGameScriptInterfaceCtorOverlay, World) == 0x04);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, Component) == 0x08);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, DisplayEngine) == 0x0C);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, DefinitionManager) == 0x10);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, PlayerManager) == 0x14);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, ThingSearchTools) == 0x18);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, CurrentPlayer) == 0x1C);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, CurrentLevelID) == 0x20);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, CurrentScriptLevelID) == 0x24);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, CurrentScriptID) == 0x28);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, InMovieSequence) == 0x2C);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, AllowScreenFadingIfAlreadyFaded) == 0x2D);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, HeroSThing) == 0x30);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, ScriptTimers) == 0x3C);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, CameraResetToViewBehindHeroCount) == 0x4C);
static_assert(offsetof(CGameScriptInterfaceCtorOverlay, CreateCreatureDelayFrames) == 0x50);
static_assert(sizeof(CGameScriptInterfaceCtorOverlay) == 0x54);
} // namespace

CGameScriptInterface::CGameScriptInterface(
    CMainGameComponent& component,
    const CGameDefinitionManager& definition_manager,
    CWorld& world,
    CPlayerManager& player_manager,
    CDisplayEngine& display_engine)
    : NHeroInformationScreens::CBase()
{
    auto* const self = reinterpret_cast<CGameScriptInterfaceCtorOverlay*>(this);
    const auto* const worldOverlay = reinterpret_cast<const CWorldCtorOverlay*>(&world);

    self->Component = &component;
    self->DisplayEngine = &display_engine;
    self->DefinitionManager = const_cast<CGameDefinitionManager*>(&definition_manager);
    self->vftable = &PTR__vector_deleting_destructor__01260f0c;
    self->World = &world;
    self->PlayerManager = &player_manager;
    self->ThingSearchTools = worldOverlay->ThingSearchTools;
    self->CurrentPlayer = -1;
    self->CurrentLevelID = 0;
    self->CurrentScriptLevelID = 0;
    self->CurrentScriptID = 0;
    self->InMovieSequence = false;
    self->AllowScreenFadingIfAlreadyFaded = false;

    NHeroInformationScreens::CBase::CBase(
        reinterpret_cast<NHeroInformationScreens::CBase*>(&self->HeroSThing));
    self->HeroSThing.vftable = &PTR__scalar_deleting_destructor__01238c8c;
    self->HeroSThing.field_04 = 0;
    self->HeroSThing.sharedState = nullptr;

    self->ScriptTimers.field_00 = nullptr;
    auto* const scriptTimerHead = static_cast<ScriptTimerNodeOverlay*>(std::malloc(0x18));
    self->ScriptTimers.field_00 = scriptTimerHead;
    self->ScriptTimers.field_04 = 0;
    scriptTimerHead->field_00 = 0;
    scriptTimerHead->field_04 = 0;
    scriptTimerHead->field_08 = scriptTimerHead;
    scriptTimerHead->field_0C = scriptTimerHead;
    self->ScriptTimers.field_0C = -1;
    self->CameraResetToViewBehindHeroCount = 0;

    DAT_0143e8f0 = this;
    CThing::SetCurrentThingManager(reinterpret_cast<CThingManager*>(this));
}