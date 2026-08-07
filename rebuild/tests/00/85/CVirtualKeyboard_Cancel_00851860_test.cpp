#include <cstdio>

struct VirtualKeyboard
{
    unsigned char bytes[8];
};

static unsigned char g_ime[8];
static unsigned char g_imeActive;
static unsigned int g_addChildCalls;
static unsigned int g_finaliseCalls;
static unsigned int g_finaliseValue;

extern "C" void __fastcall
CVirtualKeyboard_Cancel_00851860(void*, void*);

extern "C" unsigned long __fastcall
FableVirtualKeyboard_AddChildPrimitive_00851860(void*, void*)
{
    ++g_addChildCalls;
    return (unsigned long)g_ime;
}

extern "C" unsigned char __fastcall
FableVirtualKeyboard_IsIMEActive_00851860(void*, void*)
{
    return g_imeActive;
}

extern "C" void __fastcall
FableVirtualKeyboard_FinaliseString_00851860(void*, void*, bool value)
{
    ++g_finaliseCalls;
    g_finaliseValue = value ? 1u : 0u;
}

static int Check(unsigned char active, unsigned int adds, unsigned int finalises,
                 unsigned int finaliseValue, const VirtualKeyboard& keyboard)
{
    return keyboard.bytes[4] == 1 && keyboard.bytes[5] == 1 &&
        g_addChildCalls == adds && g_finaliseCalls == finalises &&
        g_finaliseValue == finaliseValue && g_imeActive == active;
}

int main()
{
    VirtualKeyboard keyboard = {{0, 0, 0, 0, 0, 0, 0, 0}};
    g_imeActive = 0;
    g_addChildCalls = 0;
    g_finaliseCalls = 0;
    g_finaliseValue = 99;
    CVirtualKeyboard_Cancel_00851860(&keyboard, 0);
    if (!Check(0, 1, 0, 99, keyboard))
        return 1;

    keyboard.bytes[4] = 0;
    keyboard.bytes[5] = 0;
    g_imeActive = 1;
    g_addChildCalls = 0;
    g_finaliseCalls = 0;
    g_finaliseValue = 99;
    CVirtualKeyboard_Cancel_00851860(&keyboard, 0);
    if (!Check(1, 2, 1, 0, keyboard))
        return 2;

    std::printf("CVIRTUALKEYBOARD_CANCEL_00851860_TEST PASS\n");
    return 0;
}
