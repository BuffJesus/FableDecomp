#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/53/CList_InitialiseOffsets_00536b4f.cpp"

extern "C" const float FableCListInitialiseOffsetsU32Float = 4294967296.0f;
extern "C" const float FableCListInitialiseOffsetsOne = 1.0f;

static volatile float g_scale[2] = { 1.0f, 2.0f };
static volatile float g_step = 0.25f;
static volatile unsigned int g_positionBits;
static volatile unsigned int g_lastXBits;
static volatile unsigned int g_lastYBits;
static void* g_holder;

static void __declspec(naked) GetScales()
{
    __asm {
        lea eax,g_scale
        ret 4
    }
}

static void __declspec(naked) GetStep()
{
    __asm {
        fld dword ptr [g_step]
        ret
    }
}

static void __declspec(naked) SetPosition()
{
    __asm {
        mov eax,dword ptr [esp+4]
        mov dword ptr [g_positionBits],eax
        ret 4
    }
}

static void __declspec(naked) GetDef()
{
    __asm {
        mov eax,dword ptr [esp+4]
        mov dword ptr [eax],0
        mov eax,dword ptr [g_holder]
        ret 4
    }
}

static void __declspec(naked) SetVector()
{
    __asm {
        mov eax,dword ptr [esp+4]
        mov edx,dword ptr [eax]
        mov dword ptr [g_lastXBits],edx
        mov edx,dword ptr [eax+4]
        mov dword ptr [g_lastYBits],edx
        ret 4
    }
}

extern "C" __declspec(naked) void __cdecl
CallInitialiseOffsets(void*, unsigned long)
{
    __asm {
        mov ecx,dword ptr [esp+4]
        push dword ptr [esp+8]
        call CList_InitialiseOffsets_00536b4f
        ret 8
    }
}

static bool RunEvenMode()
{
    unsigned char list[0x260];
    void* vtable[0x100];
    void* holder[1];
    unsigned char def[0x230];
    std::memset(list,0,sizeof(list));
    std::memset(vtable,0,sizeof(vtable));
    std::memset(def,0,sizeof(def));
    *(void**)list=vtable;
    holder[0]=def;
    g_holder=holder;
    vtable[0x24c/4]=(void*)GetScales;
    vtable[0x250/4]=(void*)GetStep;
    vtable[0x258/4]=(void*)SetPosition;
    vtable[0x1b0/4]=(void*)GetDef;
    vtable[0x25c/4]=(void*)SetVector;
    g_positionBits=g_lastXBits=g_lastYBits=0;
    CallInitialiseOffsets(list,4);
    return g_positionBits==0x3f400000u &&
        g_lastXBits==0x3f800000u &&
        g_lastYBits==0xc0000000u;
}

static bool RunOddMode()
{
    unsigned char list[0x260];
    void* vtable[0x100];
    void* holder[1];
    unsigned char def[0x230];
    std::memset(list,0,sizeof(list));
    std::memset(vtable,0,sizeof(vtable));
    std::memset(def,0,sizeof(def));
    *(void**)list=vtable;
    holder[0]=def;
    g_holder=holder;
    vtable[0x24c/4]=(void*)GetScales;
    vtable[0x250/4]=(void*)GetStep;
    vtable[0x258/4]=(void*)SetPosition;
    vtable[0x1b0/4]=(void*)GetDef;
    vtable[0x25c/4]=(void*)SetVector;
    g_positionBits=g_lastXBits=g_lastYBits=0;
    CallInitialiseOffsets(list,5);
    return g_positionBits==0x3f000000u &&
        g_lastXBits==0x40000000u &&
        g_lastYBits==0xc0800000u;
}

int main()
{
    const bool ok=RunEvenMode()&&RunOddMode();
    std::puts(ok ? "CList_InitialiseOffsets_00536b4f_test PASS"
                 : "CList_InitialiseOffsets_00536b4f_test FAIL");
    return ok ? 0 : 1;
}
