/* gamepad_redefine_hook.c — CODE half of the gamepad-redefine patch (self-contained,
 * no MinHook). Injected by FSE_Launcher (place in Mods\, list in Mods.ini). Pairs with
 * the DATA half (tools/build_gamepad_redefine_data.py). Design: docs/GAMEPAD_REDEFINE_PATCH.md.
 *
 * Approach v3 (targeted, non-invasive — v2's raw vector-swap blacked the screen even though
 * the passive/controller vector holds 123 records):
 *   1. Action @0x59A238 — on action 284 resolve+cache the gamepad screen ptr, GotoNextScreen.
 *   2. GotoNextScreen @0x596763 — g_useController = (screen == gamepad screen). No swap.
 *   3. GetAssignedInputForAction @0x408C90 — this accessor already branches keyboard(+0x54,
 *      usePassive!=0) vs controller(+0x60, usePassive==0). While the gamepad screen is showing,
 *      FORCE its usePassive arg to 0 so callers read the controller vector. This touches nothing
 *      but the per-call arg, so it can't corrupt the vectors or other screens. It also records
 *      each caller's return address into a small buffer (flushed to %TEMP%\gamepad_hook.log when
 *      leaving the screen) so if the redefine rows DON'T come through 0x408C90 we see who to
 *      target instead.
 *
 * Revert = remove the Mods.ini line (or restore gamepad_redefine.dll.singlehook.bak).
 */
#include <windows.h>

#define BASE_DEFAULT 0x400000u
static unsigned int g_delta = 0;
#define A(va) ((unsigned int)(va) + g_delta)

static unsigned int a_GotoNext, a_WCtor, a_Lookup, a_Resolve, a_WDtor;
static unsigned char* g_tramp_Action;
static unsigned char* g_tramp_Goto;
static unsigned char* g_tramp_GetInput;
static void* g_gamepadScreen = 0;
static int   g_useController = 0;
static const char g_scr[] = "UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD";

/* caller-address ring buffer for the GetAssignedInputForAction diagnostic */
#define CAP 64
static unsigned int g_ret[CAP];
static unsigned int g_act[CAP];
static unsigned int g_pass[CAP];
static volatile int g_n = 0;

static void flush_log(void)
{
    char path[MAX_PATH]; DWORD n; char line[128]; HANDLE h; int i, cnt = g_n;
    n = GetTempPathA(MAX_PATH, path);
    if (n == 0 || n > MAX_PATH - 20) return;
    lstrcatA(path, "gamepad_hook.log");
    h = CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
                    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (h == INVALID_HANDLE_VALUE) return;
    SetFilePointer(h, 0, 0, FILE_END);
    n = (DWORD)wsprintfA(line, "FLUSH getinput_calls=%d gp=%08X\r\n", cnt, (unsigned)g_gamepadScreen);
    WriteFile(h, line, n, &n, 0);
    if (cnt > CAP) cnt = CAP;
    for (i = 0; i < cnt; i++) {
        n = (DWORD)wsprintfA(line, "  call ret=%08X action=%u usePassive=%u\r\n",
                             g_ret[i], g_act[i], g_pass[i]);
        WriteFile(h, line, n, &n, 0);
    }
    CloseHandle(h);
    g_n = 0;
}

/* called from hk_GetInput (regs saved): record the caller, return forced-usePassive (0/keep) */
static int __cdecl on_getinput(unsigned int retaddr, unsigned int action, unsigned int usePassive)
{
    if (g_useController) {
        int i = g_n;
        if (i < CAP) { g_ret[i] = retaddr; g_act[i] = action; g_pass[i] = usePassive; }
        g_n = i + 1;
        return 0;                 /* force controller/passive vector */
    }
    return (int)usePassive;        /* unchanged */
}

/* called from hk_Goto: manage the flag; flush the diagnostic when leaving the screen */
static void __cdecl on_goto(void* screen)
{
    int now = (screen == g_gamepadScreen && g_gamepadScreen != 0) ? 1 : 0;
    if (!now && g_useController) { g_useController = 0; flush_log(); }
    else g_useController = now;
}

/* ---- Action detour: action 284 -> resolve+cache gamepad screen + GotoNextScreen ---- */
__declspec(naked) static void hk_Action(void)
{
    __asm {
        mov  eax, [esp+4]
        mov  eax, [eax]
        mov  eax, [eax]
        cmp  eax, 0x0000011C
        jne  run_original
        push ebp
        mov  ebp, esp
        sub  esp, 0x40
        push esi
        push ebx
        mov  esi, ecx
        push 0xFFFFFFFF
        lea  eax, g_scr
        push eax
        lea  ecx, [ebp-0x20]
        mov  eax, a_WCtor
        call eax
        push 0
        lea  eax, [ebp-0x20]
        push eax
        mov  eax, a_Lookup
        call eax
        mov  ecx, eax
        mov  eax, a_Resolve
        call eax
        mov  ebx, eax
        mov  g_gamepadScreen, eax
        lea  ecx, [ebp-0x20]
        mov  eax, a_WDtor
        call eax
        push 0
        push ebx
        mov  ecx, esi
        mov  eax, a_GotoNext
        call eax
        pop  ebx
        pop  esi
        mov  esp, ebp
        pop  ebp
        ret  4
    run_original:
        jmp  dword ptr [g_tramp_Action]
    }
}

/* ---- GotoNextScreen detour: toggle g_useController based on the target screen ---- */
__declspec(naked) static void hk_Goto(void)
{
    __asm {
        pushad
        pushfd
        mov  eax, [esp+0x28]           /* screen arg */
        push eax
        call on_goto
        add  esp, 4
        popfd
        popad
        jmp  dword ptr [g_tramp_Goto]
    }
}

/* ---- GetAssignedInputForAction detour: force usePassive=0 on the gamepad screen ----
 * On entry: [esp]=retaddr, [esp+4]=action (arg1), [esp+8]=usePassive (arg2), ecx=this. */
__declspec(naked) static void hk_GetInput(void)
{
    __asm {
        pushad
        pushfd
        mov  eax, [esp+0x2c]           /* usePassive (arg2): pushad32+pushfd4+ret4+arg1_4 = +0x2c */
        push eax
        mov  eax, [esp+0x2c]           /* action (arg1): shifted by the one push above -> +0x2c */
        push eax
        mov  eax, [esp+0x2c]           /* retaddr: shifted by two pushes -> +0x2c */
        push eax
        call on_getinput              /* __cdecl(retaddr, action, usePassive) -> forced usePassive */
        add  esp, 0x0c
        mov  [esp+0x20], eax          /* stash forced usePassive into saved-EAX slot (popad restores) */
        popfd
        popad
        mov  [esp+8], eax             /* overwrite the real arg2 with the forced value */
        jmp  dword ptr [g_tramp_GetInput]
    }
}

/* ---- inline hook installer (trampoline) -------------------------------- */
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

static void install_all(void)
{
    g_delta    = (unsigned int)GetModuleHandleW(NULL) - BASE_DEFAULT;
    a_GotoNext = A(0x00596763);
    a_WCtor    = A(0x0099EBF0);
    a_Lookup   = A(0x0041E5F2);
    a_Resolve  = A(0x0041DB1D);
    a_WDtor    = A(0x0099EAE0);
    /* Action @0x59A238 prologue = 55 8bec 83ec14 (6 bytes) */
    g_tramp_Action   = install_tramp(0x0059A238, hk_Action, 6);
    /* GotoNextScreen @0x596763 prologue = 55 8bec 51 53 56 (6 bytes) */
    g_tramp_Goto     = install_tramp(0x00596763, hk_Goto, 6);
    /* GetAssignedInputForAction @0x408C90 prologue = 8a442408 84c0 (6 bytes) */
    g_tramp_GetInput = install_tramp(0x00408C90, hk_GetInput, 6);
}

BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID r)
{
    (void)h; (void)r;
    if (reason == DLL_PROCESS_ATTACH) install_all();
    return TRUE;
}
