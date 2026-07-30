#include <new>
#include <stdio.h>
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
    CGameComponent* g_nextComponent;

    class MockGameComponent : public CGameComponent
    {
    public:
        explicit MockGameComponent(ComponentKind kind)
            : kind_(kind)
        {
        }

        virtual ~MockGameComponent()
        {
            ++g_destroyCalls;
        }

        virtual void Init()
        {
            ++g_initCalls;
        }

        virtual bool Run(CGameComponent** nextComponent)
        {
            ++g_runCalls;
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
        g_nextComponent = 0;
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
            g_destroyCalls == 1 &&
            g_FableRetiredGameComponent_013B7D58 == 0 &&
            current == g_nextComponent &&
            Quit(game);

        delete current;
        return passed;
    }
}

CWideString::CWideString()
    : storage_(0)
{
}

CWideString::~CWideString()
{
}

CGameComponent::~CGameComponent()
{
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
    *reinterpret_cast<void**>(destination) = 0;
}

extern "C" void __fastcall
FableGameCharStringCtor_00412f90(void* destination, void*)
{
    ++g_charConstructCalls;
    *reinterpret_cast<void**>(destination) = 0;
}

extern "C" void __fastcall
FableGameWideStringDtor_00412f90(void*, void*)
{
    ++g_wideDestroyCalls;
}

extern "C" void __fastcall
FableGameCharStringDtor_00412f90(void*, void*)
{
    ++g_charDestroyCalls;
}

extern "C" void __fastcall
FableGameWideStringAssign_00412f90(
    void* destination,
    void*,
    const CWideString*)
{
    ++g_wideAssignCalls;
    *reinterpret_cast<void**>(destination) = 0;
}

extern "C" void* __fastcall
FableGameMainComponentCtor_00412f90(
    void* destination,
    void*,
    CGame*,
    const CMainGameComponentInit*)
{
    g_constructedKind = COMPONENT_MAIN;
    return new (destination) MockGameComponent(COMPONENT_MAIN);
}

extern "C" void* __fastcall
FableGameLegacyFrontendCtor_00412f90(
    void* destination,
    void*,
    CGame*,
    const CFrontendGameComponentInit*)
{
    g_constructedKind = COMPONENT_LEGACY_FRONTEND;
    return new (destination) MockGameComponent(COMPONENT_LEGACY_FRONTEND);
}

extern "C" void* __fastcall
FableGameNewFrontendCtor_00412f90(
    void* destination,
    void*,
    CGame*,
    const CNewFrontendGameComponentInit*)
{
    g_constructedKind = COMPONENT_NEW_FRONTEND;
    return new (destination) MockGameComponent(COMPONENT_NEW_FRONTEND);
}

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
