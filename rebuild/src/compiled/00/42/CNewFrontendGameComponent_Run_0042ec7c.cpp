// CNewFrontendGameComponent::Run @ 0x0042EC7C (2191 bytes).
// The frontend's whole lifetime: play the intro logos, open the frontend
// banks, initialise, then run Input/Update/Draw each frame (with the attract
// movie after two idle minutes) until the system quits or a menu asks for a
// new component; finally hand back the dev frontend or the main game.
#pragma optimize("s", on)
#include <stdlib.h>
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"
#include "fable_rgb_colour.h"
#include "fable_profile.h"
#include "fable_nui_manager.h"
#include "fable_console.h"
#include "fable_file_installer.h"
#include "fable_resource_directories.h"

extern bool GUseRetailBanks;

// FableWin ?GetLanguage@NLocalisation@@YIKXZ (0x00415070): a Windows primary
// language id (LANG_GERMAN 7, LANG_SPANISH 10, LANG_FRENCH 12, LANG_ITALIAN 16;
// 4/17/18 are Chinese/Japanese/Korean, which fall back to English).
namespace NLocalisation
{
    unsigned long FABLE_FASTCALL GetLanguage();
}

extern "C" __declspec(dllimport) void FABLE_STDCALL Sleep(unsigned long milliseconds);

// Frontend globals (retail addresses; names not in the PDBs).
extern CNewFrontendGameComponent* g_FableFrontendGameComponent_013B871C;
extern bool g_FableFrontendFirstRun_01375448;
extern bool g_FablePlayIntroMovies_0137544A;
extern CRGBColour g_FableMovieBackgroundColour_013961E0;
extern bool g_FablePlayingFullScreenMovie_013CA7E9;
extern CASoundManager* g_FableSoundManager_013B8394;
extern CFontManager* g_FableFontManager_013B838C;
extern CInputManager* g_FableInputManager_013B8388;
extern CRenderManager2D* g_FableRenderManager2D_013B8384;

// FableWin ?InstallationComplete@@YI_NXZ (0x0042E3CE), ?GPlayMusic@@YIXM@Z
// (0x0042DED5) and ?GFPlayFullScreenXBoxMovie@@YI_N...@Z (0x006286F0).
bool FABLE_FASTCALL InstallationComplete();
void FABLE_FASTCALL GPlayMusic(float offset);
bool FABLE_FASTCALL GFPlayFullScreenXBoxMovie(
    const CCharString& filename,
    unsigned long flags,
    unsigned long width,
    unsigned long height,
    unsigned long bufferSize,
    const NGameText::CDataBank* textBank,
    CIEngine* engine,
    bool fullScreen,
    float timeOut,
    bool (FABLE_FASTCALL *continueFunc)(),
    CXMVPlayerBuffers* buffers,
    CWideString* errorString);

// The intro logo table Run builds on its stack (0x20 bytes per movie).
struct CFrontendMovieInfo
{
    const char* FileName;
    unsigned long Width;
    unsigned long Height;
    CRGBColour BackgroundColour;
    unsigned long PauseAfter;
    unsigned long Flags;
    float TimeOut;
    bool (FABLE_FASTCALL *ContinueFunc)();
};

bool CNewFrontendGameComponent::Run(CGameComponent** nextComponent)
{
    m_fLastUpdateTime = (float)GFGetTime();
    m_fLastRenderTime = (float)GFGetTime();

    CFrontendMovieInfo movies[3] =
    {
        { "Data\\Video\\lionhead_logo.xmv", 640, 400, CRGBColour(255, 255, 255, 255), 0, 416, 20.0f, InstallationComplete },
        { "Data\\Video\\Microsoft_Logo.xmv", 640, 480, CRGBColour(0, 0, 0, 255), 0, 416, 20.0f, InstallationComplete },
        { "Data\\Video\\intro_comp.xmv", 640, 360, CRGBColour(0, 0, 0, 0), 0, 416, 10.0f, InstallationComplete },
    };

    if (g_FableFrontendFirstRun_01375448 && g_FablePlayIntroMovies_0137544A)
    {
        for (unsigned long i = 0; i < 3; ++i)
        {
            // Retail copies through CRGBColour's copy constructor (R, G, B, A).
            g_FableMovieBackgroundColour_013961E0 = CRGBColour(movies[i].BackgroundColour);
            g_FablePlayingFullScreenMovie_013CA7E9 = true;
            CWideString errorString;
            const bool played = GFPlayFullScreenXBoxMovie(
                    movies[i].FileName,
                    movies[i].Flags,
                    movies[i].Width,
                    movies[i].Height,
                    0xA00000,
                    m_pTextBank.Get(),
                    m_pEngine.Get(),
                    true,
                    movies[i].TimeOut,
                    movies[i].ContinueFunc,
                    0,
                    &errorString);
            if (!played)
            {
                unknown144_ = false;
                NUISystem::CFrontEndManager* frontEndManager = NUISystem::CFrontEndManager::GetInstance();
                frontEndManager->MediaPlayerErrorString = errorString;
            }
            g_FablePlayingFullScreenMovie_013CA7E9 = false;
            g_FableMovieBackgroundColour_013961E0 = CRGBColour(0, 0, 0, 255);
            Sleep(movies[i].PauseAfter);
            if (GFGetSystemManager()->Update() == ESR_QUIT)
                return false;
        }
    }

    if (GUseRetailBanks)
    {
        CWideString platform(L"pc\\");
        g_FableBankFileManager_013CA79C.OpenRetailBank(NResourceDirectoryNames::GetGraphicBankDir() + platform + L"frontend.big", true);
        g_FableBankFileManager_013CA79C.OpenRetailBank(NResourceDirectoryNames::GetDVDDialogueDir() + L"text.big", false);
        g_FableBankFileManager_013CA79C.OpenRetailBank(NResourceDirectoryNames::GetShaderBankDir() + platform + L"shaders.big", false);
    }

    Running = true;
    Init2();
    NProfileTimer::StartProfile("Init Engine", 0);
    InitialiseEngine();
    NProfileTimer::EndProfile(0);
    NProfileTimer::StartProfile("Init frontend", 0);

    CXMVPlayerBuffers* buffers = new CXMVPlayerBuffers;
    g_FableFontManager_013B838C->SetLoadFontGlyphsAsynchronously(false);
    m_pMenuFont.Get()->Preload(L"A", true);

    CDisplayManager* displayManager = GFGetSystemManager()->PDisplayManager.Data;
    g_FableRenderManager2D_013B8384->ClearRenderTarget(CRGBColour(0, 0, 0, 255), 0);
    displayManager->SwapScreens();

    GPlayMusic(0.0f);
    m_pFrontEndManager->ChangeStateFirstTime();
    m_attractModeTimer.StartZero();
    m_attractModeTimer.Reset();
    NProfileTimer::EndProfile(0);
    NUISystem::CUIScreenUnitConverter::ResetDimensions();

    while (GFGetSystemManager()->Update() != ESR_QUIT && !Quit)
    {
        float musicOffset;
        if (GFGetSystemManager()->GetForceMusicPlay(musicOffset))
        {
            GFGetSystemManager()->ResetForceMusicPlay();
            GPlayMusic(musicOffset);
        }

        if (m_inputDelayTimer.m_bIsRunning && m_inputDelayTimer.GetElapsedSeconds() >= 0.2f)
            m_inputDelayTimer.Stop();

        Input();
        Update();

        CEngineInterpolationInfo interpolationInfo;
        UpdateInterpolation(interpolationInfo);
        Draw(interpolationInfo);

        if (m_attractModeTimer.GetElapsedSeconds() >= 120.0f && m_pFrontEndManager->CanStartAttractMode())
        {
            if (g_FableSoundManager_013B8394)
                g_FableSoundManager_013B8394->UnknownSlot18(0);

            const long movieIndex = rand() % (long)m_pFrontEndDef->vAttractModeMovie.size();
            CCharString movie(m_pFrontEndDef->vAttractModeMovie[movieIndex]);
            switch (NLocalisation::GetLanguage())
            {
            case 4:
                movie += "English";
                break;
            case 7:
                movie += "German";
                break;
            case 10:
                movie += "Spanish";
                break;
            case 12:
                movie += "French";
                break;
            case 16:
                movie += "Italian";
                break;
            case 17:
                movie += "English";
                break;
            case 18:
                movie += "English";
                break;
            default:
                movie += "English";
                break;
            }
            movie += ".wmv";

            g_FablePlayingFullScreenMovie_013CA7E9 = true;
            GFPlayFullScreenXBoxMovie(movie, 0x221, 0, 0, 0x100000, m_pTextBank.Get(), m_pEngine.Get(), true, -1.0f, 0, buffers, 0);
            g_FablePlayingFullScreenMovie_013CA7E9 = false;
            m_attractModeTimer.Reset();
            g_FableInputManager_013B8388->ClearEvents();
            m_inputDelayTimer.StartZero();
            GPlayMusic(0.0f);
        }
    }

    Quit = true;
    Running = false;
    delete buffers;
    ReleaseXMVCode();
    g_FableFrontendGameComponent_013B871C = 0;
    CleanUpInterfaces();

    if (CreateDevFrontEnd)
    {
        Shutdown();
        CGameComponent* component = new CFrontendGameComponent(*Game, CFrontendGameComponentInit());
        component->Init();
        *nextComponent = component;
        g_FableFrontendGameComponent_013B871C = 0;
        g_FableRetiredGameComponent_013B7D58 = this;
        return true;
    }

    if (m_bCreateGame)
    {
        g_FableFrontendFirstRun_01375448 = false;
        NProfileTimer::StartProfile("Leave frontend", 0);
        if (g_FableSoundManager_013B8394)
            g_FableSoundManager_013B8394->UnknownSlot18(500);
        if (GUseRetailBanks)
        {
            CWideString platform(L"pc\\");
            g_FableBankFileManager_013CA79C.CloseRetailBank(NResourceDirectoryNames::GetGraphicBankDir() + platform + L"frontend.big");
            g_FableBankFileManager_013CA79C.OpenRetailBank(NResourceDirectoryNames::GetGraphicBankDir() + L"graphics.big", true);
            g_FableBankFileManager_013CA79C.OpenRetailBank(NResourceDirectoryNames::GetGraphicBankDir() + platform + L"textures.big", true);
            g_FableBankFileManager_013CA79C.OpenRetailBank(NResourceDirectoryNames::GetDVDDialogueDir() + L"dialogue.big", false);
            g_FableBankFileManager_013CA79C.OpenRetailBank(NResourceDirectoryNames::GetHDMiscDir() + platform + L"effects.big", false);
        }
        CTBaseSingleton<CFileInstaller>::Release();

        CMainGameComponentInit init;
        init.SaveGameName = SaveGame;
        init.InitialWorldName = CWideString::CreateFromCharString("FinalAlbion.wld");
        NProfileTimer::EndProfile(0);

        CGame& game = *Game;
        Shutdown();
        NProfileTimer::StartProfile("Init Game", 1);
        CGameComponent* component = new CMainGameComponent(game, init);
        component->Init();
        *nextComponent = component;
        g_FableRetiredGameComponent_013B7D58 = this;
        return true;
    }

    g_FableFrontendGameComponent_013B871C = 0;
    Shutdown();
    g_FableRetiredGameComponent_013B7D58 = this;
    return false;
}
