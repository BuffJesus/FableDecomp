#include "fable_game.h"
#include "fable_console.h"
#include "test_support/console_member_stubs.h"
#include "fable_input_event.h"
#include "fable_nui_manager.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>
#include "test_support/base_class_stubs.h"

// ---- recording log -------------------------------------------------------
enum
{
    L_CLEAR = 1000, L_UPDATE, L_SCAN_START, L_SCAN_NEXT, L_SCAN_END, L_CONSOLE,
    L_KEY, L_EVENT, L_ATTRACT_RESET, L_REPEAT_ELAPSED, L_REPEAT_STARTZERO,
    L_REPEAT_STOP, L_OTHER_TIMER, L_JOY_GET
};
static long logv[512];
static int logn;
static void Log(long a) { if (logn < 512) logv[logn++] = a; }
static void Log2(long a, long b) { Log(a); Log(b); }
static bool ok;
static void Expect(bool c, int line) { if (!c) { if (ok) printf("fail line %d\n", line); ok = false; } }
#define EXPECT(c) Expect((c), __LINE__)

// ---- component storage -----------------------------------------------------
static unsigned long storage[sizeof(CNewFrontendGameComponent) / 4];
static unsigned char* const comp = reinterpret_cast<unsigned char*>(storage);
static CNewFrontendGameComponent* const self = reinterpret_cast<CNewFrontendGameComponent*>(storage);
template <class T> static T& At(int off) { return *reinterpret_cast<T*>(comp + off); }

// ---- scripted events --------------------------------------------------------
struct Scripted
{
    EInputEventType type;
    long key;          // EInputKey or wchar_t
    long device;
    long button;
    float wheel;
    float jx, jy;
    float mx, my;
};
static const Scripted* script;
static int scriptCount, scriptPos;
static const Scripted* cur;
static CInputEvent* scanEvent;
static CInputEventScanInfo* scanInfo;
static int eventCtors, eventDtors;

CInputEvent::CInputEvent() { ++eventCtors; memset(this, 0, sizeof(*this)); }
CInputEvent::~CInputEvent() { ++eventDtors; }
EInputEventType CInputEvent::GetType() const { EXPECT(this == scanEvent); return cur->type; }
long CInputEvent::GetDeviceNumber() const { EXPECT(this == scanEvent); return cur->device; }
EInputKey CInputEvent::GetKey() const { EXPECT(this == scanEvent); return EInputKey(cur->key); }
wchar_t CInputEvent::GetChar() const { EXPECT(this == scanEvent); return wchar_t(cur->key); }
long CInputEvent::GetJoystickButton() const { EXPECT(this == scanEvent); return cur->button; }
float CInputEvent::GetMouseWheelMovement() const { EXPECT(this == scanEvent); return cur->wheel; }
C2DVector CInputEvent::GetJoystickPosition() const { EXPECT(this == scanEvent); return C2DVector(cur->jx, cur->jy); }

static bool Load(CInputEventScanInfo& info, CInputEvent& event)
{
    EXPECT(&info == scanInfo && &event == scanEvent);
    if (scriptPos >= scriptCount) return false;
    cur = &script[scriptPos++];
    float* raw = reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(&event) + 0x0C);
    raw[0] = cur->mx; raw[1] = cur->my; raw[2] = 99.0f;
    return true;
}

CInputManager* g_FableInputManager_013B8388;
bool CInputManager::SetExclusiveMode(bool) { Log(9999); return false; }
void CInputManager::Update() { EXPECT(this == g_FableInputManager_013B8388); Log(L_UPDATE); }
void CInputManager::ClearEvents() { EXPECT(this == g_FableInputManager_013B8388); Log(L_CLEAR); }
bool CInputManager::EventScanStart(CInputEventScanInfo& info, CInputEvent& event) const
{
    EXPECT(this == g_FableInputManager_013B8388);
    EXPECT(info.Started == false);
    Log(L_SCAN_START);
    scanInfo = &info; scanEvent = &event;
    return Load(info, event);
}
bool CInputManager::EventScanNext(CInputEventScanInfo& info, CInputEvent& event) const
{
    EXPECT(this == g_FableInputManager_013B8388);
    Log(L_SCAN_NEXT);
    return Load(info, event);
}
void CInputManager::EventScanEnd(CInputEventScanInfo& info) const
{
    EXPECT(this == g_FableInputManager_013B8388 && &info == scanInfo);
    Log(L_SCAN_END);
}

const float DELTA_NOTIONAL_ZERO = 0.0001f;

// ---- console -------------------------------------------------------------------
CCountedPointer<CConsole> g_FableConsolePClass_013CAA40;
CConsole::CConsole() {}
template<> void CCountedPointer<CConsole>::Reset(CConsole* console) { Data = console; }
bool CConsole::ProcessInput(const CInputEvent& event)
{
    EXPECT(this == g_FableConsolePClass_013CAA40.Data && &event == scanEvent);
    Log2(L_CONSOLE, cur->key);
    return true;
}

// ---- joystick manager ---------------------------------------------------------------
static bool joyExplicit;
static long joyMain;
static unsigned char joyStorage[0x0C];
CGameJoystickManager& FABLE_FASTCALL CGameJoystickManager::Get() { return *reinterpret_cast<CGameJoystickManager*>(joyStorage); }
bool CGameJoystickManager::HasExplicitMainPlayerJoystickDeviceNumber() const { EXPECT((void*)this == joyStorage); return joyExplicit; }
long CGameJoystickManager::GetMainPlayerJoystickDeviceNumber() const { EXPECT((void*)this == joyStorage); return joyMain; }

// ---- UI manager ---------------------------------------------------------------------
namespace NUISystem
{
    void CObservable::SetEvent(EEvent) { Log(9999); }
    void CObservable::SetOwnedEvent(CObserver*, EEvent) { Log(9999); }
    void CObservable::AddObserver(CObserver*) { Log(9999); }
    void CObservable::AddExclusiveObserver(CObserver*) { Log(9999); }
    void CObservable::RemoveExclusiveObserver(CObserver*) { Log(9999); }
    void CObservable::RemoveObserver(CObserver*) { Log(9999); }
    void CObservable::ClearObservers() { Log(9999); }
    CObserver* CObservable::GetExclusiveObserver() { Log(9999); return 0; }
    bool CObservable::IsExclusivelyObserved() { Log(9999); return false; }
    void CObservable::AddConcurrentExclusiveObserver(CObserver*) { Log(9999); }
    void CObservable::RemoveConcurrentExclusiveObserver(CObserver*) { Log(9999); }
    bool CManager::GetMouseDisabled() { Log(9999); return false; }
    void CManager::SetKeyPressed(wchar_t) { Log(9999); }
    wchar_t CManager::GetKeyPressed() { Log(9999); return 0; }
    // fable_nui_manager.h declares retail's out-of-line ctor (0x0041E3F6).
    CManager::CManager() {}
}
class CFakeManager : public NUISystem::CManager
{
public:
    virtual void SetEvent(NUISystem::EEvent event) { EXPECT(this == fake); Log2(L_EVENT, event); }
    virtual void SetKeyPressed(wchar_t key) { EXPECT(this == fake); Log2(L_KEY, key); }
    static CFakeManager* fake;
};
CFakeManager* CFakeManager::fake;
NUISystem::CManager* FABLE_FASTCALL NUISystem::CManager::GetInstance() { return CFakeManager::fake; }

// ---- stopwatches ---------------------------------------------------------------------
static float elapsed;
static void TimerLog(const void* t, long what)
{
    if (t == comp + 0xD8) Log(what);
    else if (t == comp + 0x100 && what == L_REPEAT_STOP + 100) Log(L_ATTRACT_RESET);
    else Log2(L_OTHER_TIMER, what);
}
void CStopWatch::Reset() { TimerLog(this, L_REPEAT_STOP + 100); }
float CStopWatch::GetElapsedSeconds() const { TimerLog(this, L_REPEAT_ELAPSED); return elapsed; }
void CStopWatch::StartZero() { TimerLog(this, L_REPEAT_STARTZERO); }
void CStopWatch::Stop() { TimerLog(this, L_REPEAT_STOP); }

// ---- harness ----------------------------------------------------------------------------
static void Reset(const Scripted* s, int n)
{
    memset(storage, 0xA5, sizeof(storage));
    At<bool>(0x28) = true;             // m_bAllowInputs
    At<bool>(0xF0) = false;            // m_RepeatTimer.m_bIsRunning
    At<float>(0xF8) = 1.0f;            // m_fRepeatDelay
    At<unsigned long>(0xFC) = 0;       // m_dwDPadTriggerState
    At<bool>(0x138) = false;           // m_inputDelayTimer.m_bIsRunning
    At<long>(0x140) = -1;              // LastActiveJoystickDeviceNumber
    script = s; scriptCount = n; scriptPos = 0; cur = 0;
    logn = 0; eventCtors = eventDtors = 0;
    joyExplicit = false; joyMain = 0; elapsed = 0.0f;
    CFakeManager::fake->MouseMovement = C2DVector(-7.0f, -7.0f);
}
static bool Same(const long* expected, int n)
{
    bool same = n == logn && memcmp(expected, logv, n * sizeof(long)) == 0;
    if (!same)
    {
        printf("log (%d):", logn);
        for (int i = 0; i < logn; ++i) printf(" %ld", logv[i]);
        printf("\n");
    }
    return same;
}
static bool UntouchedExcept(const int* offs, int n)
{
    // Every byte other than the listed dword offsets and the fields the
    // harness seeded keeps its 0xA5 fill.
    for (int i = 0; i < (int)sizeof(storage); ++i)
    {
        bool skip = i == 0x28 || (i >= 0xF0 && i < 0x100) || i == 0x138 || (i >= 0x140 && i < 0x144);
        for (int k = 0; k < n; ++k) if (i >= offs[k] && i < offs[k] + 4) skip = true;
        if (!skip && comp[i] != 0xA5) return false;
    }
    return true;
}
#define SAME(arr) Same(arr, sizeof(arr) / sizeof(arr[0]))

static bool FABLE_FASTCALL InputSetExclusiveModeShim(CInputManager* self, void*, bool on) { return self->CInputManager::SetExclusiveMode(on); }
static void FABLE_FASTCALL InputUpdateShim(CInputManager* self) { self->CInputManager::Update(); }

int main()
{
    ok = true;
    CFakeManager manager;
    CFakeManager::fake = &manager;
    // CInputManager has no default constructor: raw storage whose vtable
    // forwards slots 1 and 2 to this fixture's SetExclusiveMode / Update.
    static union { double align; char bytes[sizeof(CInputManager)]; } inputStorage;
    static void* inputTable[3] = { 0, (void*)InputSetExclusiveModeShim, (void*)InputUpdateShim };
    *reinterpret_cast<void**>(&inputStorage) = inputTable;
    CInputManager& inputManager = *reinterpret_cast<CInputManager*>(&inputStorage);
    g_FableInputManager_013B8388 = &inputManager;
    g_FableConsolePClass_013CAA40.Data = new CConsole;

    // 1. Input delay running: clear events, nothing else.
    Reset(0, 0);
    At<bool>(0x138) = true;
    self->Input();
    { const long e[] = { L_CLEAR }; EXPECT(SAME(e)); }
    EXPECT(eventCtors == 0 && UntouchedExcept(0, 0));

    // 2. Every handled event type; no joystick state; LEFT dispatch wins.
    {
        static const Scripted s[] = {
            { IE_KEY_PRESSED, KB_LEFT, 0, 0, 0, 0, 0, 0, 0 },
            { IE_CHAR_PRESSED, L'x', 0, 0, 0, 0, 0, 0, 0 },
            { IE_LMB_PRESSED }, { IE_LMB_RELEASED }, { IE_MMB_PRESSED }, { IE_RMB_PRESSED },
            { IE_RMB_RELEASED }, { IE_KEY_RELEASED },
            { IE_MOUSE_MOVEMENT, 0, 0, 0, 0, 0, 0, 1.5f, -2.5f },
            { IE_MOUSE_WHEEL_MOVEMENT, 0, 0, 0, 0.5f },
            { IE_MOUSE_WHEEL_MOVEMENT, 0, 0, 0, -0.5f },
            { IE_MOUSE_WHEEL_MOVEMENT, 0, 0, 0, 0.00005f },
            { IE_MB4_PRESSED }, { IE_MB5_PRESSED }, { IE_MB6_PRESSED }, { IE_MB7_PRESSED }, { IE_MB8_PRESSED },
        };
        Reset(s, sizeof(s) / sizeof(s[0]));
        self->Input();
        const long e[] = {
            L_UPDATE, L_SCAN_START,
            L_CONSOLE, KB_LEFT, L_KEY, KB_LEFT, L_EVENT, 33, L_SCAN_NEXT,
            L_KEY, L'x', L_EVENT, 34, L_SCAN_NEXT,
            L_EVENT, 26, L_SCAN_NEXT, L_EVENT, 28, L_SCAN_NEXT, L_EVENT, 35, L_SCAN_NEXT, L_EVENT, 27, L_SCAN_NEXT,
            L_SCAN_NEXT, L_SCAN_NEXT,
            L_EVENT, 25, L_SCAN_NEXT,
            L_EVENT, 36, L_SCAN_NEXT, L_EVENT, 37, L_SCAN_NEXT, L_SCAN_NEXT,
            L_EVENT, 38, L_SCAN_NEXT, L_EVENT, 39, L_SCAN_NEXT, L_EVENT, 40, L_SCAN_NEXT,
            L_EVENT, 41, L_SCAN_NEXT, L_EVENT, 42, L_SCAN_NEXT,
            L_SCAN_END,
            L_ATTRACT_RESET, L_REPEAT_STARTZERO,
            L_EVENT, 2, L_EVENT, 20 };
        EXPECT(SAME(e));
        EXPECT(manager.MouseMovement.X == 1.5f && manager.MouseMovement.Y == -2.5f);
        EXPECT(At<float>(0xF8) == 0.333f && eventCtors == 2 && eventDtors == 2);
        EXPECT(UntouchedExcept(0, 0));
    }

    // 3. No input at all: repeat timer stopped, delay reset, no dispatch.
    Reset(0, 0);
    self->Input();
    { const long e[] = { L_UPDATE, L_SCAN_START, L_SCAN_END, L_REPEAT_STOP }; EXPECT(SAME(e)); }
    EXPECT(At<float>(0xF8) == 0.333f && eventDtors == 2);

    // 4. Repeat timer running, not yet due: returns before dispatch.
    {
        static const Scripted s[] = { { IE_KEY_PRESSED, KB_A } };
        Reset(s, 1);
        At<bool>(0xF0) = true; At<float>(0xF8) = 0.5f; elapsed = 0.25f;
        self->Input();
        const long e[] = { L_UPDATE, L_SCAN_START, L_CONSOLE, KB_A, L_KEY, KB_A, L_EVENT, 33, L_SCAN_NEXT,
                           L_SCAN_END, L_ATTRACT_RESET, L_REPEAT_ELAPSED };
        EXPECT(SAME(e));
        EXPECT(At<float>(0xF8) == 0.5f && eventDtors == 2);
    }

    // 5. Repeat timer running and due: fast repeat, A -> SELECT, Y key sets Y.
    {
        static const Scripted s[] = { { IE_KEY_PRESSED, KB_A }, { IE_KEY_PRESSED, KB_Y } };
        Reset(s, 2);
        At<bool>(0xF0) = true; At<float>(0xF8) = 0.5f; elapsed = 0.5f;
        self->Input();
        const long e[] = { L_UPDATE, L_SCAN_START, L_CONSOLE, KB_A, L_KEY, KB_A, L_EVENT, 33, L_SCAN_NEXT,
                           L_CONSOLE, KB_Y, L_KEY, KB_Y, L_EVENT, 33, L_SCAN_NEXT,
                           L_SCAN_END, L_ATTRACT_RESET, L_REPEAT_ELAPSED, L_REPEAT_STARTZERO, L_EVENT, 4 };
        EXPECT(SAME(e));
        EXPECT(At<float>(0xF8) == 0.085f);
    }

    // 6. Joystick: stick, buttons from another device ignored, D-pad latch.
    {
        static const Scripted s[] = {
            { IE_JOYSTICK_POSITION, 0, 0, 0, 0, 0.5f, 0.1f },          // right
            { IE_JOYSTICK_BUTTON_PRESSED, 0, 3, XBOX_PAD_B_BUTTON },    // wrong device
            { IE_JOYSTICK_BUTTON_PRESSED, 0, 2, XBOX_PAD_DPAD_UP_BUTTON },
            { IE_JOYSTICK_BUTTON_PRESSED, 0, 2, XBOX_PAD_DPAD_RIGHT_BUTTON },
            { IE_JOYSTICK_BUTTON_RELEASED, 0, 2, XBOX_PAD_DPAD_RIGHT_BUTTON },
            { IE_JOYSTICK_BUTTON_PRESSED, 0, 2, XBOX_PAD_X_BUTTON },
        };
        Reset(s, sizeof(s) / sizeof(s[0]));
        joyExplicit = true; joyMain = 2;
        self->Input();
        const long e[] = { L_UPDATE, L_SCAN_START, L_SCAN_NEXT, L_SCAN_NEXT, L_SCAN_NEXT, L_SCAN_NEXT,
                           L_SCAN_NEXT, L_SCAN_NEXT, L_SCAN_END, L_ATTRACT_RESET, L_REPEAT_STARTZERO,
                           L_EVENT, 3, L_EVENT, 21 };
        EXPECT(SAME(e));
        EXPECT(At<unsigned long>(0xFC) == 0x10 && At<long>(0x140) == 2);
    }

    // 7. D-pad latch alone drives UP; stick down; allow-inputs off ignores pad.
    {
        static const Scripted s[] = {
            { IE_JOYSTICK_POSITION, 0, 0, 0, 0, 0.0f, -0.5f },
            { IE_JOYSTICK_BUTTON_PRESSED, 0, 5, XBOX_PAD_START_BUTTON },
        };
        Reset(s, 2);
        At<bool>(0x28) = false;
        At<unsigned long>(0xFC) = 0x10;
        self->Input();
        const long e[] = { L_UPDATE, L_SCAN_START, L_SCAN_NEXT, L_SCAN_NEXT, L_SCAN_END,
                           L_ATTRACT_RESET, L_REPEAT_STARTZERO, L_EVENT, 0 };
        EXPECT(SAME(e));
        EXPECT(At<long>(0x140) == -1);
    }

    // 8. Stick up + START from the main pad: START wins (SELECT).
    {
        static const Scripted s[] = {
            { IE_JOYSTICK_POSITION, 0, 0, 0, 0, -0.5f, 0.5f },
            { IE_JOYSTICK_BUTTON_PRESSED, 0, 1, XBOX_PAD_BACK_BUTTON },
        };
        Reset(s, 2);
        self->Input();
        const long e[] = { L_UPDATE, L_SCAN_START, L_SCAN_NEXT, L_SCAN_NEXT, L_SCAN_END,
                           L_ATTRACT_RESET, L_REPEAT_STARTZERO, L_EVENT, 5 };
        EXPECT(SAME(e));
        EXPECT(At<long>(0x140) == 1);
    }

    if (!ok) return 1;
    puts("FABLETLC_NEW_FRONTEND_INPUT PASS");
    return 0;
}