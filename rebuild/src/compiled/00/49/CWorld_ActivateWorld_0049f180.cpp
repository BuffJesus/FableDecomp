// CWorld::ActivateWorld, retail 0x0049F180 (FableWin ?ActivateWorld@CWorld@@QAEX_N@Z):
// bring the loaded world live: make sure the hero exists, clear any screen
// filters, activate the GUI and the initial quests, then tell the game (through
// a type-9 game event) and mark the frame 3D rendering starts on.
// Built with /GS (retail cookie check). InitQuests is inlined here, as in retail.
#include "fable_world.h"
#include "fable_game.h"
#include "fable_player_manager.h"
#include "fable_environment.h"
#include "fable_player_gui.h"
#include "fable_quest_manager.h"
#include "fable_profile.h"

inline void CWorld::InitQuests()
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().ActivateMultipleQuests(InitialActiveQuests, false, true);
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PostBoastTransfer();
}

void CWorld::ActivateWorld(bool flag)
{
    NProfileTimer::StartProfile("Init Characters", 0);
    CThingPlayerCreature* creature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!IS_THING_ALIVE(creature))
        PlayerManager->InitPlayerCharacter(flag);
    GetEnvironment().GetSpellEffectInterpolator().ClearScreenFilters();

    NProfileTimer::StartProfile("Init GUI", 0);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().InitialActivate();

    NProfileTimer::StartProfile("Init Quests", 0);
    InitQuests();
    NProfileTimer::EndProfile(0);

    CGameEvent event(9);
    event.AddUBYTE(0);
    event.SetPlayer(PlayerManager->GetMainPlayerNumber());
    CMainGameComponent::Get()->AddGameEvent(event);

    SetAsStarting3DRendering();
}