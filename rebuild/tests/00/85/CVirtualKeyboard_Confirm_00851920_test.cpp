#include <cstdio>

struct VirtualKeyboard
{
    unsigned char bytes[8];
};

struct ProfileName
{
    void* storage;
};

static ProfileName g_profileName;
static long g_profileLength;
static unsigned char g_ime[8];
static unsigned char g_imeActive;
static unsigned int g_addChildCalls;
static unsigned int g_destroyCalls;
static unsigned int g_finaliseCalls;
static unsigned int g_finaliseValue;

extern "C" void __fastcall
CVirtualKeyboard_Confirm_00851920(void*, void*);

extern "C" void* __fastcall
FableVirtualKeyboard_GetProfileName_00851920(void*, void*, void* output)
{
    *(ProfileName*)output = g_profileName;
    return output;
}

extern "C" long __fastcall
FableVirtualKeyboard_GetProfileNameLength_00851920(void*, void*)
{
    return g_profileLength;
}

extern "C" void __fastcall
FableVirtualKeyboard_DestroyProfileName_00851920(void*, void*)
{
    ++g_destroyCalls;
}

extern "C" unsigned long __fastcall
FableVirtualKeyboard_AddChildPrimitive_00851920(void*, void*)
{
    ++g_addChildCalls;
    return (unsigned long)g_ime;
}

extern "C" unsigned char __fastcall
FableVirtualKeyboard_IsIMEActive_00851920(void*, void*)
{
    return g_imeActive;
}

extern "C" void __fastcall
FableVirtualKeyboard_FinaliseString_00851920(void*, void*, bool value)
{
    ++g_finaliseCalls;
    g_finaliseValue = value ? 1u : 0u;
}

int main()
{
    VirtualKeyboard keyboard = {{0, 0, 0, 0, 0, 0, 0, 0}};
    g_profileName.storage = 0;
    g_profileLength = 1;
    g_imeActive = 1;
    g_addChildCalls = 0;
    g_destroyCalls = 0;
    g_finaliseCalls = 0;
    g_finaliseValue = 99;
    CVirtualKeyboard_Confirm_00851920(&keyboard, 0);
    if (keyboard.bytes[4] != 0 || keyboard.bytes[5] != 1 ||
        g_addChildCalls != 2 || g_destroyCalls != 1 ||
        g_finaliseCalls != 1 || g_finaliseValue != 1)
    {
        return 1;
    }

    keyboard.bytes[4] = 7;
    keyboard.bytes[5] = 1;
    g_addChildCalls = 0;
    g_destroyCalls = 0;
    g_finaliseCalls = 0;
    CVirtualKeyboard_Confirm_00851920(&keyboard, 0);
    if (keyboard.bytes[4] != 7 || keyboard.bytes[5] != 1 ||
        g_addChildCalls != 0 || g_destroyCalls != 0 ||
        g_finaliseCalls != 0)
        return 2;

    std::printf("CVIRTUALKEYBOARD_CONFIRM_00851920_TEST PASS\n");
    return 0;
}
