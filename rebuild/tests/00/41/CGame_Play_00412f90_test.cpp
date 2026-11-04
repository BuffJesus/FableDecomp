#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"
// The component destructors destroy scoped_ptr members of these.
#include "engine/CDisplayEngine.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"

class CFileInformation
{
};

fable_u8 g_FableCompileFrontendDefinitions_013B8648 = 0;
fable_u8 g_FableStartMainGame_013B8605 = 0;
fable_u8 g_FableUseLegacyFrontend_013B8642 = 0;
CGameComponent* g_FableRetiredGameComponent_013B7D58 = 0;
CWideString g_FableMainGameStartupPath_013B7D5C;

namespace
{
    enum ComponentKind
    {
        COMPONENT_MAIN,
        COMPONENT_LEGACY_FRONTEND,
        COMPONENT_NEW_FRONTEND,
        COMPONENT_NEXT,
        COMPONENT_RETIRED
    };

    fable_u32 g_compileDefsCalls;
    fable_u32 g_allocationCalls;
    fable_u32 g_lastAllocationSize;
    fable_u32 g_wideConstructCalls;
    fable_u32 g_charConstructCalls;
    fable_u32 g_wideAssignCalls;
    fable_u32 g_wideDestroyCalls;
    fable_u32 g_charDestroyCalls;
    fable_u32 g_initCalls;
    fable_u32 g_runCalls;
    fable_u32 g_destroyCalls;
    ComponentKind g_constructedKind;
    bool g_continueFirstRun;
    CGameComponent* g_nextComponent;

    // String constructions are counted only outside component
    // constructors: the components' own string members are not Play's.
    bool g_countStrings = true;

    CGame& MockGame()
    {
        static unsigned char storage[sizeof(CGame)];
        return *reinterpret_cast<CGame*>(storage);
    }

    // Stand-in for components the fixture creates itself (the next
    // component and a retired one); the counted behaviour lives on the
    // CGameComponent virtuals defined below.
    class MockGameComponent : public CGameComponent
    {
    public:
        virtual bool Run(CGameComponent** next) { return CGameComponent::Run(next); }
        virtual void ChangeTextureColourDepth(long) {}
        virtual void OnPreDeviceReset() {}
        virtual bool OnPostDeviceReset() { return true; }
        MockGameComponent()
            : CGameComponent(MockGame())
        {
            g_countStrings = true;
        }
    };

    bool g_trackAllocations;
    CGame* g_constructedGame;

    CGameComponent*& CurrentComponent(CGame& game)
    {
        return *reinterpret_cast<CGameComponent**>(
            reinterpret_cast<fable_u8*>(&game) + 8);
    }

    bool Quit(const CGame& game)
    {
        return *(reinterpret_cast<const fable_u8*>(&game) + 0x20C) != 0;
    }

    void ResetCounters()
    {
        g_compileDefsCalls = 0;
        g_allocationCalls = 0;
        g_lastAllocationSize = 0;
        g_wideConstructCalls = 0;
        g_charConstructCalls = 0;
        g_wideAssignCalls = 0;
        g_wideDestroyCalls = 0;
        g_charDestroyCalls = 0;
        g_initCalls = 0;
        g_runCalls = 0;
        g_destroyCalls = 0;
        g_constructedKind = COMPONENT_RETIRED;
        g_continueFirstRun = false;
        g_nextComponent = 0;
        g_trackAllocations = false;
        g_constructedGame = 0;
        g_FableRetiredGameComponent_013B7D58 = 0;
    }

    CGame& FreshGame(unsigned char (&storage)[sizeof(CGame)])
    {
        memset(storage, 0, sizeof(storage));
        return *reinterpret_cast<CGame*>(storage);
    }

    bool CheckCompileDefinitionsPath()
    {
        ResetCounters();
        g_FableCompileFrontendDefinitions_013B8648 = 1;
        g_FableStartMainGame_013B8605 = 0;
        g_FableUseLegacyFrontend_013B8642 = 0;

        unsigned char storage[sizeof(CGame)];
        CGame& game = FreshGame(storage);
        g_trackAllocations = true;
        game.Play();
        g_trackAllocations = false;

        return
            g_compileDefsCalls == 1 &&
            g_allocationCalls == 1 &&
            g_lastAllocationSize == 0x148 &&
            g_constructedKind == COMPONENT_NEW_FRONTEND &&
            g_initCalls == 1 &&
            g_runCalls == 0 &&
            g_destroyCalls == 1 &&
            CurrentComponent(game) == 0 &&
            !Quit(game);
    }

    bool CheckMainGamePath()
    {
        ResetCounters();
        g_FableCompileFrontendDefinitions_013B8648 = 0;
        g_FableStartMainGame_013B8605 = 1;
        g_FableUseLegacyFrontend_013B8642 = 0;

        unsigned char storage[sizeof(CGame)];
        CGame& game = FreshGame(storage);
        g_trackAllocations = true;
        game.Play();
        g_trackAllocations = false;

        CGameComponent* current = CurrentComponent(game);
        const bool passed =
            g_allocationCalls == 1 &&
            g_lastAllocationSize == 0x161E8 &&
            g_constructedKind == COMPONENT_MAIN &&
            g_constructedGame == &game &&
            g_wideConstructCalls == 3 &&
            g_charConstructCalls == 1 &&
            g_wideAssignCalls == 1 &&
            g_wideDestroyCalls == 3 &&
            g_charDestroyCalls == 1 &&
            g_initCalls == 1 &&
            g_runCalls == 1 &&
            g_destroyCalls == 0 &&
            current != 0 &&
            Quit(game);
        delete current;
        return passed;
    }

    bool CheckLegacyAndTransitionPath()
    {
        ResetCounters();
        g_FableCompileFrontendDefinitions_013B8648 = 0;
        g_FableStartMainGame_013B8605 = 0;
        g_FableUseLegacyFrontend_013B8642 = 1;
        g_continueFirstRun = true;
        g_nextComponent = new MockGameComponent;
        g_FableRetiredGameComponent_013B7D58 =
            new MockGameComponent;

        unsigned char storage[sizeof(CGame)];
        CGame& game = FreshGame(storage);
        g_trackAllocations = true;
        game.Play();
        g_trackAllocations = false;

        CGameComponent* current = CurrentComponent(game);
        const bool passed =
            g_allocationCalls == 1 &&
            g_lastAllocationSize == 0x1E60 &&
            g_constructedKind == COMPONENT_LEGACY_FRONTEND &&
            g_constructedGame == &game &&
            g_initCalls == 1 &&
            g_runCalls == 2 &&
            g_destroyCalls == 1 &&
            g_FableRetiredGameComponent_013B7D58 == 0 &&
            current == g_nextComponent &&
            Quit(game);

        delete current;
        return passed;
    }
}

void* operator new(size_t size)
{
    if (g_trackAllocations)
    {
        ++g_allocationCalls;
        g_lastAllocationSize = static_cast<fable_u32>(size);
    }
    return malloc(size ? size : 1);
}

void operator delete(void* block)
{
    free(block);
}

CWideString::CWideString()
    : storage_(0)
{
    if (g_countStrings)
    {
        ++g_wideConstructCalls;
    }
}

CWideString::CWideString(const CWideString& other)
    : storage_(other.storage_)
{
}

CWideString::~CWideString()
{
    ++g_wideDestroyCalls;
}

CWideString& CWideString::operator=(const CWideString& other)
{
    ++g_wideAssignCalls;
    storage_ = other.storage_;
    return *this;
}

CCharString::CCharString()
    : storage_(0)
{
    if (g_countStrings)
    {
        ++g_charConstructCalls;
    }
}

CCharString::~CCharString()
{
    ++g_charDestroyCalls;
}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
// ~CGameComponent is inline and empty (retail 0x00415940), so a component's
// destruction is observed at the out-of-line base destructor it chains to.
CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
    ++g_destroyCalls;
}

void CDeviceResetCallback::OnPreDeviceReset() {}
bool CDeviceResetCallback::OnPostDeviceReset() { return true; }
void CGameComponent::SetQuit() {}

#include "test_support/init_base_class_stubs.h"

// Link stubs for the members the component constructors build.
CInterpolationInfo::CInterpolationInfo() {}
CGameEventPackageSet::CGameEventPackageSet() {}
CNetworkClient::CNetworkClient() {}
CFrameRateSmoother::CFrameRateSmoother() {}
CRoughFrameCounter::CRoughFrameCounter() {}
CStopWatch::CStopWatch() {}
CAFile::CAFile() {}

CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() {}
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { return CFileInformation(); }

CGameComponent::CGameComponent(CGame& game)
{
    Game = &game;
    g_countStrings = false;
}


void CGameComponent::Init()
{
    ++g_initCalls;
}

bool CGameComponent::Run(CGameComponent** nextComponent)
{
    ++g_runCalls;
    if (g_continueFirstRun && g_runCalls == 1)
    {
        *nextComponent = g_nextComponent;
        return true;
    }
    return false;
}

void FABLE_FASTCALL CNewFrontendGameComponent::CompileDefs()
{
    ++g_compileDefsCalls;
}

CMainGameComponent::CMainGameComponent(
    CGame& game,
    const CMainGameComponentInit&)
    : CGameComponent(game)
{
    g_countStrings = true;
    g_constructedKind = COMPONENT_MAIN;
    g_constructedGame = &game;
}

CFrontendGameComponent::CFrontendGameComponent(
    CGame& game,
    const CFrontendGameComponentInit& init)
    : CGameComponent(game)
{
    g_countStrings = true;
    g_constructedKind = init.Nowt == 0
        ? COMPONENT_LEGACY_FRONTEND
        : COMPONENT_RETIRED;
    g_constructedGame = &game;
}

CNewFrontendGameComponent::CNewFrontendGameComponent(
    CGame& game,
    const CNewFrontendGameComponentInit& init)
    : CGameComponent(game)
{
    g_countStrings = true;
    g_constructedKind = init.Nothing == 0
        ? COMPONENT_NEW_FRONTEND
        : COMPONENT_RETIRED;
    g_constructedGame = &game;
}

CMainGameComponent::~CMainGameComponent() {}
CNewFrontendGameComponent::~CNewFrontendGameComponent() {}

int main()
{
    if (
        !CheckCompileDefinitionsPath() ||
        !CheckMainGamePath() ||
        !CheckLegacyAndTransitionPath())
    {
        puts("FABLETLC_CGAME_PLAY_BEHAVIOR FAIL");
        return 1;
    }

    puts("FABLETLC_CGAME_PLAY_BEHAVIOR PASS");
    return 0;
}

void CMainGameComponent::Init() { CGameComponent::Init(); }
bool CMainGameComponent::Run(CGameComponent** next) { return CGameComponent::Run(next); }
void CMainGameComponent::ChangeTextureColourDepth(long) {}
void CMainGameComponent::SetQuit() {}
void CMainGameComponent::OnPreDeviceReset() {}
bool CMainGameComponent::OnPostDeviceReset() { return true; }
void CMainGameComponent::Update() {}
void CMainGameComponent::GetInputs() {}
void CMainGameComponent::Render() {}
void CMainGameComponent::PostInit() {}
bool CMainGameComponent::IsEditorActive() const { return false; }

void CFrontendGameComponent::Init() { CGameComponent::Init(); }
bool CFrontendGameComponent::Run(CGameComponent** next) { return CGameComponent::Run(next); }
void CFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CFrontendGameComponent::SetQuit() {}
void CFrontendGameComponent::OnPreDeviceReset() {}
bool CFrontendGameComponent::OnPostDeviceReset() { return true; }

void CNewFrontendGameComponent::Init() { CGameComponent::Init(); }
bool CNewFrontendGameComponent::Run(CGameComponent** next) { return CGameComponent::Run(next); }
void CNewFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CNewFrontendGameComponent::SetQuit() {}
void CNewFrontendGameComponent::OnPreDeviceReset() {}
bool CNewFrontendGameComponent::OnPostDeviceReset() { return true; }
