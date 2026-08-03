#include <stdio.h>

__declspec(naked) void SetFriendsWithEverythingFlag(bool /*flag*/)
{
    __asm
    {
        mov ecx, dword ptr [ecx+0x4]
        test ecx, ecx
        je   done
        mov  eax, dword ptr [ecx]
        jmp  dword ptr [eax+0x10c]
    done:
        ret 0x4
    }
}

static int   g_called = 0;
static bool  g_flag   = false;
static void* g_this   = 0;

void __fastcall Handler(void* self, void* /*edx*/, bool flag)
{
    g_called++;
    g_flag = flag;
    g_this = self;
}

static void* g_vtbl[68];

struct Delegate { void* vtbl; };
struct ScriptThing { void* pad0; Delegate* d; };

int main()
{
    for (int i = 0; i < 68; ++i) g_vtbl[i] = 0;
    g_vtbl[67] = (void*)&Handler;

    Delegate del;
    del.vtbl = g_vtbl;

    ScriptThing st;
    st.pad0 = 0;
    st.d = &del;

    void* pthis = &st;
    __asm {
        mov  ecx, pthis
        push 1
        call SetFriendsWithEverythingFlag
    }
    if (!(g_called == 1 && g_flag == true && g_this == (void*)&del)) {
        printf("FAIL case1 called=%d flag=%d this=%p exp=%p\n",
               g_called, (int)g_flag, g_this, (void*)&del);
        return 1;
    }

    ScriptThing st2;
    st2.pad0 = (void*)0xdeadbeef;
    st2.d = 0;
    void* pthis2 = &st2;
    __asm {
        mov  ecx, pthis2
        push 0
        call SetFriendsWithEverythingFlag
    }
    if (g_called != 1) {
        printf("FAIL case2 handler ran on null delegate called=%d\n", g_called);
        return 1;
    }

    printf("OK_0x004ab060 called=%d flag=%d\n", g_called, (int)g_flag);
    return 0;
}