#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CList_ProcessEvent_0053673b(void*);

static void* g_manager;
static unsigned long g_globalListStorage[1];
extern "C" void* FableCListProcessEventGlobalListHead = g_globalListStorage;
static int g_slot2d8Count;
static int g_slot2dcCount;

extern "C" void* FableCListProcessEventGetInstance()
{
    return g_manager;
}

extern "C" __declspec(naked) void FakeManagerGate()
{
    __asm {
        xor eax, eax
        ret
    }
}

static unsigned char* g_resource;

extern "C" void* FableCListProcessEventGetResource()
{
    return g_resource;
}

extern "C" __declspec(naked) void FakeBaseAcquire()
{
    __asm {
        mov eax, dword ptr [esp+4]
        push eax
        call FableCListProcessEventGetResource
        pop edx
        mov dword ptr [edx], eax
        ret 4
    }
}

extern "C" __declspec(naked) void FakeEventSlot2dc()
{
    __asm {
        inc dword ptr g_slot2dcCount
        ret
    }
}

extern "C" __declspec(naked) void FakeEventSlot2d8()
{
    __asm {
        inc dword ptr g_slot2d8Count
        ret
    }
}

extern "C" __declspec(naked) void CallProcessEvent(void*, long)
{
    __asm {
        mov ecx, dword ptr [esp+4]
        push dword ptr [esp+8]
        call CList_ProcessEvent_0053673b
        ret
    }
}

int main()
{
    unsigned char listStorage[0x400];
    unsigned char managerStorage[0x100];
    unsigned char resourceStorage[0x300];
    unsigned char baseVtable[0x300];
    unsigned char managerVtable[0x100];
    unsigned char resourceVtable[0x300];
    unsigned char* list = listStorage + 4;

    memset(listStorage, 0, sizeof(listStorage));
    memset(managerStorage, 0, sizeof(managerStorage));
    memset(resourceStorage, 0, sizeof(resourceStorage));
    memset(baseVtable, 0, sizeof(baseVtable));
    memset(managerVtable, 0, sizeof(managerVtable));
    memset(resourceVtable, 0, sizeof(resourceVtable));

    *(void**)managerStorage = managerVtable;
    *(void**)(managerVtable + 0x2c) = (void*)FakeManagerGate;
    g_manager = managerStorage;

    *(void**)resourceStorage = resourceVtable;
    *(long*)(resourceStorage + 4) = 3;
    g_resource = resourceStorage;

    *(void**)listStorage = baseVtable;
    *(void**)(baseVtable + 0x1b0) = (void*)FakeBaseAcquire;
    *(void**)(baseVtable + 0x2dc) = (void*)FakeEventSlot2dc;
    *(void**)(baseVtable + 0x2d8) = (void*)FakeEventSlot2d8;

    *(float*)(list + 0x2c) = 2.0f;
    *(float*)(list + 0x15c) = 0.0f;
    *(float*)(list + 0x1e8) = 1.0f;
    g_globalListStorage[0] = (unsigned long)g_globalListStorage;

    CallProcessEvent(list, 0);
    CallProcessEvent(list, 1);

    if (g_slot2dcCount != 1)
        return 1;
    if (g_slot2d8Count != 1)
        return 2;

    puts("CList_ProcessEvent_0053673b_test PASS");
    return 0;
}
