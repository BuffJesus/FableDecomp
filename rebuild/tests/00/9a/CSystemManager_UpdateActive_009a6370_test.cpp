// Behaviour of CSystemManager::UpdateActive with scripted stand-ins: the external-
// window and quit early-outs, WaitMessage only while inactive+waiting, focus/active
// tracking against the foreground window (with the input manager told), the
// device-lost sleep-and-retry path, and the loop exits.
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

// No user32.lib in the toolset: the source object's import slots are satisfied
// with scripted stand-ins (Sleep stays the real kernel32 import).
static char g_log[128]; static int g_n;
static void Log(char c) { if (g_n < 127) g_log[g_n++] = c; g_log[g_n] = 0; }
static void* g_fgScript[8]; static int g_fg;
extern "C" void* FABLE_STDCALL FakeGetForegroundWindow() { Log('F'); return g_fgScript[g_fg++]; }
extern "C" int FABLE_STDCALL FakeWaitMessage() { Log('W'); return 1; }
extern "C" void* (FABLE_STDCALL *g_FakeGetForegroundWindowSlot)() = FakeGetForegroundWindow;
extern "C" int (FABLE_STDCALL *g_FakeWaitMessageSlot)() = FakeWaitMessage;
#pragma comment(linker, "/alternatename:__imp__GetForegroundWindow@0=_g_FakeGetForegroundWindowSlot")
#pragma comment(linker, "/alternatename:__imp__WaitMessage@0=_g_FakeWaitMessageSlot")

static unsigned char g_obj[sizeof(CSystemManager)];
static unsigned char g_input[16];
static unsigned char g_display[16];
static bool g_deviceScript[8]; static int g_dev;
static int g_quitAfterMessages;
static bool g_focusArgs[8]; static const void* g_focusThis[8]; static int g_focus;
static const void* g_messagesThis; static const void* g_deviceThis;

void FABLE_FASTCALL GFRecalibrateFastSubFrameTimer() { Log('R'); }
void CSystemManager::ProcessWindowMessages()
{
    Log('M'); g_messagesThis = this;
    if (--g_quitAfterMessages == 0) WindowsQuit = true;
}
void CInputManager::SetHasInputFocus(bool hasFocus) { Log(hasFocus ? 'I' : 'i'); g_focusThis[g_focus] = this; g_focusArgs[g_focus++] = hasFocus; }
bool CDisplayManager::ProcessLostDevice() { Log('D'); g_deviceThis = this; return g_deviceScript[g_dev++]; }

static void* const kWindow = reinterpret_cast<void*>(0x00330044);
static void* const kOther = reinterpret_cast<void*>(0x00550066);

static CSystemManager* Setup(bool external, bool quit, bool active, bool everActive, bool wait, bool input)
{
    memset(g_obj, 0, sizeof(g_obj));
    g_obj[0xEC] = external; g_obj[0x08] = quit; g_obj[0x09] = active; g_obj[0xDC] = everActive; g_obj[0xEB] = wait;
    *reinterpret_cast<void**>(g_obj + 0x94) = kWindow;
    *reinterpret_cast<void**>(g_obj + 0x58) = input ? g_input : 0;
    *reinterpret_cast<void**>(g_obj + 0x60) = g_display;
    g_n = 0; g_log[0] = 0; g_fg = 0; g_dev = 0; g_focus = 0; g_quitAfterMessages = -1;
    for (int i = 0; i < 8; ++i) { g_fgScript[i] = kWindow; g_deviceScript[i] = true; }
    return reinterpret_cast<CSystemManager*>(g_obj);
}

int main()
{
    // Attached to an external window: recalibrate only.
    Setup(true, false, false, false, true, true)->UpdateActive();
    if (strcmp(g_log, "R") != 0) { printf("FAIL external %s\n", g_log); return 1; }

    // Already quitting: recalibrate only.
    Setup(false, true, false, false, true, true)->UpdateActive();
    if (strcmp(g_log, "R") != 0) { printf("FAIL quit %s\n", g_log); return 2; }

    // Active in the foreground: one pass, no WaitMessage, focus told to the input manager.
    CSystemManager* m = Setup(false, false, true, true, true, true);
    m->UpdateActive();
    if (strcmp(g_log, "RMFID") != 0 || g_messagesThis != m || g_focusThis[0] != g_input || g_deviceThis != g_display ||
        g_obj[0x09] != 1 || g_obj[0x0A] != 1 || g_obj[0xDC] != 1 || g_obj[0xEA] != 0) { printf("FAIL active %s\n", g_log); return 3; }

    // Inactive and waiting: background pass, then WaitMessage and a foreground pass (Restore raised).
    Setup(false, false, false, true, true, true);
    g_fgScript[0] = kOther;
    m->UpdateActive();
    if (strcmp(g_log, "RWMFiDWMFID") != 0 || g_obj[0x09] != 1 || g_obj[0x0A] != 1 || g_obj[0xEA] != 1 || g_focus != 2 || g_focusArgs[0] != false || g_focusArgs[1] != true)
    { printf("FAIL wait %s\n", g_log); return 4; }

    // Not waiting while inactive: a single background pass.
    Setup(false, false, false, false, false, false);
    g_fgScript[0] = kOther;
    m->UpdateActive();
    if (strcmp(g_log, "RMFD") != 0 || g_obj[0x09] != 0 || g_obj[0x0A] != 0 || g_obj[0xDC] != 1 || g_focus != 0) { printf("FAIL nowait %s\n", g_log); return 5; }

    // Device lost: Sleep(10) and go round again even though active.
    Setup(false, false, true, true, true, false);
    g_deviceScript[0] = false;
    m->UpdateActive();
    if (strcmp(g_log, "RMFDMFD") != 0) { printf("FAIL lost device %s\n", g_log); return 6; }

    // Quit raised by the message pump ends the wait loop.
    Setup(false, false, false, false, true, false);
    g_fgScript[0] = kOther; g_quitAfterMessages = 2;
    m->UpdateActive();
    if (strcmp(g_log, "RWMFDWMFD") != 0 || g_obj[0x08] != 1) { printf("FAIL quit in loop %s\n", g_log); return 7; }

    printf("FABLETLC_SYSMGR_UPDATEACTIVE PASS\n");
    return 0;
}