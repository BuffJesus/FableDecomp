#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CFrontEndManager_GotoProfileMenu_00597b20(void*);

static int g_refreshCalls;
static int g_countCalls;
static int g_textCalls;
static int g_gotoCalls;
static void* g_gotoThis;
static unsigned long g_gotoDefinition;
static long g_gotoMode;
static unsigned long g_textObject[1] = {0x12345678UL};

extern "C" __declspec(naked) int __fastcall FableProfileMenuRefresh(void*)
{
    __asm {
        inc dword ptr g_refreshCalls
        xor eax, eax
        ret
    }
}

extern "C" __declspec(naked) int __fastcall FableProfileMenuCount(void*, void*)
{
    __asm {
        inc dword ptr g_countCalls
        xor eax, eax
        ret 4
    }
}

extern "C" __declspec(naked) void* __fastcall FableProfileMenuGetText(void*, void*)
{
    __asm {
        inc dword ptr g_textCalls
        mov eax, offset g_textObject
        ret 4
    }
}

extern "C" __declspec(naked) void* __fastcall FableProfileMenuGetChild(void*)
{
    __asm {
        xor eax, eax
        ret
    }
}

extern "C" __declspec(naked) void* __fastcall FableProfileMenuGetInstance()
{
    __asm {
        xor eax, eax
        ret
    }
}

extern "C" __declspec(naked) void __fastcall FableProfileMenuWriteState(void*)
{
    __asm ret
}

extern "C" __declspec(naked) void __fastcall FableProfileMenuAttach(void*, void*)
{
    __asm ret 4
}

extern "C" __declspec(naked) void __fastcall FableProfileMenuGotoNext(void*, void*, long)
{
    __asm {
        inc dword ptr g_gotoCalls
        mov dword ptr g_gotoThis, ecx
        mov eax, dword ptr [esp+4]
        mov dword ptr g_gotoDefinition, eax
        mov eax, dword ptr [esp+8]
        mov dword ptr g_gotoMode, eax
        ret 8
    }
}

extern "C" __declspec(naked) void CallGotoProfileMenu(void*)
{
    __asm {
        mov ecx, dword ptr [esp+4]
        call CFrontEndManager_GotoProfileMenu_00597b20
        ret
    }
}

int main()
{
    unsigned char manager[0x200];
    memset(manager, 0, sizeof(manager));

    CallGotoProfileMenu(manager);

    if (g_refreshCalls != 1 || g_countCalls != 1 || g_textCalls != 1)
        return 1;
    if (g_gotoCalls != 1 || g_gotoThis != manager)
        return 2;
    if (g_gotoDefinition != g_textObject[0] || g_gotoMode != 0)
        return 3;

    puts("CFrontEndManager_GotoProfileMenu_00597b20_test PASS");
    return 0;
}
