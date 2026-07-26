#include <cstdio>

// Behavior recorders
static int g_theme_in   = 0;   // theme passed to GetThemeList
static int g_list       = 0;   // list handle returned
static int g_count      = 0;   // count to return from GetInternalCount
static int g_getcount_ecx = 0; // ecx seen by GetInternalCount
static int g_calls      = 0;   // number of SetWeight calls
static int g_last_i     = 0;   // last i arg
static int g_last_1     = 0;   // the constant-1 arg
static int g_last_count = 0;   // the count arg
static int g_last_theme = 0;   // the theme arg
static int g_last_ecx   = 0;   // ecx (internal) seen by SetWeight

// __stdcall(int): pushes cleaned by callee (ret 4)
extern "C" __declspec(naked) void GetThemeListForTheme()
{
    __asm {
        mov  eax, [esp+4]
        mov  g_theme_in, eax
        mov  eax, g_list
        ret  4
    }
}

// thiscall-ish: ecx = list, no stack args (ret 0)
extern "C" __declspec(naked) void ThemeList_GetInternalCount()
{
    __asm {
        mov  g_getcount_ecx, ecx
        mov  eax, g_count
        ret
    }
}

// thiscall: ecx = internal, 4 stack args, callee cleans 16 (ret 16)
extern "C" __declspec(naked) void ThemeInternal_SetWeight()
{
    __asm {
        mov  g_last_ecx, ecx
        mov  eax, [esp+4]
        mov  g_last_i, eax
        mov  eax, [esp+8]
        mov  g_last_1, eax
        mov  eax, [esp+0x0c]
        mov  g_last_count, eax
        mov  eax, [esp+0x10]
        mov  g_last_theme, eax
        inc  g_calls
        ret  16
    }
}

__declspec(naked) void CGameScriptInterface_SetEnvironmentThemeWeightAllInternals()
{
    __asm {
        mov  eax, [esp+4]
        push ebp
        push edi
        push eax
        mov  ebp, ecx
        call GetThemeListForTheme
        mov  ecx, eax
        call ThemeList_GetInternalCount
        mov  edi, eax
        test edi, edi
        jle  done
        push ebx
        mov  ebx, [esp+0x14]
        push esi
        mov  esi, 1
    lp:
        mov  ecx, [ebp+4]
        mov  ecx, [ecx+0x18]
        push ebx
        push edi
        push 1
        push esi
        call ThemeInternal_SetWeight
        inc  esi
        cmp  esi, 4
        jle  lp
        pop  esi
        pop  ebx
    done:
        pop  edi
        pop  ebp
        ret  8
    }
}

// this-layout: +4 = mid pointer; mid+0x18 = internal object pointer
struct Inner { unsigned char pad[0x18]; void* internal; };
struct FakeThis { void* vt; Inner* mid; };

static void call_target(FakeThis* self, int theme, int arg2)
{
    void* fn = (void*)&CGameScriptInterface_SetEnvironmentThemeWeightAllInternals;
    __asm {
        push arg2
        push theme
        mov  ecx, self
        call fn
    }
}

int main()
{
    Inner inner;
    inner.internal = (void*)0xDEAD;
    FakeThis self;
    self.mid = &inner;

    // Positive count -> 4 iterations, args threaded correctly.
    g_list = 0x1234; g_count = 3;
    g_calls = 0;
    call_target(&self, 42, 99);
    if (g_theme_in != 42)   { std::printf("FAIL theme_in=%d\n", g_theme_in); return 1; }
    if (g_getcount_ecx != 0x1234) { std::printf("FAIL gc_ecx=%d\n", g_getcount_ecx); return 1; }
    if (g_calls != 4)       { std::printf("FAIL calls=%d\n", g_calls); return 1; }
    if (g_last_i != 4)      { std::printf("FAIL last_i=%d\n", g_last_i); return 1; }
    if (g_last_1 != 1)      { std::printf("FAIL last_1=%d\n", g_last_1); return 1; }
    if (g_last_count != 3)  { std::printf("FAIL last_count=%d\n", g_last_count); return 1; }
    // 4th arg to SetWeight comes from the 2nd parameter (arg2), reloaded from
    // the stack home at [esp+0x14] inside the loop.
    if (g_last_theme != 99) { std::printf("FAIL last_arg2=%d\n", g_last_theme); return 1; }
    if (g_last_ecx != (int)0xDEAD) { std::printf("FAIL last_ecx=%x\n", g_last_ecx); return 1; }

    // Zero count -> no iterations.
    g_count = 0; g_calls = 0;
    call_target(&self, 7, 0);
    if (g_calls != 0)       { std::printf("FAIL zero calls=%d\n", g_calls); return 1; }

    // Negative count -> no iterations.
    g_count = -5; g_calls = 0;
    call_target(&self, 5, 0);
    if (g_calls != 0)       { std::printf("FAIL neg calls=%d\n", g_calls); return 1; }

    std::printf("CGameScriptInterface_00890040_TEST PASS\n");
    return 0;
}