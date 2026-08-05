#include <stdio.h>

extern "C" const char FableInitialiseEngineEnvironmentName_01230c84[];
extern "C" unsigned char FableInitialiseEngineProfileFlag_01375459;
extern "C" unsigned char FableInitialiseEngineCacheFlag_013b8642;
extern "C" unsigned long FableInitialiseEngineDefaults_0137549c[10];
extern "C" void __fastcall CNewFrontendGameComponent_InitialiseEngine_0042e204(void*);

struct FakeVtable { void* slots[80]; };
static unsigned char g_component[0x100];
static unsigned char g_primitive[0x40];
static unsigned char g_system[0x80];
static unsigned char g_systemNode[0x20];
static FakeVtable g_primitiveVtable;
static int g_primitiveCreate, g_reset, g_charCtor, g_preMain, g_getDef;
static int g_stringCons, g_engineCtor, g_systemGet, g_autoSave;
static int g_assign, g_wideDestroy, g_cacheDir, g_engineDestroy;
static int g_primitiveInitialise, g_primitiveReset;
static int g_primitiveSet;

extern "C" const char FableInitialiseEngineEnvironmentName_01230c84[] = "ENGINE";
extern "C" unsigned char FableInitialiseEngineProfileFlag_01375459 = 0;
extern "C" unsigned char FableInitialiseEngineCacheFlag_013b8642 = 0;
extern "C" unsigned long FableInitialiseEngineDefaults_0137549c[10] =
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

extern "C" __declspec(naked) void* FableInitialiseEngineCreatePrimitive_00b26340()
{
    __asm
    {
        inc g_primitiveCreate
        mov eax, offset g_primitive
        ret
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineReset_0042fd04()
{
    __asm
    {
        inc g_reset
        ret 4
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineCharStringCtor_0099ebf0()
{
    __asm
    {
        inc g_charCtor
        ret 8
    }
}
extern "C" __declspec(naked) void* FableInitialiseEngineGetPreMainMemory_0043368d()
{
    __asm
    {
        inc g_preMain
        xor eax, eax
        ret
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineGetEnvironmentDef_00430096()
{
    __asm
    {
        inc g_getDef
        ret 0ch
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineStringCons_0099eae0()
{
    __asm
    {
        inc g_stringCons
        ret
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineCtor_0042d830()
{
    __asm
    {
        inc g_engineCtor
        ret
    }
}
extern "C" __declspec(naked) void* FableInitialiseEngineGetSystemManager_009a4ec0()
{
    __asm
    {
        inc g_systemGet
        mov eax, offset g_system
        ret
    }
}
extern "C" __declspec(naked) void* FableInitialiseEngineGetAutoSaveName_0041a100()
{
    __asm
    {
        inc g_autoSave
        mov eax, offset g_systemNode
        ret
    }
}
extern "C" __declspec(naked) void* FableInitialiseEngineGetAutoSaveName_0041a1c0()
{
    __asm
    {
        inc g_autoSave
        mov eax, offset g_systemNode
        ret
    }
}
extern "C" __declspec(naked) void* FableInitialiseEngineGetAutoSaveName_0041a1e0()
{
    __asm
    {
        inc g_autoSave
        mov eax, offset g_systemNode
        ret
    }
}
extern "C" __declspec(naked) void* FableInitialiseEngineGetAutoSaveName_0041a200()
{
    __asm
    {
        inc g_autoSave
        mov eax, offset g_systemNode
        ret
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineGetCacheDir_0041a220()
{
    __asm
    {
        inc g_cacheDir
        mov eax, offset g_systemNode
        ret
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineWideAssign_0099b7d0()
{
    __asm
    {
        inc g_assign
        ret 4
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineWideDestroy_0099b510()
{
    __asm
    {
        inc g_wideDestroy
        ret
    }
}
extern "C" __declspec(naked) void FableInitialiseEngineDestructor_0042f7c1()
{
    __asm
    {
        inc g_engineDestroy
        ret
    }
}
extern "C" __declspec(naked) void FableInitialiseEnginePrimitiveInitialise()
{
    __asm
    {
        inc g_primitiveInitialise
        ret 4
    }
}
extern "C" __declspec(naked) void FableInitialiseEnginePrimitiveReset()
{
    __asm
    {
        inc g_primitiveReset
        ret 10h
    }
}
extern "C" __declspec(naked) void FableInitialiseEnginePrimitiveSet()
{
    __asm
    {
        inc g_primitiveSet
        ret 4
    }
}

int main()
{
    g_primitiveVtable.slots[2] = (void*)&FableInitialiseEnginePrimitiveInitialise;
    g_primitiveVtable.slots[67] = (void*)&FableInitialiseEnginePrimitiveReset;
    g_primitiveVtable.slots[76] = (void*)&FableInitialiseEnginePrimitiveSet;
    *(void**)g_primitive = (void*)&g_primitiveVtable;
    *(void**)(g_component + 0x58) = (void*)g_primitive;
    *(void**)(g_system + 0x60) = (void*)g_systemNode;
    *(unsigned long*)(g_systemNode + 0x08) = 0x10203040;
    *(unsigned long*)(g_component + 0x10) = 0x11111111;
    *(unsigned long*)(g_component + 0x40) = 0x22222222;
    CNewFrontendGameComponent_InitialiseEngine_0042e204(g_component);
    if (g_primitiveCreate != 1 || g_reset != 1 || g_charCtor != 1 ||
        g_preMain != 2 || g_getDef != 1 || g_stringCons != 1 ||
        g_engineCtor != 1 || g_systemGet != 1 || g_autoSave != 4 ||
        g_assign != 5 || g_wideDestroy != 5 || g_cacheDir != 1 ||
        g_engineDestroy != 1 || g_primitiveInitialise != 1 ||
        g_primitiveReset != 1 || g_primitiveSet != 1)
    {
        puts("INITIALISE_ENGINE_0042E204_TEST FAIL");
        return 1;
    }
    puts("INITIALISE_ENGINE_0042E204_TEST PASS");
    return 0;
}
