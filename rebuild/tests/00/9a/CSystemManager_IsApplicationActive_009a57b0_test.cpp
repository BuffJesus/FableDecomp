// Behaviour of CSystemManager::IsApplicationActive: active iff ApplicationWinHandle
// (+0x94) is the foreground window (GetForegroundWindow, one call); ApplicationActive (+0x09) and
// EverBeenActive (+0x0DC) are updated; Restore (+0x0EA) is raised only on an
// inactive->active transition after the application has been active before.
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

// The toolset ships no user32.lib: satisfy the source object's import slot
// (__imp__GetForegroundWindow@0) with a scripted stand-in.
static void* g_foreground;
static int g_foregroundCalls;
extern "C" void* FABLE_STDCALL FakeGetForegroundWindow() { ++g_foregroundCalls; return g_foreground; }
extern "C" void* (FABLE_STDCALL *g_FakeGetForegroundWindowSlot)() = FakeGetForegroundWindow;
#pragma comment(linker, "/alternatename:__imp__GetForegroundWindow@0=_g_FakeGetForegroundWindowSlot")

static unsigned char g_obj[sizeof(CSystemManager)];

static void Setup(void* window, unsigned char active, unsigned char everActive)
{
    memset(g_obj, 0, sizeof(g_obj));
    *reinterpret_cast<void**>(g_obj + 0x94) = window;
    g_obj[0x09] = active;
    g_obj[0xDC] = everActive;
}

int main()
{
    CSystemManager* manager = reinterpret_cast<CSystemManager*>(g_obj);
    g_foreground = reinterpret_cast<void*>(0x00120ABC);
    void* foreground = g_foreground;
    void* other = reinterpret_cast<void*>(reinterpret_cast<char*>(foreground) + 0x10001);

    // Not the foreground window: inactive, EverBeenActive still latched.
    Setup(other, 1, 0);
    if (manager->IsApplicationActive() != false || g_obj[0x09] != 0 || g_obj[0xDC] != 1 || g_obj[0xEA] != 0) { printf("FAIL background\n"); return 1; }

    // Foreground, first activation: active, no Restore.
    Setup(foreground, 0, 0);
    if (manager->IsApplicationActive() != true || g_obj[0x09] != 1 || g_obj[0xDC] != 1 || g_obj[0xEA] != 0) { printf("FAIL first\n"); return 2; }

    // Foreground again after having been active and then inactive: Restore raised.
    Setup(foreground, 0, 1);
    if (manager->IsApplicationActive() != true || g_obj[0xEA] != 1) { printf("FAIL restore\n"); return 3; }

    // Already active: no Restore.
    Setup(foreground, 1, 1);
    if (manager->IsApplicationActive() != true || g_obj[0xEA] != 0) { printf("FAIL steady\n"); return 4; }

    // Neighbouring flags (WindowsQuit +0x08, HasInputFocus +0x0A, WindowValid +0x0E8) untouched.
    if (g_obj[0x08] != 0 || g_obj[0x0A] != 0 || g_obj[0xE8] != 0 || g_obj[0xEB] != 0) { printf("FAIL neighbours\n"); return 5; }
    if (g_foregroundCalls != 4) { printf("FAIL call count %d\n", g_foregroundCalls); return 6; }
    printf("FABLETLC_SYSMGR_ISAPPACTIVE PASS\n");
    return 0;
}