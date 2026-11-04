// CMainGameComponent::GetInputs @ 0x00416E78 (393 bytes).
// FableWin ?GetInputs@CMainGameComponent@@MAEXXZ (protected virtual, slot 6).
// Folds the game camera's render position (at interpolation 0 and 1) into a
// checksum the input manager stamps on this server frame, updates the input
// manager, then (while the application is active and the world is past its
// first frame) pumps the player interface's game events: queued normally, or
// only system events processed while event packages are being played back.
// MainGameComponent.cpp is size-optimised in retail; the CGameEvent/CCamera
// locals make this body carry a /GS cookie.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_camera.h"
#include "fable_checksum.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include "fable_system_manager.h"
#include "fable_world.h"

void CMainGameComponent::GetInputs()
{
    CGameEvent event;

    long positionSum = 0;
    positionSum += PWorld->PeekGameCamera()->GetRenderCamera(0.0f).PeekWorldPos().X;
    positionSum += PWorld->PeekGameCamera()->GetRenderCamera(0.0f).PeekWorldPos().Y;
    positionSum += PWorld->PeekGameCamera()->GetRenderCamera(0.0f).PeekWorldPos().Z;
    positionSum += PWorld->PeekGameCamera()->GetRenderCamera(1.0f).PeekWorldPos().X;
    positionSum += PWorld->PeekGameCamera()->GetRenderCamera(1.0f).PeekWorldPos().Y;
    positionSum += PWorld->PeekGameCamera()->GetRenderCamera(1.0f).PeekWorldPos().Z;

    g_FableInputManager_013B8388->UpdateChecksum(
        GetCurrentServerFrame(),
        CChecksum(PeekWorldChecksum(), PeekWorldSeed(), positionSum));
    g_FableInputManager_013B8388->Update();

    if (GFGetSystemManager()->IsApplicationActive() && CWorld::GetFrame() > 1)
    {
        PPlayerInterface->Update();
        while (PPlayerInterface->GetGameEvent(event))
        {
            if (!LoadingEventPackages)
                AddGameEvent(event);
            else if (EventIsSystemEvent(&event))
                ProcessEvent(&event);
        }
    }
}