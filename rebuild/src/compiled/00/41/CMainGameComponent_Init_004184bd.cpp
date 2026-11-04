// CMainGameComponent::Init @ 0x004184BD
// The main game's bring-up: text, console, definitions, graphics and fonts,
// the player manager and interface, the world, display engine, players, sound
// and particles, then PostInit, the network client's first update, the user
// config scripts and the system manager's reinit callback.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_console.h"
#include "fable_definition_manager.h"
#include "fable_filesystem.h"
#include "fable_game_init_statics.h"
#include "fable_gfuninitialise.h"
#include "fable_player_manager.h"
#include "fable_profile.h"
#include "fable_progress_display.h"
#include "fable_world.h"

// Each progress message and profile marker builds its own named CCharString
// (retail constructs it before pushing the other arguments and destroys it
// straight after the call).
#define MAIN_INIT_PROGRESS(text)                                              \
    do {                                                                      \
        CCharString progressText(text);                                       \
        NProgressDisplay::DisplayProgress(progressText, -1.0f, false, false); \
    } while (0)

#define MAIN_INIT_PROFILE(text)                                               \
    do {                                                                      \
        CCharString profileName(text);                                        \
        NProfileTimer::StartProfile(profileName, 0);                          \
    } while (0)

void CMainGameComponent::Init()
{
    PMainGameComponent = this;

    NProgressDisplay::StartProgress(true, false, false, false, false, false);
    NProgressDisplay::SetToDisplayText(false);
    InitialiseText();
    NProgressDisplay::SetToDisplayText(true);

    CTBaseSingleton<CConsole>::Get().Initialise('`', EInputKey(0x29), PDebugFont.Data);

    g_WorldFrame_013B7D70 = 0;
    g_RenderFrame_013B7D6C = 0;
    LastFrameRenderDuration = 1.0f / g_FableDisplayManager_013B8390->RefreshRate + 0.0001f;

    if (!CGameDefinitionManager::IsAllocated())
        CGameDefinitionManager::Set(new CGameDefinitionManager);

    MAIN_INIT_PROGRESS("Init Thing Components");
    CTCBase::RegisterTCs();

    MAIN_INIT_PROFILE("Init Definition Manager");
    MAIN_INIT_PROGRESS("Init Definition Manager");
    InitDefinitions(true);

    MAIN_INIT_PROFILE("Init Graphics");
    InitGraphics();
    InitialiseFonts();

    MAIN_INIT_PROGRESS("Init Subtitled Message");
    CSubtitledMessage::Initialise();

    MAIN_INIT_PROGRESS("Adding Console Variables");
    MAIN_INIT_PROGRESS("Init Conversation Attitude");
    CConversationAttitude::Init();

    MAIN_INIT_PROFILE("Init Player Manager");
    MAIN_INIT_PROGRESS("Init Player Manager");
    InitPlayerManager();

    MAIN_INIT_PROGRESS("Init Player Interface");
    PPlayerInterface.reset(new CGamePlayerInterface(*this, *PPlayerManager));

    bool checkPointSaveExists = false;
    if (g_DeleteCheckPointSaveOnInitWorld_013B7C90)
    {
        CWorld::DeleteCheckPointSave();
        g_DeleteCheckPointSaveOnInitWorld_013B7C90 = false;
    }
    else
    {
        checkPointSaveExists = true;
    }

    MAIN_INIT_PROGRESS("Init World");
    InitWorld();
    PWorld->SetAutoSaveCheckPointExists(checkPointSaveExists);

    MAIN_INIT_PROFILE("Init Display Engine");
    MAIN_INIT_PROGRESS("Init Display Engine");
    InitDisplayEngine();

    MAIN_INIT_PROFILE("Create Players");
    MAIN_INIT_PROGRESS("Create Players");
    CreatePlayers();

    MAIN_INIT_PROFILE("Init Sound");
    MAIN_INIT_PROGRESS("Init Sound");
    InitSound();

    if (!g_FableCompileFrontendDefinitions_013B8648)
    {
        MAIN_INIT_PROFILE("Load Particles");
        MAIN_INIT_PROGRESS("Loading particles");
        InitParticleEngine();
    }

    PostInit();

    if (!g_FableCompileFrontendDefinitions_013B8648)
    {
        FrameRateSmoother.Init(60, 0);
        Client.Update(CurrentServerFrame, PeekWorldChecksum(), WorldSeed);

        bool defaultConfigExists;
        {
            CWideString filename(L"default_user.ini");
            defaultConfigExists = CAFile::PathExists(filename);
        }
        if (defaultConfigExists)
        {
            CWideString filename(L"default_user.ini");
            CTBaseSingleton<CConsole>::Get().RunScript(filename);
        }
        {
            CWideString filename(L"user.ini");
            CTBaseSingleton<CConsole>::Get().RunScript(filename);
        }
    }

    GFGetSystemManager()->SetAppReinitFunc(AppReinitFunc, this);
    TimePassedSinceLastUpdate = 0.0f;
    LastUpdateTime = (float)GFGetTime();
    Initialised = true;
}