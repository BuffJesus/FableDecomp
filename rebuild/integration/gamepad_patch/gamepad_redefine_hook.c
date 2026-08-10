/* gamepad_redefine_hook.c — CODE half of the gamepad-redefine patch (self-contained,
 * no MinHook). Injected by FSE_Launcher (place in Mods\, list in Mods.ini). Pairs with
 * the DATA half (tools/build_gamepad_redefine_data.py). Design: docs/GAMEPAD_REDEFINE_PATCH.md.
 *
 * SINGLE hook (boot-safe — Init2 is NOT patched):
 *   CFrontEndManager::Action @0x59A238 — when the 5th options row fires action 284,
 *   resolve UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD by name and GotoNextScreen to it,
 *   exactly the shape retail's dispatch uses (which resolves the same object into the
 *   key-slot). Retail leaves 284 as an inert default; every other action runs unchanged.
 * Capture needs no hook (CKeyRedefiner::Redefine is device-agnostic). Revert = remove the
 * Mods.ini line. All conventions taken from the retail disasm at 0x598F4D / 0x59a762.
 */
#include <windows.h>

#define BASE_DEFAULT 0x400000u
static unsigned int g_delta = 0;
#define A(va) ((unsigned int)(va) + g_delta)

static unsigned int a_GotoNext, a_WCtor, a_Lookup, a_Resolve, a_WDtor;
static unsigned char* g_tramp_Action;                 /* saved-prologue trampoline */
static const char g_scr[] = "UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD";

/* ---- Action detour: action 284 -> resolve gamepad screen + GotoNextScreen ---- */
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
        /* ~CWideString(&wstr) */
        lea  ecx, [ebp-0x20]
        mov  eax, a_WDtor
        call eax
        /* GotoNextScreen(this, screen, 0) */
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

/* ---- inline hook installer ---------------------------------------------- */
static unsigned char* install(unsigned int target_va, void* detour, int savelen)
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

static void install_all(void)
{
    g_delta    = (unsigned int)GetModuleHandleW(NULL) - BASE_DEFAULT;
    a_GotoNext = A(0x00596763);
    a_WCtor    = A(0x0099EBF0);
    a_Lookup   = A(0x0041E5F2);
    a_Resolve  = A(0x0041DB1D);
    a_WDtor    = A(0x0099EAE0);
    /* Action @0x59A238 prologue = 55 8bec 83ec14 (6 bytes) */
    g_tramp_Action = install(0x0059A238, hk_Action, 6);
}

BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID r)
{
    (void)h; (void)r;
    if (reason == DLL_PROCESS_ATTACH) install_all();
    return TRUE;
}
