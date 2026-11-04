// CMainGameComponent::Shutdown @ 0x004175E5 (354 bytes).
// FableWin ?Shutdown@CMainGameComponent@@IAEXXZ. Tears the game component
// down: quiets the sound manager around the world's Uninitialise, drops the
// player-GUI singleton, unpublishes the component and its device-reset
// callback, uninitialises the frame smoother, network client, event-package
// files and particle engine, re-arms the console with the debug font, then
// releases every owned subsystem and finally the definition manager.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_console.h"
#include "fable_definition_manager.h"
#include "fable_display_engine.h"
#include "fable_particle_engine.h"
#include "fable_player_gui.h"
#include "fable_player_manager.h"
#include "fable_system_manager.h"
#include "fable_world.h"

class CLUA;

// CCountedPointer<T>::Reset is inline in the retail header (the out-of-line
// copies, e.g. 0x00403E40, are ReleaseData inlined + the new-info branch):
// Set(0) below reaches retail as `PClass.ReleaseData(); PClass.Data = 0`.
template<>
inline void CCountedPointer<CPlayerGui>::Reset(CPlayerGui* data)
{
    ReleaseData();
    Data = data;
    if (data)
        Info = new CCPPointerInfo(&DeleteData, data);
}

void CMainGameComponent::Shutdown()
{
    Initialised = false;

    if (g_FableSoundManager_013B8394)
    {
        g_FableSoundManager_013B8394->UnknownSlot16();
        g_FableSoundManager_013B8394->UnknownSlot18(0);
        g_FableSoundManager_013B8394->UnknownUpdateEntries_00991750();
    }

    if (PWorld)
        PWorld->Uninitialise();

    if (g_FableSoundManager_013B8394)
        g_FableSoundManager_013B8394->UnknownClearEntries_009918F0();

    PSampleBank = 0;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(0);
    PMainGameComponent = 0;
    g_FableDisplayManager_013B8390->RemoveDeviceResetCallback(this);

    FrameRateSmoother.Uninitialise();
    if (Client.IsValid())
        Client.Uninitialise();

    UninitEventPackageLoading();
    UninitEventPackageSaving();
    NParticleEngine::Uninitialise();

    CTBaseSingleton<CConsole>::Get().Initialise('`', KB_HASH, &PeekDebugFont());

    PTextBank.ReleaseData();
    PTextBank.Data = 0;
    PPlayerManager.reset(0);
    PPlayerInterface.reset(0);
    PWorld.reset(0);
    PDisplayEngine.reset(0);
    PLUA.ReleaseData();
    PLUA.Data = 0;
    PDebugFont.ReleaseData();
    PDebugFont.Data = 0;
    PCutSceneMainFont.ReleaseData();
    PCutSceneMainFont.Data = 0;
    PMainGraphicBank.ReleaseData();
    PMainGraphicBank.Data = 0;

    CGameDefinitionManager::Set(0);
}