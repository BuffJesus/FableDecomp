#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_frontend_startup.h"
#include "fable_performance_counter.h"

CNewFrontendGameComponent* g_FableNewFrontend_013B871C = 0;
fable_u8 g_FableCompileFrontendDefinitions_013B8648 = 0;
fable_u8 g_FableStartMainGame_013B8605 = 0;
fable_u8 g_FableUseLegacyFrontend_013B8642 = 0;
CGameComponent* g_FableRetiredGameComponent_013B7D58 = 0;
CWideString g_FableMainGameStartupPath_013B7D5C;

namespace
{
    char events[64];
    unsigned eventCount;
    unsigned timeCalls;
    unsigned stopwatchCalls;
    CNewFrontendGameComponent* expectedFrontend;
    const CFontBank* expectedFont;
    CConsole console;
    int fontToken;

    void Require(bool value, const char* label)
    {
        if (!value)
        {
            printf("FRONTEND_STARTUP FAIL %s\n", label);
            exit(1);
        }
    }
    void Event(char value)
    {
        Require(eventCount + 1 < sizeof(events), "event capacity");
        events[eventCount++] = value;
        events[eventCount] = 0;
    }
    bool Poisoned(const void* memory, size_t size)
    {
        const unsigned char* p = static_cast<const unsigned char*>(memory);
        for (size_t i = 0; i != size; ++i)
            if (p[i] != 0xA5) return false;
        return true;
    }
    void ResetEvents() { eventCount = 0; events[0] = 0; }
}

// Only these dependency doubles replace outside behavior. The base constructor,
// frontend/stopwatch constructors and Init are real reconstructed translation units.
CBaseClass::CBaseClass() { Event('B'); }
CBaseClass::~CBaseClass() {}
void CDeviceResetCallback::OnPreDeviceReset() {}
bool CDeviceResetCallback::OnPostDeviceReset() { return true; }
CGameComponent::~CGameComponent() {}
void CGameComponent::Init() {}
bool CGameComponent::Run(CGameComponent**) { return false; }
void CGameComponent::ChangeTextureColourDepth(long) {}
void CGameComponent::SetQuit() { Quit = true; }
CNewFrontendGameComponent::~CNewFrontendGameComponent() {}
bool CNewFrontendGameComponent::Run(CGameComponent**)
{
    Require(this == expectedFrontend && XMVCodeLoaded &&
            g_FableNewFrontend_013B871C == this, "Play dispatches initialized frontend");
    Event('R');
    return false; // The unreconstructed frame loop is the explicit boundary.
}
void CNewFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CNewFrontendGameComponent::SetQuit() { Quit = true; }
void CNewFrontendGameComponent::OnPreDeviceReset()
{
    Require(this == expectedFrontend, "secondary callback this adjustment");
}
bool CNewFrontendGameComponent::OnPostDeviceReset()
{
    Require(this == expectedFrontend, "secondary callback this adjustment");
    return true;
}

CWideString::CWideString() : storage_(0) { Event('W'); }
CWideString::CWideString(const wchar_t* text) : storage_(0)
{
    Require(text && !*text, "empty SaveGame literal");
    Event('E');
}
CWideString::~CWideString() {}
const CWideString& CWideString::operator=(const wchar_t* text)
{
    Require(text && !*text, "empty startup path literal");
    Require(this == &expectedFrontend->m_bInitMainGame.InitialWorldName ||
            this == &expectedFrontend->m_bInitMainGame.SaveGameName,
            "assigned startup string receiver");
    Event('A');
    return *this;
}
CCharString::CCharString() : storage_(0) { Event('C'); }
CCharString::~CCharString() {}
void CNewFrontendGameComponent::CompileDefs() { Require(false, "unexpected compile-defs branch"); }
CMainGameComponent::CMainGameComponent(CGame& game, const CMainGameComponentInit&)
    : CGameComponent(game) { Require(false, "unexpected main-game constructor"); }
CFrontendGameComponent::CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit&)
    : CGameComponent(game) { Require(false, "unexpected legacy constructor"); }
CWideString& CWideString::operator=(const CWideString&)
{
    Require(false, "unexpected main-game startup-path copy");
    return *this;
}

void* __cdecl operator new(size_t size)
{
    void* memory = malloc(size);
    Require(memory != 0, "fixture allocation");
    if (size == sizeof(CNewFrontendGameComponent))
        expectedFrontend = static_cast<CNewFrontendGameComponent*>(memory);
    return memory;
}
void __cdecl operator delete(void* memory) { free(memory); }

double FABLE_CDECL GFGetTime()
{
    Event('T');
    return ++timeCalls == 1 ? 12.5 : 37.25;
}
static int __stdcall Frequency(FablePerformanceCounterValue* frequency)
{
    const CStopWatch* ordered[] = { &expectedFrontend->m_RepeatTimer,
        &expectedFrontend->m_attractModeTimer, &expectedFrontend->m_inputDelayTimer };
    Require(stopwatchCalls < 3, "timer construction count");
    const CStopWatch* watch = ordered[stopwatchCalls++];
    Require(!watch->m_fTimerPeriod && !watch->m_nStartTick &&
        !watch->m_nPrevElapsedTicks && !watch->m_bIsRunning,
        "timer state initialized before frequency query");
    Event('S');
    frequency->QuadPart = 100;
    return 1;
}
extern "C" {
    int (__stdcall* FableTestFrequencyImport)(FablePerformanceCounterValue*) = Frequency;
}

void CNewFrontendGameComponent::InitialiseDefs()
{
    Require(this == expectedFrontend && g_FableNewFrontend_013B871C == this,
            "publish frontend before definitions");
    Require(!XMVCodeLoaded, "XMV flag before load");
    m_pMenuFont.Data = expectedFont;
    Event('D');
}
void FABLE_CDECL StaticLoadXMVCode()
{
    Require(g_FableNewFrontend_013B871C == expectedFrontend &&
            !expectedFrontend->XMVCodeLoaded, "XMV flag changes after loader");
    Event('L');
}
CConsole* FABLE_CDECL FableGetConsole_00414C90()
{
    Require(expectedFrontend->XMVCodeLoaded, "XMV flag before console lookup");
    Event('G');
    return &console;
}
void CConsole::Initialise(char toggleCharacter, int toggleKey, CFontBank* font)
{
    Require(this == &console && toggleCharacter == 0x60 && toggleKey == 0x29 && font == expectedFont,
            "console receiver, activation keys and font");
    Event('I');
}

static void CheckFrontend(bool hasFont)
{
    union { double alignment; unsigned char bytes[sizeof(CNewFrontendGameComponent)]; } storage;
    memset(storage.bytes, 0xA5, sizeof(storage.bytes));
    CGame game;
    CNewFrontendGameComponentInit init;
    init.value = 0xDEADBEEF; // Retail never reads the initializer's placeholder.
    expectedFrontend = reinterpret_cast<CNewFrontendGameComponent*>(storage.bytes);
    expectedFont = hasFont ? reinterpret_cast<CFontBank*>(&fontToken) : 0;
    ResetEvents();
    timeCalls = stopwatchCalls = 0;
    CNewFrontendGameComponent* frontend = new (storage.bytes) CNewFrontendGameComponent(game, init);

    Require(sizeof(*frontend) == 0x148 && sizeof(CGameComponent) == 0x10, "retail sizes");
    Require(frontend->m_RepeatTimer.m_fTimerPeriod == 0.01f &&
        frontend->m_attractModeTimer.m_fTimerPeriod == 0.01f &&
        frontend->m_inputDelayTimer.m_fTimerPeriod == 0.01f, "real stopwatch construction");
#define CHECK_OFFSET(member, offset) Require(reinterpret_cast<unsigned char*>(&frontend->member) - storage.bytes == offset, #member " offset")
    CHECK_OFFSET(Quit, 0x08); CHECK_OFFSET(Running, 0x09); CHECK_OFFSET(Game, 0x0C);
    CHECK_OFFSET(m_pMenuFont, 0x10); CHECK_OFFSET(TimeLastMovementHappened, 0x20);
    CHECK_OFFSET(m_bInitMainGame, 0x2C); CHECK_OFFSET(m_pInputManager, 0x68);
    CHECK_OFFSET(FileNames, 0x6C); CHECK_OFFSET(FileTimes, 0x78); CHECK_OFFSET(SaveGame, 0x84);
    CHECK_OFFSET(AVIPlaying, 0xB0); CHECK_OFFSET(XMVCodeLoaded, 0xB1);
    CHECK_OFFSET(m_pFrontEndManager, 0xB4); CHECK_OFFSET(m_fLastRealUpdateTime, 0xC0);
    CHECK_OFFSET(CreatedMarkerFile, 0xC8); CHECK_OFFSET(m_LastInterp, 0xCC);
    CHECK_OFFSET(m_RepeatTimer, 0xD8); CHECK_OFFSET(m_attractModeTimer, 0x100);
    CHECK_OFFSET(m_inputDelayTimer, 0x120); CHECK_OFFSET(WindowsMediaPlayerInstalled, 0x144);
#undef CHECK_OFFSET
    Require(reinterpret_cast<unsigned char*>(static_cast<CDeviceResetCallback*>(frontend)) - storage.bytes == 4,
            "device callback secondary base");
    Require(!frontend->Quit && !frontend->Running && frontend->Game == &game, "base state");
    Require(!frontend->CurrentMovement && !frontend->StartedRepeatingMovement &&
            frontend->TimeLastMovementHappened == 12.5 && frontend->m_fLastRealUpdateTime == 37.25,
            "movement and separate time samples");
    Require(frontend->m_bAllowInputs && !frontend->m_bCreateGame && !frontend->CreateDevFrontEnd &&
            !frontend->CreatedMarkerFile && frontend->WindowsMediaPlayerInstalled, "frontend flags");
    Require(!frontend->m_pFrontEndDef && !frontend->m_pFrontEndManager &&
            !frontend->PAVITextureShow && !frontend->PAVITextureDecode &&
            !frontend->AVIPlaying && !frontend->XMVCodeLoaded, "resource defaults");
    Require(Poisoned(&frontend->m_pInputManager, sizeof(frontend->m_pInputManager)) &&
            Poisoned(&frontend->AVISrcBox, sizeof(frontend->AVISrcBox)) &&
            Poisoned(&frontend->AVIDstBox, sizeof(frontend->AVIDstBox)) &&
            Poisoned(&frontend->m_fLastUpdateTime, sizeof(float)) &&
            Poisoned(&frontend->m_fLastRenderTime, sizeof(float)), "retail deferred initialization");
    Require(Poisoned(storage.bytes + 0x1D, 3), "padding not wholesale cleared");
    Require(*reinterpret_cast<unsigned long*>(&frontend->m_fRepeatDelay) == 0x3EAA7EFA && !frontend->m_dwDPadTriggerState &&
            !frontend->LastActiveJoystickDeviceNumber, "repeat and joystick defaults");
    Require(!frontend->m_pMenuFont.Data && !frontend->m_pMenuFont.Info &&
            !frontend->m_pFrontEndGraphicBank.Data && !frontend->m_pFrontEndGraphicBank.Info &&
            !frontend->m_pMeshBank.Data && !frontend->m_pMeshBank.Info &&
            !frontend->PSampleBank.Data && !frontend->PSampleBank.Info &&
            !frontend->m_pEngine.Data && !frontend->m_pEngine.Info &&
            !frontend->m_pTextBank.Data && !frontend->m_pTextBank.Info,
            "counted-pointer storage initialized");
    Require(!frontend->FileNames.Begin && !frontend->FileNames.End && !frontend->FileNames.CapacityEnd &&
            !frontend->FileTimes.Begin && !frontend->FileTimes.End && !frontend->FileTimes.CapacityEnd,
            "retail vector storage initialized");
    Require(frontend->m_LastInterp.GTPredictedRenderAt == 0 &&
            frontend->m_LastInterp.GTPredictedTimeSinceLastRenderFrame == 0 &&
            frontend->m_LastInterp.WFInterpolate == 0, "interpolation defaults");
    Require(strcmp(events, "BTWWCWETSSSAA") == 0, "constructor dependency sequence");
    Require(timeCalls == 2 && stopwatchCalls == 3, "constructor dependency cardinality");
    ResetEvents();
    CGameComponent* component = frontend;
    component->Init(); // Exercise the actual primary virtual table's Init slot.
    Require(strcmp(events, "DLGI") == 0, "Init dependency sequence");
    CDeviceResetCallback* callback = frontend;
    callback->OnPreDeviceReset();
    Require(callback->OnPostDeviceReset(), "secondary callback virtual dispatch");
    frontend->~CNewFrontendGameComponent();
    g_FableNewFrontend_013B871C = 0;
}

static void CheckPlayStartupChain()
{
    CGame game;
    memset(&game, 0, sizeof(game));
    ResetEvents();
    timeCalls = stopwatchCalls = 0;
    expectedFont = reinterpret_cast<CFontBank*>(&fontToken);
    expectedFrontend = 0;
    game.Play();
    Require(strcmp(events, "BTWWCWETSSSAADLGIR") == 0, "Play -> constructor -> Init -> Run");
    CGameComponent* current = *reinterpret_cast<CGameComponent**>(
        reinterpret_cast<unsigned char*>(&game) + 8);
    Require(current == expectedFrontend && current->Game == &game,
            "Play retains constructed frontend and owning game");
    Require(*(reinterpret_cast<unsigned char*>(&game) + 0x20C) == 1,
            "Play latches quit after frame-loop boundary returns false");
    delete current;
    g_FableNewFrontend_013B871C = 0;
}

int main()
{
    CheckFrontend(true);
    CheckFrontend(false);
    CheckPlayStartupChain();
    puts("FABLETLC_FRONTEND_STARTUP_BEHAVIOR PASS");
    return 0;
}
