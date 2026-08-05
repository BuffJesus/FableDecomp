#include <stdio.h>
#include <stdlib.h>

extern "C" void __fastcall CTable_GetDataFromDef_00551340(void*);

static volatile int g_releaseCount;
static void* g_definition;

extern "C" void* __cdecl FableCTableGetDataOperatorNew(unsigned int size)
{
    return malloc(size);
}

extern "C" __declspec(naked) void __fastcall
FableCTableGetDataVectorCopy(void*, const void*)
{
    __asm {
        mov eax, ecx
        mov edx, dword ptr [esp+4]
        mov ecx, dword ptr [edx]
        mov dword ptr [eax], ecx
        mov ecx, dword ptr [edx+4]
        mov dword ptr [eax+4], ecx
        mov ecx, dword ptr [edx+8]
        mov dword ptr [eax+8], ecx
        ret 4
    }
}

extern "C" void* __cdecl FableCTableGetDataManager(void)
{
    return 0;
}

extern "C" __declspec(naked) void* __cdecl
FableCTableGetDataCreateComponent(void*, long)
{
    __asm xor eax, eax
    __asm ret 4
}

extern "C" __declspec(naked) void* __cdecl
FableCTableGetDataLookup(void*, void*)
{
    __asm xor eax, eax
    __asm ret 4
}

extern "C" void __cdecl FableCTableGetDataOperatorDelete(void* object)
{
    free(object);
}

extern "C" __declspec(naked) void __fastcall
DefinitionProvider(void*)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr g_definition
        mov dword ptr [eax], edx
        ret 4
    }
}

extern "C" __declspec(naked) void __fastcall
FinalRelease(void*)
{
    __asm {
        inc dword ptr g_releaseCount
        ret
    }
}

int main()
{
    unsigned char object[0x180] = {0};
    unsigned char definition[0x100] = {0};
    unsigned char tree[0x20] = {0};
    void* vtable[0x70] = {0};

    vtable[0x1b0 / 4] = (void*)DefinitionProvider;
    vtable[4 / 4] = (void*)FinalRelease;
    *(void**)object = vtable;
    *(int*)(object + 4) = 1;
    *(int*)(definition + 0x58) = 0x22;
    *(int*)(definition + 0x5c) = 0x11;
    *(void**)definition = vtable;
    *(int*)(definition + 4) = 1;
    *(void**)(definition + 0x64) = tree;
    *(void**)tree = vtable;
    *(int*)(tree + 4) = 1;
    *(void**)(tree + 8) = tree;
    g_definition = definition;

    CTable_GetDataFromDef_00551340(object);

    if (*(int*)(object + 0xcc) != 0x11 || *(int*)(object + 0xd0) != 0x22)
        return 1;
    if (*(void**)(object + 0x168) == 0 || *(void**)(object + 0x16c) == 0)
        return 2;
    if (g_releaseCount != 1)
        return 3;

    FableCTableGetDataOperatorDelete(*(void**)(object + 0x168));
    FableCTableGetDataOperatorDelete(*(void**)(object + 0x16c));
    puts("CTable_GetDataFromDef_00551340_test PASS");
    return 0;
}
