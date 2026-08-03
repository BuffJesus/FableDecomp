#include <stdio.h>

int g_freeCalls = 0;
void* g_freeLastArg = 0;

extern "C" __declspec(naked) void __cdecl fake_free_target()
{
    __asm
    {
        push eax
        mov eax, dword ptr [esp+8]
        mov g_freeLastArg, eax
        mov eax, g_freeCalls
        add eax, 1
        mov g_freeCalls, eax
        pop eax
        ret
    }
}

extern "C" void (__cdecl* g_FableFreeThunkTarget_00440154)() = &fake_free_target;

extern "C" __declspec(naked) void __cdecl free_00bfea14(void* p)
{
    __asm
    {
        jmp dword ptr [g_FableFreeThunkTarget_00440154]
    }
}

int main()
{
    const unsigned char expected[2] = { 0xFF,0x25 };
    const unsigned char* code = reinterpret_cast<const unsigned char*>(&free_00bfea14);
    for (int i=0;i<2;++i) if(code[i]!=expected[i]) return 1;

    void* marker = (void*)0x1234;
    free_00bfea14(marker);

    if (g_freeCalls != 1 || g_freeLastArg != marker) {
        printf("FAIL calls=%d arg=%p\n", g_freeCalls, g_freeLastArg);
        return 1;
    }

    printf("FREE_THUNK_OK\n");
    return 0;
}