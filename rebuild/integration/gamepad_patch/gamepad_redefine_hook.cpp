// gamepad_redefine_hook.cpp — CODE half of the gamepad-redefine patch.
//
// Pairs with the DATA half (tools/build_gamepad_redefine_data.py: the frontend.bin
// row #344-clone Action=284 + screen-clone UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD
// + options-list #219 5th row). Design: docs/GAMEPAD_REDEFINE_PATCH.md §4b.
//
// Two hooks on retail Fable.exe (ImageBase 0x400000; VAs below assume no ASLR /
// resolve against the module base at runtime — see REBASE()):
//   1. CFrontEndManager::Action @ 0x0059A238 — when the 5th options row fires
//      action 284 (0x11C), set used-key 0x17 and continue to GotoNextScreen, so it
//      routes exactly like the 4 stock rows (retail leaves 284 as an inert default).
//   2. CFrontEndManager::Init2 @ 0x00598A1C — after it runs, replay ONE bind block
//      (the same shape Init2 uses @0x598F4D) binding key 0x17 -> the cloned gamepad
//      screen def, by name. All 5 helper fns are recovered.
//
// STATUS: NOT yet built/tested — this is the reference implementation the design
// resolves to. Needs a MinHook-class trampoline lib + a live validation pass
// (boot to Options, click the Gamepad row, confirm screen 5). Capture is already
// device-agnostic (CKeyRedefiner::Redefine 0x557D20), so no capture hook is needed;
// a modern-XInput->engine bridge (e.g. the FableControllerSupport DLL) must be
// present for controller events to reach the screen.

#include <windows.h>
#include <cstdint>
// #include "MinHook.h"   // or FSE's native-hook facility

// ---- recovered retail VAs (ImageBase 0x400000) -----------------------------
static uintptr_t g_base = 0x400000;                 // set to GetModuleHandle(0) at init
#define REBASE(va) ((va) - 0x400000 + g_base)

constexpr uintptr_t VA_Action   = 0x0059A238;       // CFrontEndManager::Action(this, ev**)
constexpr uintptr_t VA_Init2    = 0x00598A1C;       // CFrontEndManager::Init2(this)
constexpr uintptr_t VA_GotoNext = 0x00596763;       // GotoNextScreen (dispatch tail 0x59a7d2)
// Init2 bind-block helpers (see docs §4b):
constexpr uintptr_t VA_WStr_ctor = 0x0099EBF0;      // CWideString::CWideString(const char*, int len=-1)  [thiscall]
constexpr uintptr_t VA_GetSlot   = 0x0059B5D7;      // CFrontEndManager::GetOrCreateBindSlot(int* key)     [thiscall]
constexpr uintptr_t VA_FeLookup  = 0x0041E5F2;      // resolve-by-name step A (flags, &wstr)
constexpr uintptr_t VA_FeResolve = 0x0041DB1D;      // resolve-by-name step B -> screen obj                 [thiscall]
constexpr uintptr_t VA_WStr_dtor = 0x0099EAE0;      // ~CWideString                                         [thiscall]

constexpr int  ACTION_GAMEPAD_REDEFINE = 284;       // 0x11C — free; DATA half sets row #344-clone Action to this
constexpr int  KEY_GAMEPAD_REDEFINE    = 0x17;      // free used-key (0x16 = keyboard redefine)
static const char* const SCREEN_GAMEPAD = "UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD";

// ---- 1. Action detour -------------------------------------------------------
// Retail reads the action id as **(int**)eventArg (mov eax,[ebp+8]; mov eax,[eax];
// mov ecx,[eax]). The stock cases do `mov [ebp+8], usedKey; jmp 0x59a7d2` where the
// tail does GotoNextScreen(&usedKey) on this+0x54. We mirror that for action 284.
typedef void(__thiscall* Action_t)(void* self, void** eventArgArg);
static Action_t o_Action = nullptr;

// GotoNextScreen is reached in retail as: ecx=this+0x54, arg=&usedKey. Model it as
// a thiscall on the sub-object; exact prototype TBD from 0x596763 (validate live).
typedef void(__thiscall* GotoNext_t)(void* feSub54, int* usedKey);

static void __fastcall hk_Action(void* self, void* /*edx*/, void** eventArgArg)
{
    // eventArgArg points at (event*); *event = action id (first dword).
    int action = -1;
    if (eventArgArg && *eventArgArg)
        action = *reinterpret_cast<int*>(*eventArgArg);

    if (action == ACTION_GAMEPAD_REDEFINE) {
        int usedKey = KEY_GAMEPAD_REDEFINE;
        void* feSub54 = reinterpret_cast<void*>(reinterpret_cast<char*>(self) + 0x54);
        reinterpret_cast<GotoNext_t>(REBASE(VA_GotoNext))(feSub54, &usedKey);
        return;   // handled — do not fall through to the stock switch
    }
    o_Action(self, eventArgArg);   // stock behaviour for every other action
}

// ---- 2. Init2 post-hook: bind key 0x17 -> gamepad screen --------------------
// Replays one Init2 bind block (docs §4b @0x598F4D) for our (key, screenName).
typedef void*(__thiscall* WStrCtor_t)(void* wstr, const char* s, int len);
typedef void**(__thiscall* GetSlot_t)(void* fem, int* key);
typedef void*(__stdcall*  FeLookup_t)(int flags, void* wstr);   // arg order per retail push ebx;push &wstr
typedef void*(__thiscall* FeResolve_t)(void* lookupResult);
typedef void (__thiscall* WStrDtor_t)(void* wstr);

static void BindGamepadScreen(void* fem /* CFrontEndManager* this */)
{
    // CWideString wstr(SCREEN_GAMEPAD, -1);
    alignas(8) unsigned char wstr[0x20] = {};
    reinterpret_cast<WStrCtor_t>(REBASE(VA_WStr_ctor))(wstr, SCREEN_GAMEPAD, -1);

    int key = KEY_GAMEPAD_REDEFINE;
    void** slot = reinterpret_cast<GetSlot_t>(REBASE(VA_GetSlot))(fem, &key);

    void* lk  = reinterpret_cast<FeLookup_t>(REBASE(VA_FeLookup))(0, wstr);
    void* scr = reinterpret_cast<FeResolve_t>(REBASE(VA_FeResolve))(lk);
    if (slot) *slot = scr;

    reinterpret_cast<WStrDtor_t>(REBASE(VA_WStr_dtor))(wstr);
}

typedef void(__thiscall* Init2_t)(void* self);
static Init2_t o_Init2 = nullptr;

static void __fastcall hk_Init2(void* self, void* /*edx*/)
{
    o_Init2(self);            // run all stock binds first
    BindGamepadScreen(self);  // then add key 0x17 -> gamepad screen
}

// ---- install ----------------------------------------------------------------
extern "C" __declspec(dllexport) void InstallGamepadRedefineHooks()
{
    g_base = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    // MH_Initialize();
    // MH_CreateHook((void*)REBASE(VA_Action), &hk_Action, (void**)&o_Action);
    // MH_CreateHook((void*)REBASE(VA_Init2),  &hk_Init2,  (void**)&o_Init2);
    // MH_EnableHook(MH_ALL_HOOKS);
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) InstallGamepadRedefineHooks();
    return TRUE;
}
