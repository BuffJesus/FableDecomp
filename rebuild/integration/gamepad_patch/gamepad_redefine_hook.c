/* gamepad_redefine_hook.c — CODE half of the gamepad-redefine patch (self-contained,
 * no MinHook). Injected by FSE_Launcher (place in Mods\, list in Mods.ini). Pairs with
 * the DATA half (tools/build_gamepad_redefine_data.py). Design: docs/GAMEPAD_REDEFINE_PATCH.md.
 *
 * THREE hooks (all boot-safe — Init2 is NOT patched):
 *   1. CFrontEndManager::Action @0x59A238 — when the 5th options row fires action 284,
 *      resolve UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD by name, cache the resolved screen
 *      pointer in g_gamepadScreen, and GotoNextScreen to it. Retail leaves 284 inert.
 *   2. CFrontEndManager::GotoNextScreen @0x596763 — track the active screen: set
 *      g_useController = (screenArg == g_gamepadScreen). This is the single source of truth
 *      for "the gamepad redefine screen is showing"; it flips back to 0 the instant the user
 *      navigates anywhere else (every screen transition passes through here).
 *   3. CUserProfileManager::GetPrimaryInputVector @0x4088E0 — the redefine row builder
 *      (CRedefinerList::Refresh @0x557000, call site 0x557008 -> retaddr 0x55700D) reads the
 *      binding vector to display through this accessor, which retail HARDCODES to the primary
 *      (keyboard) vector +0x54. When g_useController is set AND we were called from Refresh,
 *      return the PASSIVE (controller) vector +0x60 instead — flipping BOTH the displayed
 *      values and, because CKeyRedefiner::Redefine writes back through the same vector the list
 *      is bound to, the capture side too. The retaddr guard keeps every OTHER caller of
 *      0x4088E0 on the primary vector, so nothing outside the gamepad screen is affected.
 *
 * Controller records live in the passive vector +0x60/+0x64 (28-byte records, same layout as
 * primary). If it is empty we call the engine's default-scheme loader (0x4085F0), exactly like
 * the retail accessor does for an empty primary; the default control scheme populates both
 * vectors, so +0x60 ends up holding the native Xbox-pad bindings.
 *
 * Revert = remove the Mods.ini line. Conventions from the retail disasm at 0x4088E0 / 0x557000 /
 * 0x596763 / 0x598F4D.
 */
#include <windows.h>

#define BASE_DEFAULT 0x400000u
static unsigned int g_delta = 0;
#define A(va) ((unsigned int)(va) + g_delta)

static unsigned int a_GotoNext, a_WCtor, a_Lookup, a_Resolve, a_WDtor, a_EnsureDefaults;
static unsigned int a_RefreshRet;                     /* Refresh's call-site return addr 0x55700D */
static unsigned char* g_tramp_Action;                 /* saved-prologue trampolines */
static unsigned char* g_tramp_Goto;
static void* g_gamepadScreen = 0;                     /* resolved gamepad screen def ptr */
static volatile int g_useController = 0;              /* 1 while the gamepad redefine screen shows */
static const char g_scr[] = "UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD";

/* ---- Action detour: action 284 -> resolve gamepad screen (cache ptr) + GotoNextScreen ---- */
__declspec(naked) static void hk_Action(void)
{
    __asm {
        mov  eax, [esp+4]        /* eventArg */
        mov  eax, [eax]          /* event*   */
        mov  eax, [eax]          /* action id (first dword) */
        cmp  eax, 0x0000011C     /* 284 ? */
        jne  run_original
        /* --- handle 284 (ecx = this = CFrontEndManager) --- */
        push ebp
        mov  ebp, esp
        sub  esp, 0x40           /* wstr @ ebp-0x20 */
        push esi
        push ebx
        mov  esi, ecx            /* this */
        /* CWideString wstr(g_scr, -1) */
        push 0xFFFFFFFF
        lea  eax, g_scr
        push eax
        lea  ecx, [ebp-0x20]
        mov  eax, a_WCtor
        call eax                 /* ctor cleans its 2 args (ret 8) */
        /* obj = Lookup(0, &wstr) ; screen = Resolve(obj) */
        push 0
        lea  eax, [ebp-0x20]
        push eax
        mov  eax, a_Lookup
        call eax                 /* cleans 2 args (ret 8) -> eax=obj */
        mov  ecx, eax
        mov  eax, a_Resolve
        call eax                 /* thiscall, no stack args -> eax=screen */
        mov  ebx, eax            /* save screen */
        mov  g_gamepadScreen, eax /* cache resolved gamepad screen ptr */
        /* ~CWideString(&wstr) */
        lea  ecx, [ebp-0x20]
        mov  eax, a_WDtor
        call eax
        /* GotoNextScreen(this, screen, 0) -- routes through hk_Goto, which sets g_useController */
        push 0
        push ebx
        mov  ecx, esi
        mov  eax, a_GotoNext
        call eax                 /* cleans 2 args (ret 8) */
        pop  ebx
        pop  esi
        mov  esp, ebp
        pop  ebp
        ret  4                   /* Action is thiscall w/ 1 stack arg */
    run_original:
        jmp  dword ptr [g_tramp_Action]   /* [saved prologue] + jmp Action+6 */
    }
}

/* ---- GotoNextScreen detour: track whether the active screen is the gamepad screen ---- */
__declspec(naked) static void hk_Goto(void)
{
    __asm {
        mov  eax, [esp+4]              /* screen def ptr (thiscall stack arg 1) */
        cmp  eax, g_gamepadScreen
        jne  not_gamepad
        mov  byte ptr g_useController, 1
        jmp  go_orig
    not_gamepad:
        mov  byte ptr g_useController, 0
    go_orig:
        jmp  dword ptr [g_tramp_Goto]  /* [saved 6-byte prologue] + jmp GotoNextScreen+6 */
    }
}

/* ---- GetPrimaryInputVector full detour: primary +0x54, or controller +0x60 on the gamepad screen.
 * Faithful reimplementation of retail 0x4088E0 (ecx=this profile mgr; empty vector -> load
 * defaults via 0x4085F0; returns &vector). The controller path is gated on BOTH g_useController
 * and the Refresh call site, so no other consumer of 0x4088E0 is redirected. ---- */
__declspec(naked) static void hk_GetPrimaryInputVector(void)
{
    __asm {
        /* controller path only when on the gamepad screen AND called from Refresh (0x55700D) */
        mov  eax, [esp]                 /* return address of the caller */
        cmp  eax, a_RefreshRet
        jne  primary
        cmp  byte ptr g_useController, 0
        je   primary
        /* --- controller / passive vector +0x60 --- */
        mov  eax, [ecx+0x60]
        cmp  eax, [ecx+0x64]
        jne  ret60
        push ecx                        /* preserve this across EnsureDefaults */
        mov  eax, a_EnsureDefaults
        call eax                        /* 0x4085F0 thiscall(ecx=this), no stack args */
        pop  ecx
    ret60:
        lea  eax, [ecx+0x60]
        ret
    primary:
        /* --- retail default: primary vector +0x54 --- */
        mov  eax, [ecx+0x54]
        cmp  eax, [ecx+0x58]
        jne  ret54
        push ecx
        mov  eax, a_EnsureDefaults
        call eax
        pop  ecx
    ret54:
        lea  eax, [ecx+0x54]
        ret
    }
}

/* ---- inline hook installers -------------------------------------------- */
static unsigned char* install_tramp(unsigned int target_va, void* detour, int savelen)
{
    unsigned char* t = (unsigned char*)A(target_va);
    unsigned char* tr = (unsigned char*)VirtualAlloc(0, 32, MEM_COMMIT | MEM_RESERVE,
                                                     PAGE_EXECUTE_READWRITE);
    DWORD old; int i;
    memcpy(tr, t, savelen);
    tr[savelen] = 0xE9;
    *(int*)(tr + savelen + 1) = (int)(t + savelen) - (int)(tr + savelen + 5);
    VirtualProtect(t, savelen, PAGE_EXECUTE_READWRITE, &old);
    t[0] = 0xE9;
    *(int*)(t + 1) = (int)detour - (int)(t + 5);
    for (i = 5; i < savelen; i++) t[i] = 0x90;
    VirtualProtect(t, savelen, old, &old);
    FlushInstructionCache(GetCurrentProcess(), t, savelen);
    return tr;
}

/* full-replacement detour (no trampoline; the detour reimplements the target and RETs) */
static void install_replace(unsigned int target_va, void* detour)
{
    unsigned char* t = (unsigned char*)A(target_va);
    DWORD old;
    VirtualProtect(t, 5, PAGE_EXECUTE_READWRITE, &old);
    t[0] = 0xE9;
    *(int*)(t + 1) = (int)detour - (int)(t + 5);
    VirtualProtect(t, 5, old, &old);
    FlushInstructionCache(GetCurrentProcess(), t, 5);
}

static void install_all(void)
{
    g_delta          = (unsigned int)GetModuleHandleW(NULL) - BASE_DEFAULT;
    a_GotoNext       = A(0x00596763);
    a_WCtor          = A(0x0099EBF0);
    a_Lookup         = A(0x0041E5F2);
    a_Resolve        = A(0x0041DB1D);
    a_WDtor          = A(0x0099EAE0);
    a_EnsureDefaults = A(0x004085F0);
    a_RefreshRet     = A(0x0055700D);          /* CRedefinerList::Refresh call site + 5 */
    /* Action @0x59A238 prologue = 55 8bec 83ec14 (6 bytes) */
    g_tramp_Action = install_tramp(0x0059A238, hk_Action, 6);
    /* GotoNextScreen @0x596763 prologue = 55 8bec 51 53 56 (6 bytes) */
    g_tramp_Goto   = install_tramp(0x00596763, hk_Goto, 6);
    /* GetPrimaryInputVector @0x4088E0 — full replacement (5-byte jmp) */
    install_replace(0x004088E0, hk_GetPrimaryInputVector);
}

BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID r)
{
    (void)h; (void)r;
    if (reason == DLL_PROCESS_ATTACH) install_all();
    return TRUE;
}
