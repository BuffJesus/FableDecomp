#include <stdio.h>

extern "C" unsigned char FableInit2FrontendFlag_01375448;
extern "C" void* FableInit2DisplayManager_013b8390;
extern "C" void __fastcall CNewFrontendGameComponent_Init2_0042e98f(void*);

struct FakeVtable { void* slots[2]; };

static unsigned char g_component[0x180];
static unsigned char g_manager[0x200];
static unsigned char g_stream[0x240];
static FakeVtable g_streamVtable;
static int g_getManager, g_graphics, g_sounds, g_input, g_thingManager;
static int g_new, g_streamCtor, g_reset, g_font, g_fontVtable;
static int g_cons, g_swap, g_preMain, g_bankPointer, g_bankPointer2;
static int g_managerInit, g_managerInit2, g_display;

extern "C" unsigned char FableInit2FrontendFlag_01375448 = 0;
extern "C" void* FableInit2DisplayManager_013b8390 = (void*)0x12340000;

extern "C" __declspec(naked) void FableInit2GetManager_00595582()
{
    __asm
    {
        inc g_getManager
        mov eax, offset g_manager
        ret
    }
}
extern "C" __declspec(naked) void FableInit2InitialiseGraphics_0042ddb3()
{
    __asm
    {
        inc g_graphics
        ret
    }
}
extern "C" __declspec(naked) void FableInit2InitFrontendSounds_0042e17c()
{
    __asm
    {
        inc g_sounds
        ret
    }
}
extern "C" __declspec(naked) void FableInit2InitialiseInput_0042dec5()
{
    __asm
    {
        inc g_input
        ret
    }
}
extern "C" __declspec(naked) void FableInit2SetCurrentThingManager_00aeaa80()
{
    __asm
    {
        inc g_thingManager
        ret
    }
}
extern "C" __declspec(naked) void* FableInit2OperatorNew_00bfea1a()
{
    __asm
    {
        inc g_new
        mov eax, offset g_stream
        ret
    }
}
extern "C" __declspec(naked) void FableInit2CompressedDataCtor_009c85a0()
{
    __asm
    {
        inc g_streamCtor
        ret
    }
}
extern "C" __declspec(naked) void FableInit2CountedPointerReset_00403e40()
{
    __asm
    {
        inc g_reset
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_component + 060h], eax
        ret 4
    }
}
extern "C" __declspec(naked) void* FableInit2GetFontBankName_00415260()
{
    __asm
    {
        inc g_font
        mov eax, offset g_stream
        ret 4
    }
}
extern "C" __declspec(naked) void FableInit2FontOpen()
{
    __asm
    {
        inc g_fontVtable
        ret 4
    }
}
extern "C" __declspec(naked) void FableInit2StringCons_0099eae0()
{
    __asm
    {
        inc g_cons
        ret
    }
}
extern "C" __declspec(naked) void FableInit2StringPairSwap_0042fb40()
{
    __asm
    {
        inc g_swap
        ret
    }
}
extern "C" __declspec(naked) void* FableInit2GetPreMainMemoryUsed_0043368d()
{
    __asm
    {
        inc g_preMain
        xor eax, eax
        ret
    }
}
extern "C" __declspec(naked) void FableInit2BankPointerCtor_00428905()
{
    __asm
    {
        inc g_bankPointer
        ret 4
    }
}
extern "C" __declspec(naked) void FableInit2BankPointerCtor_0041ba30()
{
    __asm
    {
        inc g_bankPointer2
        ret 4
    }
}
extern "C" __declspec(naked) void FableInit2ManagerInit_005958f5()
{
    __asm
    {
        inc g_managerInit
        ret
    }
}
extern "C" __declspec(naked) void FableInit2ManagerInit2_00598a1c()
{
    __asm
    {
        inc g_managerInit2
        ret 4
    }
}
extern "C" __declspec(naked) void FableInit2SetDisplayMode_009bff40()
{
    __asm
    {
        inc g_display
        ret 10h
    }
}

int main()
{
    g_streamVtable.slots[1] = (void*)&FableInit2FontOpen;
    *(void**)(g_stream) = (void*)&g_streamVtable;
    *(unsigned char*)(g_component + 0x144) = 0;
    *(void**)(g_component + 0x40) = (void*)0x56780000;
    *(void**)(g_component + 0x48) = (void*)0x56780010;
    *(void**)(g_component + 0x68) = (void*)0x56780020;
    CNewFrontendGameComponent_Init2_0042e98f(g_component);
    if (g_getManager != 1 || *(void**)(g_component + 0xb4) != (void*)g_manager ||
        *(void**)(g_manager + 0x1c) != (void*)g_component ||
        *(unsigned char*)(g_manager + 0xc0) != 1 || g_graphics != 1 ||
        g_sounds != 1 || g_input != 1 || g_thingManager != 1 || g_new != 1 ||
        g_streamCtor != 1 || g_reset != 1 || g_font != 1 || g_fontVtable != 1 ||
        g_cons != 1 || g_swap != 1 || g_preMain != 1 || g_bankPointer != 1 ||
        g_bankPointer2 != 1 || g_managerInit != 1 || g_managerInit2 != 1 ||
        g_display != 1)
    {
        puts("INIT2_0042E98F_TEST FAIL");
        return 1;
    }
    puts("INIT2_0042E98F_TEST PASS");
    return 0;
}
