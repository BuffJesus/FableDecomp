#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"

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
    bool g_enableCompileDuringInit;
    bool g_retirementOrderValid;
    CGameComponent* g_constructedComponent;
    CGameComponent* g_nextComponent;
    CGame* g_expectedGame;
    CGame g_mockGame;
    int g_startupPathSentinel;
    char g_stringEvents[32];
    unsigned g_stringEventCount;

    void StringEvent(char event)
    {
        if (g_stringEventCount + 1 >= sizeof(g_stringEvents))
            abort();
        g_stringEvents[g_stringEventCount++] = event;
        g_stringEvents[g_stringEventCount] = 0;
    }

    void RequireConstructorArguments(bool valid)
    {
        if (!valid)
        {
            puts("FABLETLC_CGAME_PLAY_BEHAVIOR FAIL constructor arguments");
            exit(2);
        }
    }

    class MockGameComponent : public CGameComponent
    {
    public:
        explicit MockGameComponent(ComponentKind kind)
            : CGameComponent(g_mockGame), kind_(kind)
        {
        }

        virtual ~MockGameComponent()
        {
            ++g_destroyCalls;
        }

        virtual void Init()
        {
            ++g_initCalls;
            if (g_enableCompileDuringInit)
                g_FableCompileFrontendDefinitions_013B8648 = 1;
        }

        virtual bool Run(CGameComponent** nextComponent)
        {
            ++g_runCalls;
            if (g_runCalls == 2 &&
                (g_destroyCalls != 1 || g_FableRetiredGameComponent_013B7D58))
                g_retirementOrderValid = false;
            if (g_continueFirstRun && g_runCalls == 1)
            {
                *nextComponent = g_nextComponent;
                return true;
            }
            return false;
        }

    private:
        ComponentKind kind_;
    };

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
        g_enableCompileDuringInit = false;
        g_retirementOrderValid = true;
        g_constructedComponent = 0;
        g_stringEventCount = 0;
        g_stringEvents[0] = 0;
        *reinterpret_cast<void**>(&g_FableMainGameStartupPath_013B7D5C) =
            &g_startupPathSentinel;
        g_nextComponent = 0;
        g_FableRetiredGameComponent_013B7D58 = 0;
    }

    CGame& FreshGame(unsigned char (&storage)[sizeof(CGame)])
    {
        memset(storage, 0, sizeof(storage));
        g_expectedGame = reinterpret_cast<CGame*>(storage);
        return *g_expectedGame;
    }

    bool CheckCompileDefinitionsPath()
    {
        ResetCounters();
        g_FableCompileFrontendDefinitions_013B8648 = 1;
        g_FableStartMainGame_013B8605 = 0;
        g_FableUseLegacyFrontend_013B8642 = 0;

        unsigned char storage[sizeof(CGame)];
        CGame& game = FreshGame(storage);
        game.Play();

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
        game.Play();

        CGameComponent* current = CurrentComponent(game);
        const bool passed =
            g_allocationCalls == 1 &&
            g_lastAllocationSize == 0x161E8 &&
            g_constructedKind == COMPONENT_MAIN &&
            g_wideConstructCalls == 3 &&
            g_charConstructCalls == 1 &&
            g_wideAssignCalls == 1 &&
            g_wideDestroyCalls == 3 &&
            g_charDestroyCalls == 1 &&
            strcmp(g_stringEvents, "WWCWAwcww") == 0 &&
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
        g_nextComponent = new MockGameComponent(COMPONENT_NEXT);
        g_FableRetiredGameComponent_013B7D58 =
            new MockGameComponent(COMPONENT_RETIRED);

        unsigned char storage[sizeof(CGame)];
        CGame& game = FreshGame(storage);
        game.Play();

        CGameComponent* current = CurrentComponent(game);
        const bool passed =
            g_allocationCalls == 1 &&
            g_lastAllocationSize == 0x1E60 &&
            g_constructedKind == COMPONENT_LEGACY_FRONTEND &&
            g_initCalls == 1 &&
            g_runCalls == 2 &&
            g_retirementOrderValid &&
            g_destroyCalls == 1 &&
            g_FableRetiredGameComponent_013B7D58 == 0 &&
            current == g_nextComponent &&
            Quit(game);

        delete current;
        delete g_constructedComponent;
        return passed;
    }

    bool CheckNewFrontendPath(bool alreadyQuit, bool enableCompileDuringInit)
    {
        ResetCounters();
        g_FableCompileFrontendDefinitions_013B8648 = 0;
        g_FableStartMainGame_013B8605 = 0;
        g_FableUseLegacyFrontend_013B8642 = 0;
        g_enableCompileDuringInit = enableCompileDuringInit;
        unsigned char storage[sizeof(CGame)];
        CGame& game = FreshGame(storage);
        storage[0x20C] = alreadyQuit ? 1 : 0;
        game.Play();
        CGameComponent* current = CurrentComponent(game);
        const bool passed =
            g_compileDefsCalls == 0 && g_allocationCalls == 1 &&
            g_lastAllocationSize == 0x148 &&
            g_constructedKind == COMPONENT_NEW_FRONTEND &&
            g_initCalls == 1 &&
            g_runCalls == ((!alreadyQuit && !enableCompileDuringInit) ? 1u : 0u) &&
            g_destroyCalls == (enableCompileDuringInit ? 1u : 0u) &&
            ((current != 0) == !enableCompileDuringInit) &&
            Quit(game) == (alreadyQuit || !enableCompileDuringInit);
        delete current;
        return passed;
    }

    bool CheckRetirementOnExit()
    {
        ResetCounters();
        g_FableCompileFrontendDefinitions_013B8648 = 0;
        g_FableStartMainGame_013B8605 = 0;
        g_FableUseLegacyFrontend_013B8642 = 0;
        g_FableRetiredGameComponent_013B7D58 =
            new MockGameComponent(COMPONENT_RETIRED);
        unsigned char storage[sizeof(CGame)];
        CGame& game = FreshGame(storage);
        game.Play();
        const bool passed = g_runCalls == 1 && g_destroyCalls == 1 &&
            g_FableRetiredGameComponent_013B7D58 == 0 && Quit(game);
        delete CurrentComponent(game);
        return passed;
    }
}

CWideString::CWideString()
    : storage_(0)
{
#if !defined(FABLETLC_CGAME_PLAY_ASM_BASELINE)
    ++g_wideConstructCalls;
    StringEvent('W');
#endif
}

CWideString::~CWideString()
{
#if !defined(FABLETLC_CGAME_PLAY_ASM_BASELINE)
    ++g_wideDestroyCalls;
    StringEvent('w');
#endif
}

CGameComponent::~CGameComponent()
{
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
void CDeviceResetCallback::OnPreDeviceReset() {}
bool CDeviceResetCallback::OnPostDeviceReset() { return true; }
CGameComponent::CGameComponent(CGame& game) : Quit(false), Running(false), Game(&game) {}
void CGameComponent::ChangeTextureColourDepth(long) {}
void CGameComponent::SetQuit() { Quit = true; }
CStopWatch::CStopWatch()
    : m_fTimerPeriod(0), m_nStartTick(0), m_nPrevElapsedTicks(0), m_bIsRunning(false) {}
CNewFrontendGameComponent::~CNewFrontendGameComponent() {}
void CNewFrontendGameComponent::Init() {}
bool CNewFrontendGameComponent::Run(CGameComponent**) { return false; }
void CNewFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CNewFrontendGameComponent::SetQuit() { Quit = true; }
void CNewFrontendGameComponent::OnPreDeviceReset() {}
bool CNewFrontendGameComponent::OnPostDeviceReset() { return true; }

CCharString::CCharString() : storage_(0)
{
#if !defined(FABLETLC_CGAME_PLAY_ASM_BASELINE)
    ++g_charConstructCalls;
    StringEvent('C');
#endif
}
CCharString::~CCharString()
{
#if !defined(FABLETLC_CGAME_PLAY_ASM_BASELINE)
    ++g_charDestroyCalls;
    StringEvent('c');
#endif
}

void CGameComponent::Init()
{
}

bool CGameComponent::Run(CGameComponent**)
{
    return false;
}

extern "C" void FableGameCompileFrontendDefinitions_00412f90()
{
    ++g_compileDefsCalls;
}

extern "C" void* __cdecl FableGameOperatorNew_00412f90(fable_u32 size)
{
    ++g_allocationCalls;
    g_lastAllocationSize = size;
    return ::operator new(size);
}

extern "C" void __fastcall
FableGameWideStringCtor_00412f90(void* destination, void*)
{
    ++g_wideConstructCalls;
    StringEvent('W');
    *reinterpret_cast<void**>(destination) = 0;
}

extern "C" void __fastcall
FableGameCharStringCtor_00412f90(void* destination, void*)
{
    ++g_charConstructCalls;
    StringEvent('C');
    *reinterpret_cast<void**>(destination) = 0;
}

extern "C" void __fastcall
FableGameWideStringDtor_00412f90(void*, void*)
{
    ++g_wideDestroyCalls;
    StringEvent('w');
}

extern "C" void __fastcall
FableGameCharStringDtor_00412f90(void*, void*)
{
    ++g_charDestroyCalls;
    StringEvent('c');
}

extern "C" void __fastcall
FableGameWideStringAssign_00412f90(
    void* destination,
    void*,
    const CWideString* source)
{
    ++g_wideAssignCalls;
    StringEvent('A');
    RequireConstructorArguments(source == &g_FableMainGameStartupPath_013B7D5C);
    *reinterpret_cast<void**>(destination) =
        *reinterpret_cast<void* const*>(source);
}

extern "C" void* __fastcall
FableGameMainComponentCtor_00412f90(
    void* destination,
    void*,
    CGame* game,
    const CMainGameComponentInit* init)
{
    RequireConstructorArguments(game == g_expectedGame && init &&
        *reinterpret_cast<void* const*>(&init->InitialWorldName) == 0 &&
        *reinterpret_cast<void* const*>(&init->InitialWorldHolySiteName) == 0 &&
        *reinterpret_cast<void* const*>(&init->InitialQuestName) == 0 &&
        *reinterpret_cast<void* const*>(&init->SaveGameName) == &g_startupPathSentinel);
    g_constructedKind = COMPONENT_MAIN;
    return g_constructedComponent = new (destination) MockGameComponent(COMPONENT_MAIN);
}

extern "C" void* __fastcall
FableGameLegacyFrontendCtor_00412f90(
    void* destination,
    void*,
    CGame* game,
    const CFrontendGameComponentInit* init)
{
    RequireConstructorArguments(game == g_expectedGame && init && init->value == 0);
    g_constructedKind = COMPONENT_LEGACY_FRONTEND;
    return g_constructedComponent = new (destination) MockGameComponent(COMPONENT_LEGACY_FRONTEND);
}

extern "C" void* __fastcall
FableGameNewFrontendCtor_00412f90(
    void* destination,
    void*,
    CGame* game,
    const CNewFrontendGameComponentInit* init)
{
    RequireConstructorArguments(game == g_expectedGame && init && init->value == 0);
    g_constructedKind = COMPONENT_NEW_FRONTEND;
    return g_constructedComponent = new (destination) MockGameComponent(COMPONENT_NEW_FRONTEND);
}

#if !defined(FABLETLC_CGAME_PLAY_ASM_BASELINE)
// Native C++ entry points for the same isolated doubles used by the asm
// fixture. No engine initialization, files, window creation, or game launch.
void* __cdecl operator new(size_t size)
{
    if (size == sizeof(CMainGameComponent) ||
        size == sizeof(CFrontendGameComponent) ||
        size == sizeof(CNewFrontendGameComponent))
    {
        ++g_allocationCalls;
        g_lastAllocationSize = size;
    }
    return malloc(size);
}

void __cdecl operator delete(void* memory) { free(memory); }

void CNewFrontendGameComponent::CompileDefs()
{
    FableGameCompileFrontendDefinitions_00412f90();
}

CWideString& CWideString::operator=(const CWideString& value)
{
    FableGameWideStringAssign_00412f90(this, 0, &value);
    return *this;
}

CMainGameComponent::CMainGameComponent(CGame& game, const CMainGameComponentInit& init)
    : CGameComponent(game)
{
    FableGameMainComponentCtor_00412f90(this, 0, &game, &init);
}
CFrontendGameComponent::CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit& init)
    : CGameComponent(game)
{
    FableGameLegacyFrontendCtor_00412f90(this, 0, &game, &init);
}
CNewFrontendGameComponent::CNewFrontendGameComponent(CGame& game, const CNewFrontendGameComponentInit& init)
    : CGameComponent(game)
{
    FableGameNewFrontendCtor_00412f90(this, 0, &game, &init);
}
#endif

int main()
{
    if (
        !CheckCompileDefinitionsPath() ||
        !CheckMainGamePath() ||
        !CheckLegacyAndTransitionPath() ||
        !CheckNewFrontendPath(false, false) ||
        !CheckNewFrontendPath(true, false) ||
        !CheckNewFrontendPath(false, true) ||
        !CheckRetirementOnExit())
    {
        puts("FABLETLC_CGAME_PLAY_BEHAVIOR FAIL");
        return 1;
    }

    puts("FABLETLC_CGAME_PLAY_BEHAVIOR PASS");
    return 0;
}
