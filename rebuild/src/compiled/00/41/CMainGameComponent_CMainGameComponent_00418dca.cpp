// CMainGameComponent::CMainGameComponent @ 0x00418DCA
// MainGameComponent.cpp is size-optimised in retail (see GenerateMetFilesFromLugFiles).
#pragma optimize("s", on)

#include "fable_game.h"

// The scoped_ptr members need their pointees complete.
#include "engine/CDisplayEngine.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"

CMainGameComponent::CMainGameComponent(
    CGame& game,
    const CMainGameComponentInit& init)
    : CGameComponent(game),
      ForceUpdateTick(false),
      ForceUpdateTickSpeedMultipler(3.0f),
      ForceUpdateTickSpeedDesiredFramerate(30.0f),
      ForceUpdateNoFailedUpdates(0),
      FirstWorldFrameUpdate(true),
      CurrentServerFrame(0),
      InputServerFrame(0),
      NoRenderFramesSinceLastGameUpdate(0),
      WorldSeed(0),
      LocalSeed(0),
      LoadingEventPackages(false),
      SavingEventPackages(false),
      LastRenderFrameStartTime(0.0),
      WorldUpdateTurn(false),
      NextComponentToRun(0),
      InitStructure(init),
      Initialised(false),
      AllowRender(true),
      Rendered(false)
{
    LastGameTurnForceRendered = 0;
    g_FableDisplayManager_013B8390->AddDeviceResetCallback(this);
    DebugNoFramesUnableToRender = 0;
}