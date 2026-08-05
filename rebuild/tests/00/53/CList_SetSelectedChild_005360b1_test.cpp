#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CList_SetSelectedChild_005360b1(void*);
extern "C" const float FableCListSelectedChildScaleA = 1.0f;
extern "C" const float FableCListSelectedChildScaleB = 0.5f;

static int g_callbackCount;

extern "C" __declspec(naked) void
CallSetSelectedChild(void*, long)
{
    __asm {
        mov ecx, dword ptr [esp+4]
        push dword ptr [esp+8]
        call CList_SetSelectedChild_005360b1
        ret 8
    }
}

extern "C" __declspec(naked) void __fastcall CListOffsetCallback(void*)
{
    __asm {
        inc dword ptr g_callbackCount
        ret 0ch
    }
}

int main()
{
    unsigned char object[0x220];
    unsigned char states[4] = {1, 0, 0, 0};
    void* vtable[0x90 / 4];
    memset(object, 0, sizeof(object));
    memset(vtable, 0, sizeof(vtable));
    vtable[0x84 / 4] = (void*)CListOffsetCallback;
    *(void**)object = vtable;
    *(unsigned char**)(object + 0x1c8) = states;
    *(unsigned char**)(object + 0x1cc) = states + sizeof(states);

    CallSetSelectedChild(object, 1);

    if (*(long*)(object + 0x15c) != 1)
        return 1;
    if (g_callbackCount != 1)
        return 2;

    puts("CList_SetSelectedChild_005360b1_test PASS");
    return 0;
}
