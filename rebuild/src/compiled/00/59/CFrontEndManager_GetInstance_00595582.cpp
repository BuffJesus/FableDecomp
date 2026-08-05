// NUISystem::CFrontEndManager::GetInstance @ 0x00595582
//
// The retail singleton accessor is compiled for size at this leaf.  The
// explicit failure branch preserves retail's `jmp`/`xor eax,eax` sequence,
// while the one-argument fastcall constructor declaration preserves the
// untouched EDX register in the constructor call.  No assembly is needed.

#pragma optimize("s", on)

extern "C" void* __cdecl
FableFrontEndManagerAllocate(unsigned long bytes);
extern "C" void* __fastcall
FableFrontEndManagerConstructInstance(void* storage);

extern "C" void* FableFrontEndManagerSingleton;

extern "C" void* __fastcall
CFrontEndManager_GetInstance_00595582()
{
    void* instance = FableFrontEndManagerSingleton;
    if (instance == 0)
    {
        instance = FableFrontEndManagerAllocate(0xe0);
        if (instance != 0)
            instance = FableFrontEndManagerConstructInstance(instance);
        else
            instance = 0;
        FableFrontEndManagerSingleton = instance;
    }
    return instance;
}

#pragma optimize("s", off)
